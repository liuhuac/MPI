/* C Example */ 
#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h> 
#include "functions.h"
int main (int argc, char *argv[]) 
{ 
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

	double my_value;

	my_value=rank;

	double *result=(double*)malloc(sizeof(double));

	*result=0;

	global_sum(result, rank, size, my_value);

	printf("Process [%d] has my_value = %f, result = %f\n", rank, my_value, *result);

	free(result);

	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
