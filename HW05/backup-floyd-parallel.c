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
	void *subvector; /*subvector*/
	void **subs; /*2D array*/
	void *storage; /*Array elements*/

	int dim[2], period[2], reorder;
	dim[0]=size;
	dim[1]=1;
	period[0]=0;
	period[1]=0;
	reorder=1;
	MPI_Comm comm;
	MPI_Comm rowcomm;
	MPI_Comm colcomm;
	int remain_dims[2];

	MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
	remain_dims[0]=1;
	remain_dims[1]=0;
	MPI_Cart_sub(comm, remain_dims, &rowcomm);
	remain_dims[0]=0;
	remain_dims[1]=0;
	MPI_Cart_sub(comm, remain_dims, &rowcomm);

	read_checkerboard_matrix_square (
		ifile,		/* IN - File name */
		&subs,         	/* OUT - 2D array */
		&storage,       /* OUT - Array elements */
		MPI_INT,   	/* IN - Element type */
		&n,	        /* OUT - Array dimension */
		comm);		/* IN - Communicator */

	print_checkerboard_matrix (
		subs,           /* IN -2D matrix */
		MPI_INT,        /* IN -Matrix element type */
		n,            	/* IN -Matrix rows */
		n,            	/* IN -Matrix columns */
		comm);    	/* IN - Communicator */
	

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_start=MPI_Wtime();

	int coord[2];
	MPI_Cart_coords(comm, rank, 2, coord);
	local_rows = BLOCK_SIZE(coord[0],dim[0],n);
	local_cols = BLOCK_SIZE(coord[1],dim[1],n);

	int i,j,k;
	int ii;
	for(k=0;k<n;k++){

		xk_coord[0]=coord[0];
		xk_coord[1]=BLOCK_OWNER(k,dim[1],n);
		ky_coord[0]=BLOCK_OWNER(k,dim[0],n);
		ky_coord[1]=coord[1];
		MPI_Cart_rank(comm, xk_coord, &xk_rank);
		MPI_Cart_rank(comm, ky_coord, &ky_rank);
		row_offset=BLOCK_LOW(k,dim[0],n);
		col_offset=BLOCK_LOW(k,dim[1],n);

		if(rank==xk_rank){
			xk_rows=BLOCK_SIZE(xk_coord[0],dim[0],n);
			for(ii=0;ii<xk_rows;ii++){
				xk_storage[ii]=subs[ii][col_offset];
			}
			
			MPI_Send(xk_storage, xk_rows, MPI_INT, dest, tag,comm)
		} else {
			recv
		}
		if(rank==ky_rank){
			ky_cols=BLOCK_SIZE(ky_coord[1],dim[1],n);
			for(ii=0;ii<ky_cols;ii++){
				ky_storage[ii]=subs[row_offset][ii];
			}
			send
		} else {
			recv
		}

		for(i=0;i<local_rows;i++){
			for(j=0;j<local_cols;j++){
				if(xk_storage[i]==-1||ky_storage[j]==-1){
					continue;
				}
				if(subs[i][j]>xk_storage[i]+ky_storage[j]||subs[i][j]==-1){
					subs[i][j]=xk_storage[i]+ky_storage[j];
				}
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double entire_stop=MPI_Wtime();

	if(rank==0){
		printf("ENTIRE:%f\tCOMPUTATION:%f\n",
			entire_stop-entire_start,
			entire_stop-comp_start);
		fflush(stdout);
	}

	free(subs);
	free(storage);
	free(ifile);
	free(ofile);

	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
