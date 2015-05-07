/*------------------------- mm-parallel.c  ---------------------------
 |  Purpose: This program is a parallel version of mm-serial. It 
 |  reads two matrix files, computes Cannon's matrix muliplication, 
 |  writes resulting matrix, to be stored in the same format.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.19
 *-------------------------------------------------------------------*/
#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <getopt.h>
#include "MyMPI.h"

void usage(void){
        const char *msg=
        "floyd-parallel [OPTIONS]\n"
        "OPTIONS:\n"
        "-i\tinput file name\n"
        "\tdefault default-make-matrix-file.dat\n"
        "-m\tinput file2 name\n"
        "\tdefault default-make-matrix-file2.dat\n"
	"-o\toutput file name\n"
	"\tdefault default-make-matrix-file.seq\n";
        printf("usage: %s", msg);
        exit(0);
}

int main (int argc, char *argv[]) 
{
        int opt;
	char *ifile=NULL;
	char *ifile2=NULL;
	char *ofile=NULL;
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

	int rank, size; /* rank is your pid, staring with 0 */
	/* size, is the number of processes you */
	/* run the program with */
	/* never make MPI calls before this and */
	/* never touch argc and argv before doing this */ 
	MPI_Init (&argc, &argv);
	/* get current process id */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	/* get number of processes */
	MPI_Comm_size (MPI_COMM_WORLD, &size); 

	MPI_Barrier(MPI_COMM_WORLD);
	double entire_start=MPI_Wtime();

	int n,m; /*vector length*/
	void **A,**B; /*2D array*/
	void *sA,*sB; /*Array elements*/

	MPI_Comm comm;

	int dim[2], period[2], reorder;
	period[0]=1;
	period[1]=1;
	reorder=1;
	dim[0]=0;
	dim[1]=0;
	MPI_Dims_create(size, 2, dim);

	if(dim[0]!=dim[1]&&rank==0){
		printf("Number of processors %d is not a squre number\n",size);
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_DIMS);
	}

	MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
	read_checkerboard_matrix_square (
		ifile,		/* IN - File name */
		&A,         	/* OUT - 2D array */
		&sA,	        /* OUT - Array elements */
		MPI_DOUBLE,   	/* IN - Element type */
		&n,	        /* OUT - Array dimension */
		comm);		/* IN - Communicator */
	read_checkerboard_matrix_square (
		ifile2,		/* IN - File name */
		&B,         	/* OUT - 2D array */
		&sB,       	/* OUT - Array elements */
		MPI_DOUBLE,   	/* IN - Element type */
		&m,	        /* OUT - Array dimension */
		comm);		/* IN - Communicator */
	if(n!=m){
		printf("Size of two input matrics does not match!\n");
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_DIMS);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_start=MPI_Wtime();

	int my2drank;
	int coord[2];
	MPI_Comm_rank(comm,&my2drank);
	MPI_Cart_coords(comm, my2drank, 2, coord);

	/* Determine the dimension of the local matrix block */
	int nlocal=n/dim[0];

	/* Perform the initial matrix alignment. First for A and then for B */
	int shift_src, shift_dest;
	MPI_Status status;
	MPI_Cart_shift(comm,1,-coord[0],&shift_src,&shift_dest);
	MPI_Sendrecv_replace(sA,nlocal*nlocal,MPI_DOUBLE,shift_dest,my2drank,shift_src,shift_src,comm,&status);

	MPI_Cart_shift(comm,0,-coord[1],&shift_src,&shift_dest);
	MPI_Sendrecv_replace(sB,nlocal*nlocal,MPI_DOUBLE,shift_dest,my2drank,shift_src,shift_src,comm,&status);

	/* Compute ranks of the up and left shifts */
	int left_rank, right_rank;
	int up_rank, down_rank;
	MPI_Cart_shift(comm,1,-1,&right_rank,&left_rank);
	MPI_Cart_shift(comm,0,-1,&down_rank,&up_rank);

	/* Malloc space for array C */
	double **C=(double**)calloc(nlocal,sizeof(double*));
	if(C==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
	}
	double *sC=(double*)calloc(nlocal*nlocal,sizeof(double));
	if(sC==NULL){
		printf("Error: Malloc failed!\n");
                fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
	}
	int i;
	for(i=0;i<nlocal;i++){
		C[i]=sC+i*nlocal;
	}

	int j,k,l;
	for(k=0;k<dim[0];k++){
		for(i=0;i<nlocal;i++){
			for(j=0;j<nlocal;j++){
				for(l=0;l<nlocal;l++){
					C[i][j]+=((double**)A)[i][l]*((double**)B)[l][j];
				}
			}
		}
		MPI_Sendrecv_replace(sA,nlocal*nlocal,MPI_DOUBLE,left_rank,my2drank,right_rank,right_rank,comm,&status);
		MPI_Sendrecv_replace(sB,nlocal*nlocal,MPI_DOUBLE,up_rank,my2drank,down_rank,down_rank,comm,&status);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_stop=MPI_Wtime();

	print_checkerboard_matrix_to_file (
		(void**)C,     /* IN -2D matrix */
		MPI_DOUBLE,     /* IN -Matrix element type */
		n,            	/* IN -Matrix rows */
		n,            	/* IN -Matrix columns */
		comm,    	/* IN - Communicator */
		ofile);		/* IN - Output file name */
		MPI_Barrier(MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	double entire_stop=MPI_Wtime();

	MPI_Comm_free(&comm);

	if(rank==0){
		printf("mm-parallel execution time:\n");
		printf("\tn = %d nodes\n\tp = %d cpus\n",n,size);
		printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
			entire_stop-entire_start,
			comp_stop-comp_start);
		fflush(stdout);
	}

	free(A);
	free(sA);
	free(B);
	free(sB);
	free(C);
	free(sC);
	free(ifile);
	free(ifile2);
	free(ofile);

	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
