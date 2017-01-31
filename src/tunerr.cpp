

#include <stdio.h>
#include "TunErr.h"
#include "Config.h"
#include <sys/signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>


TunErr* tunerr;
void read_config_file(int sigtype);

int get_tunerr_pid(char* argv0);
int get_process_name(char* proc_filename, char* ret, int cap);
int process_command_line(int pid, int argc, char** argv);
void usage(char* argv0);

bool delete_rule(int n);
bool file_exists(const char* filename);
bool add_rule(char** argv, int argc, int& argi);
bool clear_rules();


int main(int argc, char** argv)
{
	int pid = get_tunerr_pid(argv[0]);
	if (pid != 0) {																		// if tunerr is already running
		char** cargv = new char*[argc];
		
		for (int i=0; i<argc; i++)
			cargv[i] = strdup(argv[i]);
		
		int retv =process_command_line(pid, argc, cargv);		// send the informations and terminate
		
		for (int i=0; i<argc; i++)
			free(cargv[i]);
		
		delete [] cargv;
		
		return retv;
	}
	else if (argc > 1)
			printf("Warning, tunerr is not running, command line will be ignored.\n");

	tunerr = new TunErr();
	
	signal(SIGHUP, read_config_file);
	
	if (!tunerr->init()) {
		printf("Error initializint tunerr. exiting.\n");
		return -1;
	}
	
	int ret = tunerr->main();		// main loop
	
	delete tunerr;						// clean
	return ret;								// and exit
}



void read_config_file(int sigtype)
{
	if (tunerr)
		tunerr->init();
	reloadConfig(sigtype, true);
	
	signal(SIGHUP, read_config_file);
}



int get_tunerr_pid(char* argv0)
{
	DIR* dir;
	struct dirent* entry;
	char cmdline[128];
	char path[128];
	
	int mypid = getpid();
	int thispid;
	
	
	if ((dir=opendir("/proc")) == NULL) {
		printf("Fatal: Can't access proc dir.\n");
		exit(-1);
	}

	while ((entry = readdir(dir)) != NULL) {
		
		if (!isdigit(*entry->d_name))
			continue;
		
		sprintf(path, "/proc/%s/cmdline", entry->d_name);
		
		if (get_process_name(path, cmdline, 128) != -1)
			if (strcmp(argv0, cmdline) == 0) {
				thispid = atoi(entry->d_name);
				
				if (thispid != mypid) {
					closedir(dir);
					return thispid;
				}
			}
	}
	
	closedir(dir);
	return 0;
}


	
int get_process_name(char* proc_filename, char* ret, int cap)
{
	int fd, num_read;
	
	if ((fd = open(proc_filename, O_RDONLY, 0)) == -1)
		return -1;
	if ((num_read = read(fd, ret, cap)) <= 0)
		return -1;
	ret[num_read] = '\0';
	close(fd);
	
	return num_read;
}



int process_command_line(int pid, int argc, char** argv)
{
	int argi=1;
	Config* conf;
	int intarg;
	bool reload = 0;

	if (argc == 1) {
		usage(argv[0]);
		return 0;
	}

	
	while (argi < argc) {
		if (strcmp(argv[argi], "--add") == 0) {
			argi++;

			if (add_rule(argv, argc, argi))
				reload = 1;
		}
		else if (strcmp(argv[argi], "--clear") == 0) {
			argi++;
			if (clear_rules())
				reload = 1;
		}
		else if (strcmp(argv[argi], "--delete") == 0) {
			argi++;
			intarg = atoi(argv[argi++]);
			
			if (intarg < 0) {
				usage(argv[0]);
				return -1;
			}
			
			if (delete_rule(intarg))			
				reload = 1;
		}
		else if (strcmp(argv[argi], "--help") == 0) {
			usage(argv[0]);
			argi++;
		}
		else if (strcmp(argv[argi], "--reload") == 0) {
			argi++;
			reload = 1;
		}
		else if (strcmp(argv[argi], "--show") == 0) {
			Config::initialize(false);
			conf = Config::getInstance();
			
			conf->display();			
			argi++;
		}
		else if (strcmp(argv[argi], "--terminate") == 0  ||  strcmp(argv[argi], "--quit") == 0) {
			kill(pid, SIGTERM);
			return 0;
		}
		else {
			printf("unknown parameter on command line: \"%s\"\n", argv[argi]);
			return -1;			
		}
	}

	if (reload)
		kill(pid, SIGHUP);
	
	return 0;
}



void usage(char* argv0)
{
	printf("usage:\n");
	printf("%s --add [matching <matcher>] policy [ debug | discard <prob> | error <errors> | bandlimit <limit> | nothing]\n", argv0);
	printf("%s --clear\n", argv0);
	printf("%s --delete <rulenumber>\n", argv0);
	printf("%s --help\n", argv0);
	printf("%s --reload\n", argv0);
	printf("%s --show\n", argv0);
	printf("%s --terminate\n", argv0);
}


bool delete_rule(int n)
{
	char filename[128];
	sprintf(filename, "./rules/rule%d", n);
	
	return (unlink(filename) == 0);
}


bool file_exists(const char* filename)
{
	if (FILE* file = fopen(filename, "r")) {
		fclose(file);
		return true;
	}
	else
		return false;
}


bool add_rule(char** argv, int argc, int& argi)
{
	char filename[128] = { 0 };
	int n;
	
	for (n=0; ; n++) {
		sprintf(filename, "./rules/rule%d", n);
		
		if (!file_exists(filename))
			break;
	}

	
	// filename è il nome del file da scrivere.	
	FILE* rule_file = fopen(filename, "w");
	
	while (argi < argc) {
		if (strcmp(argv[argi], "matching") == 0  ||  strcmp(argv[argi], "matching:") == 0) {
			argi++;
			
			if (strcmp(argv[argi], "ip:") == 0  ||  strcmp(argv[argi], "udp:") == 0  ||  strcmp(argv[argi], "tcp:") == 0) {
				fprintf(rule_file, "%s ", argv[argi++]);
				
				while (1) {
					if (argi+2 >= argc) {
						printf("error in matching command line: missing argument.\n");
						goto cmdline_error;
					}
					
					fprintf(rule_file, "%s %s %s", argv[argi], argv[argi+1], argv[argi+2]);
					argi += 3;
					
					if (argi == argc  ||  strcmp(argv[argi], "policy") == 0  ||  strcmp(argv[argi], "policy:") == 0) {
						fprintf(rule_file, ";\n");
						break;
					}
					else
						fprintf(rule_file, ", ");
				}
			}
			else if (strcmp(argv[argi], "all") == 0) {
				argi++;
				fprintf(rule_file, "all;\n");
			}
			else {
				printf("wrong matching parameter (%s)\n", argv[argi]);
				goto cmdline_error;
			}
		}
		else if (strcmp(argv[argi], "policy") == 0  ||  strcmp(argv[argi], "policy:") == 0) {
			argi++;
			fprintf(rule_file, "policy: ");

			if (strcmp(argv[argi], "nothing") == 0) {
				argi++;
				fprintf(rule_file, "nothing;\n");
				break;
			}
			else if (strcmp(argv[argi], "debug") == 0) {
				argi++;
				fprintf(rule_file, "debug;\n");
				break;
			}
			else if (strcmp(argv[argi], "discard") == 0  ||  strcmp(argv[argi], "drop") == 0) {
				argi++;
				
				if (argc == argi) {
					printf("missing parameter after discard.\n");
					goto cmdline_error;
				}
				
				fprintf(rule_file, "discard %s;\n", argv[argi++]);
				break;
			}
			else if (strcmp(argv[argi], "bandlimit") == 0) {
				argi++;
				
				if (argc == argi) {
					printf("missing parameter after bandlimit.\n");
					goto cmdline_error;
				}
				
				fprintf(rule_file, "bandlimit %s;\n", argv[argi++]);
				break;
			}
			else if (strcmp(argv[argi], "error") == 0) {
				argi++;
				fprintf(rule_file, "error");
				
				while (argi < argc) {
					if (strcmp(argv[argi], "affect") == 0) {
						argi++;
						
						if (argc == argi) {
							printf("missing parameter after affect.\n");
							goto cmdline_error;
						}
						
						fprintf(rule_file, " affect %s", argv[argi++]);
					}
					else if (strcmp(argv[argi], "prob") == 0) {
						argi++;
						
						if (argc <= argi+1) {
							printf("missing parameter after prob.\n");
							goto cmdline_error;
						}
						
						fprintf(rule_file, " prob %s %s", argv[argi], argv[argi+1]);
						argi+=2;						
					}
					else {
							printf("unrecognized param (%s) in error, expected affect or prob.\n", argv[argi]);
							goto cmdline_error;
					}
				}
				
				fprintf(rule_file, ";\n");
			}
			else {
				printf("unrecognized parameter after error (%s).\n", argv[argi]);
				goto cmdline_error;
			}
		}
		else {
			printf("wrong parameter (%s) on command line, expected \"matching\" or \"policy\".\n", argv[argi]);
cmdline_error:
			fclose(rule_file);
			unlink(filename);
			return false;
		}
	}
	
	fclose(rule_file);
	return true;
}


bool clear_rules()
{
	DIR* dir = opendir("./rules");
	struct dirent* dp;
	char filename[128];
	bool retval = false;
	
	if (dir != NULL)
		while ((dp=readdir(dir)) != NULL) {
			if (strcmp(dp->d_name, ".") == 0   || strcmp(dp->d_name, "..") == 0)
				continue;
			
			sprintf(filename, "./rules/%s", dp->d_name);
			
			if (unlink(filename) == 0)
				retval = true;
		}
	
	closedir(dir);
	
	return retval;
}
