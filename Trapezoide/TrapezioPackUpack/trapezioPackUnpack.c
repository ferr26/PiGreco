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
   printf("pi = %lf\n", pi );

}
int main(int argc, char** argv){

  int rank, size;
  MPI_Status status;
  double countProcess;
  MPI_Init(&argc, &argv);
  double sommaTotale=0,somma=0.0;
  double invio[100];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank );  
  int position =0;
  double start,finish, time;


  char *buffer = malloc(200 *sizeof(char));
  start = MPI_Wtime(); 
  countProcess = countTrapezio(rank,size); 

  if(rank ==0) {
     sommaTotale = countProcess;
      for(int i = 1; i<size; i++){
      MPI_Recv(buffer, 110, MPI_PACKED, i, 99, MPI_COMM_WORLD,MPI_STATUS_IGNORE);   
      MPI_Unpack(buffer, 110 , &position, &invio, 1, MPI_DOUBLE, MPI_COMM_WORLD );
      int k=0;
      //sommiamo i vari risultati sviluppati dai vari processi
      sommaTotale += (double)invio[k]; 
      k++;
      }

      free(buffer);
      calcolaPi(sommaTotale);  
      finish = MPI_Wtime();
      printf("tempo di esecuzione %lf\n", (finish-start)*1000);

  } else {
      //codice per processi != 0 
      for(int j=0; j<size; j++){
      //salvo tutti i valori 
      //i quali verranno impacchettati all'interno di un buffer
      //e inviati con MPI_Send al master
      invio[j]=countProcess;
      MPI_Pack(invio, 1 , MPI_DOUBLE, buffer, 110, &position ,MPI_COMM_WORLD );
    }
      MPI_Send(buffer, position, MPI_PACKED, 0, 99, MPI_COMM_WORLD); 

  }
  MPI_Finalize();

  return 0;
}
