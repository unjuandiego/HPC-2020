#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Multiplicacion 
void mult_matriz(float **matriz1 , float **matriz2 , float **matrizR, int nn){

 float cont;
 for (int i = 0; i < nn; ++i){
     for (int j = 0; j < nn; ++j){
       cont = 0,0;
       for (int k = 0; k < nn; ++k){
         cont += matriz1[i][k] * matriz2[k][j];
       }
       matrizR[i][j] = cont;
    }
  }
}

//Llenar matriz
void matrix_dinamic(int col , int row , float **matriz ){

 if (matriz==NULL)
       printf ("Error de Mem.");
   for (int i = 0; i < row; ++i)
  {
     for (int j = 0; j < col; ++j)
     {
      float random = rand()%100;
      matriz[i][j] = random;
    }
  }
}

void save_matriz(float **Matrix, int row, int col){
 FILE *f = fopen("result_mult.csv", "a");

 if (f == NULL){
   printf("File error\n");
   exit(-1);
 }

 for (int i = 0; i < row; i++) {
   for (int j = 0; j < col; ++j){
     if(col - 1 == j){
       fprintf(f, "%.2f", Matrix[i][j]);
     }
     else{
       fprintf(f, "%.2f, ",  Matrix[i][j]);
     }
   }
    fprintf(f, "\n");
 }

 fprintf(f, "\n");
 fclose(f);

 return;
}

void write_solution(int n, double elp){
    FILE* fp = fopen("result_sec.txt", "a+");
    fprintf(fp, "%i %f\n",n,elp);
    fclose(fp);
}


int main(int argc, char const *argv[]){

  int n;
  clock_t time_ini, time_end;
  float **matriz1;
  float **matriz2;
  float **matrizR;
  double elapsed;

  sscanf (argv[1],"%d",&n);
  //Asignaciòn de memoria
  matriz1 = (float **)malloc (n*sizeof(float *));
  matriz2 = (float **)malloc (n*sizeof(float *));
  matrizR = (float **)malloc (n*sizeof(float *));

  for (int i=0;i<n;i++){
    matriz1[i] = (float *) malloc (n*sizeof(float));
    matriz2[i] = (float *) malloc (n*sizeof(float));
    matrizR[i] = (float *) malloc (n*sizeof(float)); 
  }

  matrix_dinamic(n,n,matriz1);
  matrix_dinamic(n,n,matriz2);

  time_ini = clock();
  mult_matriz(matriz1,matriz2,matrizR,n);
  time_end = clock();
  elapsed = (double)(time_end - time_ini)/CLOCKS_PER_SEC;
  printf("%s %.6f\n", "Tiempo de ejecución: ",elapsed);
  
  write_solution(n,elapsed);
  return 0;
}