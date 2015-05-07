/* C Example */ 
#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <getopt.h>
#include "functions.h"
#include "MyMPI.h"

void usage(void){
        const char *msg=
        "parallel-add-list [OPTIONS]\n"
        "OPTIONS:\n"
        "-i\tthe name of the datafile to add the integers\n"
        "\tdefault default-make-list-file.dat\n";
        printf("usage: %s", msg);
        exit(0);
}

int main (int argc, char *argv[]) 
{
        int opt;
	char *ofile=NULL;
        while((opt=getopt(argc, argv, "i:"))!=-1){
                switch(opt){
                        case 'i':
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

        if(ofile==NULL){
                ofile=strdup("default-make-list-file.dat");
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
	read_block_vector(ofile,&subvector,MPI_INT,&n,MPI_COMM_WORLD);

	int my_length=BLOCK_SIZE(rank,size,n);

	int i;
	double my_sum=0;

	MPI_Barrier(MPI_COMM_WORLD);
	double comp_start=MPI_Wtime();

	for(i=0;i<my_length;i++){
		my_sum+=(double)((int*)subvector)[i];
	}

	double *result=(double*)malloc(sizeof(double));
	*result=0;
	global_sum(result, rank, size, my_sum);

	//MPI_Barrier(MPI_COMM_WORLD);
	//double comp_stop=MPI_Wtime();

	//printf("Process [%d] has my_value = %f, result = %f\n", rank, my_sum, *result);

	MPI_Barrier(MPI_COMM_WORLD);
	double entire_stop=MPI_Wtime();

	if(rank==0){
		printf("SUM:%.0f\nENTIRE:%f\tCOMPUTATION:%f\n",
			*result,
			entire_stop-entire_start,
			entire_stop-comp_start);
		fflush(stdout);
	}

	free(result);
	free(subvector);
	free(ofile);

	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
