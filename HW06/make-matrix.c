/*--------------------------- make-matrix.c  ---------------------------
 |  Purpose: This program writes to output_file a binary file with an 
 |  n by n matrix representing a matrix of n nodes, representing one of
 |  the matrices to be multiplied, where the value of each element of
 |  the matrix is randomly generated based on a uniform random variable
 |  on the interval 'l' to 'u'. Data is 64-bit double precision floating
 |  point and the number n is stored as the first word of the file (this
 |  is an integer). The matrix is stored in row major order. 
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.19
 *-------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>
#include "graph.h"

void usage(void){
	const char *msg=
	"make-matrix [OPTIONS]\n"
	"OPTIONS:\n"
	"-n\tthe dimension of the matrix\n"
	"\tdefault n=100\n"
	"-l\tlower bound of random variable\n"
	"\tdefault l=0\n"
	"-u\tupper bound of random variable\n"
	"\tdefault u=100"
	"-o\toutput file name\n"
	"\tdefault default-make-matrix-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int n=0;
	int l=0;
	int u=0;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "n:l:u:o:"))!=-1){
		switch(opt){
			case 'n':
				n=atoi(optarg);
				if(n==0){
					printf("'%s' is an invalid parameter for -n!\n",optarg);
					usage();
				}
				break;
			case 'l':
				l=atoi(optarg);
				break;
			case 'u':
				u=atoi(optarg);
				if(u==0){
					printf("'%s' is an invalid parameter for -p!\n",optarg);
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

	if(n==0){
		n=100;
	}
	if(u==0){
		u=100;
	}
	if(ofile==NULL){
		ofile=strdup("default-make-matrix-file.dat");
	}

	double **A=(double**)malloc(n*sizeof(double*));
	if(A==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                exit(1);
	}
	double *tmp=(double*)malloc(n*n*sizeof(double));
	if(tmp==NULL){
		printf("Error: Malloc failed!\n");
		fflush(stdout);
		exit(1);
	}
	int i;
	for(i=0;i<n;i++){
		A[i]=tmp+i*n;
	}

	srand(time(NULL));
	int j;
	double variable;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			variable=l+((double)(u-l)*(double)rand())/((double)RAND_MAX);
			A[i][j]=variable;
		}
	}
	write_matrix(ofile,n,A);
	free(ofile);
	free(tmp);
	free(A);
	return 0;
}
