/*--------------------------- graph.c  -----------------------------
 |  Purpose: Helper functions for matrix processing.
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
#include "graph.h"

int read_matrix(char *file_name, int *n, double ***A){
	/*check file exist*/
        if(access(file_name,F_OK)==-1){
                printf("File %s does not exist! Use make-matrix to create file!\n", file_name);
                fflush(stdout);
                return 0;
        }

        /*open file*/
        FILE *fp=fopen(file_name,"rb");
        if(!fp){
                printf("Unable to open file %s!\n", file_name);
                fflush(stdout);
                return 0;
        }

	printf("reading matrix from file %s\n",file_name);

        fread(n,sizeof(int),1,fp);
	*A=(double**)malloc(*n*sizeof(double*));
	if(*A==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
                return 0;
	}
	double *tmp=(double*)malloc((*n)*(*n)*sizeof(double));
	if(tmp==NULL){
		printf("Error: Malloc failed!\n");
		fflush(stdout);
		return 0;
	}
	int i;
	for(i=0;i<*n;i++){
		(*A)[i]=tmp+i*(*n);
		fread((*A)[i],sizeof(double),*n,fp);
	}

        /*close file*/
        fclose(fp);

	return 1;
}

int write_matrix (char *file_name, int n, double **A){
	/*check file exist*/
        if(access(file_name,F_OK)!=-1){
                printf("File %s already exists! Start overwritting!\n", file_name);
        }

	/*open file*/
        FILE *fp=fopen(file_name,"wb");
        if(!fp){
                printf("Unable to open file %s!\n", file_name);
                return 0;
        }
       	printf("writting  matrix to file %s\n",file_name);
	fwrite(&n,sizeof(int),1,fp);
	fwrite(A[0],sizeof(double),n*n,fp);
        /*close file*/
        fclose(fp);
	return 1;
}
void print_matrix(int n, double **A){
	int i,j;
	char* space=" ";
	char* dash="----------";
	printf("Array is a %d x %d matrix\n\n",n,n);
	for(i=0;i<n;i++){
		if(i==0){
			printf("%10s |", space); /*first line*/
			for(j=0;j<n;j++){
				printf("%10d",j);
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
			printf("%10.2f",A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
