/*------------------------- floyd-serial.c  ---------------------------
 |  Purpose: This program reads a graph file, computes Floyd's shortest 
 |  pairs, writes resulting shortest path graph, to be stored in the 
 |  same format.
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
#include <time.h>
#include "graph.h"

void usage(void){
	const char *msg=
	"floyd-serial [OPTIONS]\n"
	"OPTIONS:\n"
	"-i\tintput file name\n"
	"\tdefault default-make-graph-file.dat\n"
	"-o\toutput file name\n"
	"\tdefault default-make-graph-file.seq\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int n=0;
	char *ifile=NULL;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "i:o:"))!=-1){
		switch(opt){
			case 'i':
				ifile=strdup(optarg);
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
		ifile=strdup("default-make-graph-file.dat");
	}
	if(ofile==NULL){
		ofile=strdup("default-make-graph-file.seq");
	}

	struct timespec entire_start;
	clock_gettime(CLOCK_MONOTONIC,&entire_start);

	int** A;
	read_graph(ifile, &n, &A);

	struct timespec comp_start;
        clock_gettime(CLOCK_MONOTONIC,&comp_start);

	int i,j,k;
	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(A[i][k]==-1||A[k][j]==-1){
					continue;
				}
				if(A[i][j]>A[i][k]+A[k][j]||A[i][j]==-1){
					A[i][j]=A[i][k]+A[k][j];
				}
			}
		}
	}

	struct timespec comp_stop;
        clock_gettime(CLOCK_MONOTONIC,&comp_stop);

	write_graph(ofile,n,A);;

	struct timespec entire_stop;
        clock_gettime(CLOCK_MONOTONIC,&entire_stop);

	printf("floyd-serial execution time:\n");
	printf("\tn = %d nodes\n\tp = 1 cpus\n",n);
	printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
		(entire_stop.tv_sec-entire_start.tv_sec)+(entire_stop.tv_nsec-entire_start.tv_nsec)*1e-9,
		(comp_stop.tv_sec-comp_start.tv_sec)+(comp_stop.tv_nsec-comp_start.tv_nsec)*1e-9);

	free(ifile);
	free(ofile);
	free(A[0]);
	free(A);
	return 0;
}
