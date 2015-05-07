#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
		exit(0);
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

//		MPI_Barrier(MPI_COMM_WORLD);

		*result+=recvbuf;
	}
	return;
}


