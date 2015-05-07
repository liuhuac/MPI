/*--------------------------- make-graph.c  ---------------------------
 |  Purpose: This program writes to output_file a binary file with an 
 |  n by n matrix representing a graph of n nodes, with directional 
 |  edge weights 0 to r and a probability of an edge of r/p.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.11
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
	"make-graph [OPTIONS]\n"
	"OPTIONS:\n"
	"-n\tthe dimension of the matrix\n"
	"\tdefault n=100\n"
	"-r\tr/p is the probability of having an edge\n"
	"\tdefault r=100\n"
	"-p\tr/p is the probability of having an edge\n"
	"\tdefault p=150"
	"-o\toutput file name\n"
	"\tdefault default-make-graph-file.dat\n";
	printf("usage: %s", msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int n=0;
	int r=0;
	int p=0;
	char *ofile=NULL;
	int opt;
	while((opt=getopt(argc, argv, "n:r:p:o:"))!=-1){
		switch(opt){
			case 'n':
				n=atoi(optarg);
				if(n==0){
					printf("'%s' is an invalid parameter for -n!\n",optarg);
					usage();
				}
				break;
			case 'r':
				r=atoi(optarg);
				if(r==0){
					printf("'%s' is an invalid parameter for -r!\n",optarg);
					usage();
				}
				break;
			case 'p':
				p=atoi(optarg);
				if(p==0){
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
	if(r==0){
		r=100;
	}
	if(p==0){
		p=150;
	}
	if(ofile==NULL){
		ofile=strdup("default-make-graph-file.dat");
	}

	int **A=(int**)malloc(n*sizeof(int*));
	if(A==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                exit(1);
	}
	int *tmp=(int*)malloc(n*n*sizeof(int));
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
	int u;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j){
				A[i][j]=0;
			} else {
				u=rand()%(p+1);
				if(u<=r){
					A[i][j]=u;
				} else {
					A[i][j]=-1;
				}
			}
		}
	}
	write_graph(ofile,n,A);
	free(ofile);
	free(tmp);
	free(A);
	return 0;
}
