/*------------------------- mm-serial.c  ---------------------------
 |  Purpose: Reads 2 input matrix files and computes the product of 
 |  the matrix, and then writes resulting product into the output file. 
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.19
 *-------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"

void usage(void){
	const char *msg=
	"mm-serial [OPTIONS]\n"
	"OPTIONS:\n"
	"-i\tintput file1 name\n"
	"\tdefault default-make-matrix-file.dat\n"
	"-m\tintput file2 name\n"
	"\tdefault default-make-matrix-file2.dat\n"
	"-o\toutput file name\n"
	"\tdefault default-make-matrix-file.seq\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int n=0;
	int m=0;
	char *ifile=NULL;
	char *ifile2=NULL;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "i:o:m:"))!=-1){
		switch(opt){
			case 'i':
				ifile=strdup(optarg);
				break;
			case 'm':
				ifile2=strdup(optarg);
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

	if(ifile==NULL){
		ifile=strdup("default-make-matrix-file.dat");
	}
	if(ifile2==NULL){
		ifile2=strdup("default-make-matrix-file2.dat");
	}
	if(ofile==NULL){
		ofile=strdup("default-make-matrix-file.seq");
	}

	struct timespec entire_start;
	clock_gettime(CLOCK_MONOTONIC,&entire_start);

	double** A;
	double** B;
	if(read_matrix(ifile, &n, &A)==0||read_matrix(ifile2, &m, &B)==0){
		free(ifile);
		free(ifile2);
		free(ofile);
		return 1;
	}
	
	if(n!=m){
		printf("Size of two input matrics does not match!\n");
		return 1;
	}
	
	double **C=(double**)malloc(n*sizeof(double*));
	if(C==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                return 1;
	}
	double *tmp=(double*)malloc(n*n*sizeof(double));
	if(tmp==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                return 1;
	}
	int i;
	for(i=0;i<n;i++){
		C[i]=tmp+i*n;
	}

	struct timespec comp_start;
	clock_gettime(CLOCK_MONOTONIC,&comp_start);

	int j,k;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			C[i][j]=0;
			for(k=0;k<n;k++){
				C[i][j]+=A[i][k]*B[k][j];
			}
		}
	}

	struct timespec comp_stop;
        clock_gettime(CLOCK_MONOTONIC,&comp_stop);

	if(write_matrix(ofile,n,C)==0){
		free(ifile);
		free(ifile2);
		free(ofile);
		free(A[0]);
		free(A);
		free(B[0]);
		free(B);
		free(C[0]);
		free(C);
		return 0;
	}

	struct timespec entire_stop;
        clock_gettime(CLOCK_MONOTONIC,&entire_stop);

	printf("mm-serial execution time:\n");
	printf("\tn = %d nodes\n\tp = 1 cpus\n",n);
	printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
		(entire_stop.tv_sec-entire_start.tv_sec)+(entire_stop.tv_nsec-entire_start.tv_nsec)*1e-9,
		(comp_stop.tv_sec-comp_start.tv_sec)+(comp_stop.tv_nsec-comp_start.tv_nsec)*1e-9);

	free(ifile);
	free(ifile2);
	free(ofile);
	free(A[0]);
	free(A);
	free(B[0]);
	free(B);
	free(C[0]);
	free(C);
	return 0;
}
