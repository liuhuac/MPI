/*------------------------- floyd-parallel.c  ------------------------
 |  Purpose: This program is a parallel version of floyd-serial. It 
 |  reads a graph file, computes Floyd's shortest pairs, writes 
 |  resulting shortest path graph, to be stored in the same format.
 |
 |  Author: Liuhua Chen
 |
 |  Time: 2014.11.12
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
        "\tdefault default-make-graph-file.dat\n"
	"-o\toutput file name\n"
	"\tdefault default-make-graph-file.seq\n";
        printf("usage: %s", msg);
        exit(0);
}

int main (int argc, char *argv[]) 
{
        int opt;
	char *ifile=NULL;
	char *ofile=NULL;
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

	int n; /*vector length*/
	void **subs; /*2D array*/
	void *storage; /*Array elements*/


	MPI_Comm comm;
	MPI_Comm rowcomm;
	MPI_Comm colcomm;


	int dim[2], period[2], reorder;
	period[0]=0;
	period[1]=0;
	reorder=1;
	dim[0]=0;
	dim[1]=0;
	MPI_Dims_create(size, 2, dim);

	MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);

	int remain_dims[2];

	remain_dims[0]=1;
	remain_dims[1]=0;
	MPI_Cart_sub(comm, remain_dims, &rowcomm);

	remain_dims[0]=0;
	remain_dims[1]=1;
	MPI_Cart_sub(comm, remain_dims, &colcomm);

	read_checkerboard_matrix_square (
		ifile,		/* IN - File name */
		&subs,         	/* OUT - 2D array */
		&storage,       /* OUT - Array elements */
		MPI_INT,   	/* IN - Element type */
		&n,	        /* OUT - Array dimension */
		comm);		/* IN - Communicator */

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_start=MPI_Wtime();

	int coord[2];
	MPI_Cart_coords(comm, rank, 2, coord);
	int local_rows=BLOCK_SIZE(coord[0],dim[0],n);
	int local_cols=BLOCK_SIZE(coord[1],dim[1],n);
	int rstart=BLOCK_LOW(coord[0],dim[0],n);
	int cstart=BLOCK_LOW(coord[1],dim[1],n);

	int *rbuf=(int*)malloc(sizeof(int)*local_cols);
	if(rbuf==NULL){
		printf ("Error: Malloc failed!\n");
		fflush (stdout);
		MPI_Abort (MPI_COMM_WORLD, MALLOC_ERROR);
	}

	int *cbuf=(int*)malloc(sizeof(int)*local_rows);
	if(cbuf==NULL){
		printf ("Error: Malloc failed!\n");
		fflush (stdout);
		MPI_Abort (MPI_COMM_WORLD, MALLOC_ERROR);
	}

	int rroot;
	int croot;
	int i,j,k;
	for(k=0;k<n;k++){

		rroot=BLOCK_OWNER(k,dim[0],n);
		croot=BLOCK_OWNER(k,dim[1],n);

		if(rroot==coord[0]){
			memcpy(rbuf, ((int**)subs)[k-rstart], local_cols*sizeof(int));
		}
		if(croot==coord[1]){
			int ii;
			for(ii=0;ii<local_rows;ii++){
				cbuf[ii]=((int**)subs)[ii][k-cstart];
			}
		}
		MPI_Bcast(rbuf,local_cols,MPI_INT,rroot,rowcomm);
		MPI_Bcast(cbuf,local_rows,MPI_INT,croot,colcomm);


		for(i=0;i<local_rows;i++){
			for(j=0;j<local_cols;j++){
				if(cbuf[i]==-1||rbuf[j]==-1){
					continue;
				}
				if(((int**)subs)[i][j]>cbuf[i]+rbuf[j]||((int**)subs)[i][j]==-1){
					((int**)subs)[i][j]=cbuf[i]+rbuf[j];
				}
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_stop=MPI_Wtime();

	print_checkerboard_matrix_to_file (
		subs,           /* IN -2D matrix */
		MPI_INT,        /* IN -Matrix element type */
		n,            	/* IN -Matrix rows */
		n,            	/* IN -Matrix columns */
		comm,    	/* IN - Communicator */
		ofile);		/* IN - Output file name */
		MPI_Barrier(MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	double entire_stop=MPI_Wtime();

	MPI_Comm_free(&rowcomm);
	MPI_Comm_free(&colcomm);
	MPI_Comm_free(&comm);

	if(rank==0){
		printf("floyd-parallel execution time:\n");
		printf("\tn = %d nodes\n\tp = %d cpus\n",n,size);
		printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
			entire_stop-entire_start,
			comp_stop-comp_start);
		fflush(stdout);
	}

	free(subs);
	free(storage);
	free(rbuf);
	free(cbuf);
	free(ifile);
	free(ofile);

	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
