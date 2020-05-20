#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define SEED 35791246
#define DEBUG false


double countMontecarlo(int niter, int rank, int size)
{ 
   int i, count=0;    
   double x,y,z;  
   //per avere sempre numeri random diversi
   srand(time(NULL));
   int quotient = niter / size;
   int rem = niter % size;
  
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

   for(i=init; i<n; i++){
      x=(double)rand()/RAND_MAX;
      y=(double)rand()/RAND_MAX;
    
      if(x*x+y*y<1.0)
        count++;
    }

   if(DEBUG) {
      printf("COUNT  -> rank %d, size %d, [%d - %d], count process %d\n", rank,size,init,n , count);
    }
  return count;
}
void calcolaPi(double countProcess,double niterTotale)
{ 
   double pi = 0.0;
   pi=(double)countProcess/niterTotale*4;
   printf("pi = %lf\n", pi );

}

int main(int argc, char** argv){


  int rank, size;
  MPI_Status status;
  int niterTotale=0;
  int iterProcess = 0;
  double countProcess = 0;
  double countAllProcess = 0;
  
  MPI_Init(&argc, &argv);
  int inter =0;
  double start,finish, time;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Barrier(MPI_COMM_WORLD);

  
  niterTotale = atoi(argv[1]);

  start = MPI_Wtime(); 
  countProcess = countMontecarlo(niterTotale,rank,size);

  MPI_Reduce(&countProcess,&countAllProcess,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

   //porzione di lavoro per master 
   if(rank ==0) {
      calcolaPi(countAllProcess,niterTotale);  
      finish = MPI_Wtime();
      printf("tempo di esecuzione %lf\n", (finish-start)*1000);
   }  

  MPI_Finalize();
  return 0;
}


