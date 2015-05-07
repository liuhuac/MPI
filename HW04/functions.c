#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int isPowerOfTwo (int x){

	if(x<=1) return 0;

	while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
		x /= 2;
	return (x == 1);
}

void global_sum(double* result, int rank, int size, double my_value){
	// error checking code
	if(!isPowerOfTwo(size)){
		printf("The number of processors must be power of 2!\n");
		MPI_Finalize();
		exit(1);
	}

	double recvbuf;

	MPI_Status status;

	*result=my_value; // add my_value to result

	while(0!=(size=size/2)){
		if(rank%(size*2)<size){
			MPI_Sendrecv(result, 1, MPI_DOUBLE, rank+size, size, &recvbuf, 1, MPI_DOUBLE, rank+size, size, MPI_COMM_WORLD, &status);
		} else {
			MPI_Sendrecv(result, 1, MPI_DOUBLE, rank-size, size, &recvbuf, 1, MPI_DOUBLE, rank-size, size, MPI_COMM_WORLD, &status);
		}

		*result+=recvbuf;
	}
	return;
}

void make_list(int n, int **A){
	int i;
	if((*A=(int*)malloc(n*sizeof(int)))==NULL){
		printf("Error: Malloc failed!\n");
		fflush(stdout);
		return;
	}
	for(i=0;i<n;i++){
		(*A)[i]=i;
	}
}

void write_list(char* out, int n, int* A){
        /*check file exist*/
        if(access(out,F_OK)!=-1){
                char ans;
                printf("File %s already exists! Do you want to overwrite it[y/n]?", out);
                scanf ("%c", &ans);
                if(ans!='y'){
                        printf("\nExit!\n");
                        exit(1);
                }
        }

        /*open file*/
        FILE *fp=fopen(out,"wb");
        if(!fp){
                printf("Unable to open file %s!\n", out);
                exit(1);
        }
        fwrite(&n,sizeof(int),1,fp);
	fwrite(A,sizeof(int),n,fp);
        /*close file*/
        fclose(fp);
}

void print_list(int n, int* A){
	int i;
	for(i=0;i<n;i++){
		printf("%d\n",A[i]);
	}
}

void read_list(char* out, int *n, int** A){
        /*check file exist*/
        if(access(out,F_OK)==-1){
		printf("File %s does not exist! Use make-list to create file!\n", out);
		fflush(stdout);
		exit(1);
        }

        /*open file*/
        FILE *fp=fopen(out,"rb");
        if(!fp){
                printf("Unable to open file %s!\n", out);
		fflush(stdout);
                exit(1);
        }
        fread(n,sizeof(int),1,fp);
        if((*A=(int*)malloc(*n*sizeof(int)))==NULL){
                printf("Error: Malloc failed!\n");
                fflush(stdout);
                return;
        }

	fread(*A,sizeof(int),*n,fp);

        /*close file*/
        fclose(fp);
}
