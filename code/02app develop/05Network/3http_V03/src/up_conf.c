#include <stdio.h>
#include "server.h"

extern  struct up_cfg_info up_conf;

static void print_conf_info(void)
{
	printf("---uphttpd config info----\n");
	printf("listen_port= %d\n", up_conf.listen_port);
	printf("document_root= %s\n", up_conf.doc_root_path);
	printf("conf_file= %s\n", up_conf.conf_file);
	printf("log_file= %s\n", up_conf.log_file);
	printf("run_as_deamon= %s\n", up_conf.run_as_daemon ? "True": "False");
	printf("verbose= %s\n", up_conf.verbose ? "True":"False");
	printf("Verion= %s [V%s]\n", PRG_NAME, VERSION);
	printf("---config info done-----\n");
}


void usage(const char *prg_name)
{
	printf("Usage: %s [-p|-d|-f|-D|-h|-v|-V]\n", prg_name);
	printf("\t-p --port: listen port\n");
	printf("\t-d --doc_root: document root path\n");
	printf("\t-f --log: log file path\n");
	printf("\t-D --daemon: run as daemon\n");
	printf("\t-h --help ?: help\n");
	printf("\t-v --verbose: verbose mode on\n");
	printf("\t-V --version: show version info\n");
}

/*命令行参数的解析*/
int cmd_parse(int argc, char *argv[])
{

	char *short_opts = "p:d:f:DhvV";
	/* p: port, d: document_root, f: log_file, D: Daemon, h: help*/
	static struct option long_opts[] = {
		{"port",     required_argument, 0,  'p' },
		{"doc_root", required_argument, 0,  'd' },
		{"log",  required_argument, 0,  'f' },
		{"verbose", no_argument,       0,  'v' },
		{"daemon",  no_argument, 0, 'D'},
		{"help",    no_argument, 0,  'h' },
		{"version", no_argument, 0,  'V' },
		{0,         0,                 0,  0 }
	};

	int c;
	int opt_index;

	while (1) {
		c = getopt_long(argc, argv, short_opts, long_opts,&opt_index);
		if (c <= 0) {
			break;
		}

		switch (c) {
			case 'd':
				strncpy(up_conf.doc_root_path, optarg, PATH_SIZE);
				break;
			case 'f':
				strncpy(up_conf.conf_file, optarg, PATH_SIZE);
				break;
			case 'p':
				up_conf.listen_port = atoi(optarg);	
				break;
			case 'D':
				up_conf.run_as_daemon = 1;
				break;
			case 'v':
	
				up_conf.verbose = 1;
				break;
			case 'V':
				printf("%s V%s\n", PRG_NAME, VERSION);
				exit(0);
				break;
			case 'h':
			case '?':
				usage(argv[0]);
				exit(-1);
				break;
			default:
				fprintf(stderr, "[Error]unkown opt '%c'\n", c);
				usage(argv[0]);
				exit(-1);
		}
	}
	//printf("optind = %d\n", optind);
#if 1
	if (optind != argc) {
		fprintf(stderr, "[Error]:有选项缺少参数\n");
		return -1;
	}
#endif
	return 0;
}

static int file_parse(const char *cfg_file)

{
	assert(cfg_file != NULL);

	FILE *pf = fopen(cfg_file, "r");
	if (pf == NULL) {
		ERR_PRINTF("fopen %s failed:", cfg_file);
		perror("");
		return -1;
	}

	char buf[BUF_SIZE] = {0};

	char *p  = NULL;
	char *name = NULL;
	char *val = NULL;
	/*解析配置文件*/
	while (!feof(pf)) {
		if (fgets(buf, BUF_SIZE, pf) == NULL) {
			break;
		}
		p = buf;
		/*空行*/
		if (*p == '\r' || *p == '\n' || *p == '#') {
			continue;
		}
		while (isspace(*p)) {
			++p;
		}
		/*注释 #*/
		if (*p == '#') {
			continue;
		}
		name = p;
		while (!isspace(*p) && *p != '=') {
			++p;
		}
		*p++ = '\0';
		while ((isspace(*p) && *p != '\r' && *p != '\n') || (*p == '=')) {
			++p;
		}
		val = p;
		while (!isspace(*p)) {
			++p;
		}
		*p = '\0';

		//DBG_PRINTF("name:(%s) val:(%s)\n", name, val);
	
		/*根据不同的name 设置不同的值*/
		if (strcmp(name, "port") == 0) {
			up_conf.listen_port = (unsigned short)atoi(val);
		}
		if (strcmp(name, "doc_root") == 0) {
			strncpy(up_conf.doc_root_path, val, PATH_SIZE);
		}
		if (strcmp(name, "conf_file") == 0) {
			strncpy(up_conf.conf_file, val, PATH_SIZE);
		}
		if (strcmp(name, "log_file") == 0) {
			strncpy(up_conf.log_file, val, PATH_SIZE);
		}
		if (strcmp(name, "daemon") == 0) {
			if (strcasecmp(val, "true") == 0) {
				up_conf.run_as_daemon = 1;
			} else {
				up_conf.run_as_daemon = 0;
			}
		}
		if (strcmp(name, "verbose") == 0) {
			if (strcasecmp(val, "true") == 0) {
				up_conf.verbose = 1;
			} else {
				up_conf.verbose = 0;
			}
		}
	}

	return 0;
}


int  up_conf_init(int argc, char *argv[])
{
	//1.命令行的参数解析
	int iret = cmd_parse(argc, argv);
	if (iret != 0) {
		ERR_PRINTF("cmd_parse failed.\n");
	}	
	//2.配置文件解析
	if (access(up_conf.conf_file, R_OK) == 0) {
		if (file_parse(up_conf.conf_file) < 0){
			ERR_PRINTF("file_parse failed.\n");
		}
	}
	//3.打印配置信息	
	if (up_conf.verbose) {
		print_conf_info();	
	}
	
	return 0;
}
