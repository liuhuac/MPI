/* C Example */ 
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
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

	int myNum;
	int LrankNum;
	int RrankNum;
	int L_rank=rank-1<0?size-1:rank-1;
	int R_rank=rank+1==size?0:rank+1;

	MPI_Status status[2];

	srand(rank+time(NULL));/* initialize random seed */
	myNum=rand()%101;

	printf("Process [%d] has myNum = %d, R_rank = %d, L_rank = %d\n", rank, myNum, R_rank, L_rank);

	MPI_Sendrecv(&myNum, 1, MPI_INT, L_rank, 1, &RrankNum, 1, MPI_INT, R_rank, 1, MPI_COMM_WORLD, &status[0]);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Sendrecv(&myNum, 1, MPI_INT, R_rank, 1, &LrankNum, 1, MPI_INT, L_rank, 1, MPI_COMM_WORLD, &status[1]);

	MPI_Barrier(MPI_COMM_WORLD);

	printf("Process [%d]: Recv'd from right = %d, Recv'd from left = %d\n", rank, RrankNum, LrankNum);
	MPI_Finalize(); /* don't make MPI calls after this */ 
	return 0; 
}
