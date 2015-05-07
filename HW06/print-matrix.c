/*--------------------------- print-matrix.c  ---------------------------
 |  Purpose: This program reads a matrix file created by make-matrix and 
 |  prints in ascii.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.11
 *-------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "graph.h"

void usage(void){
	const char *msg=
	"print-matrix [OPTIONS]\n"
	"OPTIONS:\n"
	"-i\tintput file name\n"
	"\tdefault default-make-matrix-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int n=0;
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
		ofile=strdup("default-make-matrix-file.dat");
	}

	double** A;
	if(read_matrix(ofile, &n, &A)==0){
		free(ofile);
		free(A[0]);
		free(A);
		return 1;
	}

	print_matrix(n, A);

	free(ofile);
	free(A[0]);
	free(A);
	return 0;
}
