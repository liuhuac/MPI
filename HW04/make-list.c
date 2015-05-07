#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"

void usage(void){
	const char *msg=
	"make-list [OPTIONS]\n"
	"OPTIONS:\n"
	"-n\tthe number of integers to place in the list\n"
	"\tdefault n=100\n"
	"-o\tthe name of the datafile to write the integers to\n"
	"\tdefault default-make-list-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int num=0;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "n:o:"))!=-1){
		switch(opt){
			case 'n':
				num=atoi(optarg);
				if(num==0){
					printf("'%s' is an invalid parameter for -n!\n",optarg);
					usage();
				}
				break;
			case 'o':
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

	if(num==0){
		num=100;
	}
	if(ofile==NULL){
		ofile=strdup("default-make-list-file.dat");
	}

	int* A;
	make_list(num,&A);
	write_list(ofile,num,A);

	free(ofile);
	free(A);
	return 0;
}
