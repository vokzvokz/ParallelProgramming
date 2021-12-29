#include <stdio.h>
#include "mpi.h"
#include<iostream>
#include<cstdlib>
#include<math.h>
#include<algorithm>

#define VECTOR_SIZE 1000000
#define in_range(var, max) for(auto var = 0; var < (max); var += 1)

using namespace std;



int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;


	int* vec;
  int* vec_part;
	int parts;
	//double sum = 0, part_sum = 0;
  unsigned long int sum_total, subsum;
	double start_time, finish_time;


	parts = VECTOR_SIZE / size;

	vec = new int[VECTOR_SIZE];

	if (rank == 0)
	   in_range(i, VECTOR_SIZE) vec[i] = 1;

  if (rank == 0) start_time = MPI_Wtime();

  // vector to passing further
  int n_sent = VECTOR_SIZE / size;
  vec_part = new int[n_sent];

  // passing further
  MPI_Scatter(vec, n_sent, MPI_INT, vec_part, n_sent, MPI_INT, 0, MPI_COMM_WORLD);

  // summing parts
  int vec_part_sum = 0;
  in_range(i, n_sent) vec_part_sum += vec_part[i];

  // cascade summing
  for(int level=2; level <= size; level *= 2) {
      if (rank % level == 0) {
          if (rank + level / 2 < size) {
            MPI_Recv(&subsum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            vec_part_sum += subsum;
          }
      } else {
        MPI_Send(&vec_part_sum, 1, MPI_INT, rank - rank % level, 0, MPI_COMM_WORLD);
        break;
      }
  }

  if (rank == 0){
    cout<<"Custom reduce result: "<<vec_part_sum<<endl;
    cout<<"------> Time spent: "<<MPI_Wtime()-start_time<<endl;

  }
  // Ended custom cascade summing

  // MPI reduce
  start_time = MPI_Wtime();
  vec_part_sum = 0;

  vec_part = new int[n_sent];
  subsum = 0;
  MPI_Scatter(vec, n_sent, MPI_INT, vec_part, n_sent, MPI_INT, 0, MPI_COMM_WORLD);

  in_range(i, n_sent) subsum += vec_part[i];

  MPI_Reduce(&subsum, &vec_part_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    cout<<"MPI_Reduce result: "<<vec_part_sum<<endl;
    cout<<"------> Time spent: "<<MPI_Wtime()-start_time<<endl;
  }


  delete[] vec;
	MPI_Finalize();
	return 0;
}
