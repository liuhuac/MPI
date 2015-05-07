/* C Example */ 
#include <stdio.h> 
#include <mpi.h> 
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
	int i;
	char a[3*rank+1];
	for(i=0;i<3*rank;i++){
		a[i]='_';
	}
	a[3*rank]='\0';
	for(i=0;i<size-rank;i++){
		printf( "%sHello world from process %d of %d\n", a, rank, size);
	} 
	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
