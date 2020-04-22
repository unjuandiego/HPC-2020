#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int **fst,**sec,**mult;

struct args
{
    int i;
    int n;
};

double ** allocate_matrix( int size )
{
  /* Allocate 'size' * 'size' doubles contiguously. */
  double * vals = (double *) malloc( size * size * sizeof(double) );

  /* Allocate array of double* with size 'size' */
  double ** ptrs = (double **) malloc( size * sizeof(double*) );

  int i;
  for (i = 0; i < size; ++i) {
    ptrs[ i ] = &vals[ i * size ];
  }

  return ptrs;
}

void genMatrix(int **mat, int n)
{
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
      mat[i][j] = rand() % n+1;
  }
}



void *valueCell(void *arguments)
{
  int i = ((struct args*)arguments)->i;
  int n = ((struct args*)arguments)->n;
  for(int j = 0; j < n; j++)
  {
    int r = 0;
    for(int k = 0; k < n; k++)
      r += fst[i][k] * sec[k][j];
    mult[i][j] = r;
  }
   pthread_exit(NULL);
}
void write_solution(int n, double elp){
    FILE* fp = fopen("result_hilos.txt", "a+");
    fprintf(fp, "%i %f\n",n,elp);
    fclose(fp);
}


int main(int argc, char *argv[])
{
  int n;
  sscanf (argv[1],"%d",&n);
  long int NUM_THREADS = n;
  
  fst = allocate_matrix(n);
  sec = allocate_matrix(n);
  mult = allocate_matrix(n);

  genMatrix(fst,n);
  genMatrix(sec,n);

   pthread_t threads[NUM_THREADS];
   int rc;
   long t = 0;
   clock_t start,end;
  start = clock();
   for(int i = 0; i < n; i++)
   {
      struct args *arguments = (struct args *)malloc(sizeof(struct args));
      arguments->i = i;
      arguments->n = n;
      pthread_create(&threads[t], NULL, valueCell,(void * )arguments);
      t++;
   }
   end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%s %.6f\n", "Tiempo de ejecución: ",cpu_time_used);
    write_solution(n,cpu_time_used);

   pthread_exit(NULL);
   return 0;
}