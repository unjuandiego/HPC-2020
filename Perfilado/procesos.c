#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


int **fst,**sec,**mult;
//static int **mult;

double **allocate_global_matrix(int size)
{
  /* Allocate 'size' * 'size' doubles contiguously. */
  double * vals = (double *) mmap(NULL, size * size * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  /* Allocate array of double* with size 'size' */
  double ** ptrs = (double **) mmap(NULL, size * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  int i;
  for (i = 0; i < size; ++i) {
    ptrs[ i ] = &vals[ i * size ];
  }

  return ptrs;
}


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


void proccess(int i,int n)
{
  //printf("%d \n",i);
    for(int j = 0; j < n; j++)
    {
        int r = 0;
        for(int k = 0; k < n; k++)
          r += fst[i][k] * sec[k][j];
        mult[i][j] = r;
    }
    //printMat(mult,n);
}
void write_solution(int n, double elp){
    FILE* fp = fopen("result_procesoso2.txt", "a+");
    fprintf(fp, "%i %f\n",n,elp);
    fclose(fp);
}



int main(int argc, char const *argv[])
{
    int n;
    sscanf (argv[1],"%d",&n);



    fst = allocate_matrix(n);
    sec = allocate_matrix(n);
    mult = allocate_global_matrix(n);

    genMatrix(fst,n);
    genMatrix(sec,n);
    clock_t start,end;
    start = clock();
    //procesos:
    pid_t *pids = malloc(argc * sizeof(pid_t));

    for(int i = 0; i < n; i = i + 2)
    {
      //printMat(mult,n);
        pid_t pid = fork();
        if(pid > 0)
        {
            pids[i] = pid;
            proccess(i,n);
        }
        else
        {
            proccess(i + 1,n);
            _exit(1);
        }
    }


    end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%s %.6f\n", "Tiempo de ejecución: ",cpu_time_used);
    write_solution(n,cpu_time_used);
    return 0;
}
