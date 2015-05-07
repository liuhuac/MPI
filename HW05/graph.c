/*--------------------------- graph.c  -----------------------------
 |  Purpose: Helper functions for graph processing.
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

void read_graph(char *file_name, int *n, int ***A){
	/*check file exist*/
        if(access(file_name,F_OK)==-1){
                printf("File %s does not exist! Use make-list to create file!\n", file_name);
                fflush(stdout);
                exit(1);
        }

        /*open file*/
        FILE *fp=fopen(file_name,"rb");
        if(!fp){
                printf("Unable to open file %s!\n", file_name);
                fflush(stdout);
                exit(1);
        }

	printf("reading graph from file %s\n",file_name);

        fread(n,sizeof(int),1,fp);
	*A=(int**)malloc(*n*sizeof(int*));
	if(*A==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                return;
	}
	int *tmp=(int*)malloc((*n)*(*n)*sizeof(int));
	if(tmp==NULL){
		printf("Error: Malloc failed!\n");
		fflush(stdout);
		return;
	}
	int i;
	for(i=0;i<*n;i++){
		(*A)[i]=tmp+i*(*n);
		fread((*A)[i],sizeof(int),*n,fp);
	}

        /*close file*/
        fclose(fp);
}

void write_graph (char *file_name, int n, int **A){
	/*check file exist*/
        if(access(file_name,F_OK)!=-1){
                printf("File %s already exists! Start overwritting!\n", file_name);
        }

	/*open file*/
        FILE *fp=fopen(file_name,"wb");
        if(!fp){
                printf("Unable to open file %s!\n", file_name);
                exit(1);
        }
       	printf("writting  graph to file %s\n",file_name);
	fwrite(&n,sizeof(int),1,fp);
	int i;
	for(i=0;i<n;i++){
		fwrite(A[i],sizeof(int),n,fp);
	}
        /*close file*/
        fclose(fp);

}
void print_graph(int n, int **A){
	int i,j;
	char* space=" ";
	char* dash="-----";
	printf("Array is a %d x %d matrix\n\n",n,n);
	for(i=0;i<n;i++){
		if(i==0){
			printf("%10s |", space); /*first line*/
			for(j=0;j<n;j++){
				printf("%5d",j);
			}
			printf("\n");
			printf("%10s |", space); /*second line*/
			for(j=0;j<n;j++){
                                printf("%s",dash);
                        }
			printf("\n");
		}
		printf("%10d |", i);
		for(j=0;j<n;j++){
			printf("%5d",A[i][j]);
		}
		printf("\n");
	}
}
