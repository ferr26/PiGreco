#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>
#define N 10000000
#define d 1E-7
#define d2 1E-14
#define DEBUG false

double countTrapezio(int rank, int size)
{
   double x2= 0.0, result = 0.0;
   int i;
   int quotient = N / size;
   int rem = N % size;
   
   if (rem > rank) {
      quotient++;
   } 
   //ottengo i valori per la dimensione degli intervalli
   //init il primo valore e n l'ultimo per il processore rank 
   int init = quotient * rank;
   int n = quotient * (rank+1);

   if (rem <= rank) {
      init+=rem;
      n+=rem;
   } 
   for (i=init; i<n; i++) {
      x2=d2*i*i;
      result+= 1.0/(1.0+x2);
    }

  if (DEBUG) {
      printf("COUNT  -> rank %d, size %d, [%d - %d], count process %lf\n", rank,size, init, n, result);
    }

  return result;
}

void calcolaPi(double countProcess)
{ 
   double pi = 0.0;
   pi=4*d*countProcess;
   printf("Il valore di Pi Greco è = %lf \n", pi );

}
int main(int argc, char** argv){

	int rank, size;
  double countProcess;
  MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start,finish, time;
  double countAllProcess = 0;
  start = MPI_Wtime(); 
  //ogni processo calcolerà una parte 
  countProcess = countTrapezio(rank,size);

  //sommiamo tutti i risultati
  MPI_Reduce(&countProcess,&countAllProcess,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);


  if (DEBUG) {
      printf("Process   -> rank %d, count process %lf\n", rank,countProcess);
   }

   //porzione di lavoro per master 
  if(rank ==0){   
    calcolaPi(countAllProcess); 
    finish = MPI_Wtime();
    printf("tempo di esecuzione %lf\n", (finish-start)*1000);

     if (DEBUG) {
      printf("MASTER -> rank %d, count process %lf\n", rank,countProcess);
     }
  }


  MPI_Finalize();

	return 0;
}
