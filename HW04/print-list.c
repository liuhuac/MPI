#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "functions.h"

void usage(void){
	const char *msg=
	"print-list [OPTIONS]\n"
	"OPTIONS:\n"
	"-i\tthe name of the datafile to print the integers\n"
	"\tdefault default-make-list-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "i:"))!=-1){
		switch(opt){
			case 'i':
				ofile=strdup(optarg);
				break;
			case '?':
			case ':':
			default :
				usage();
				break;
		}
	}

	if(optind!=argc){
		printf("Unknow argument '%s'\n",argv[optind]);
		usage();
	} else if(strcmp(argv[optind-1], "--")==0){
		printf("Unknow argument '%s'\n",argv[optind-1]);
		usage();
	}

	if(ofile==NULL){
		ofile=strdup("default-make-list-file.dat");
	}

	int* A;
	int n;
	read_list(ofile, &n, &A);
	print_list(n,A);
	free(ofile);
	free(A);
	return 0;
}
