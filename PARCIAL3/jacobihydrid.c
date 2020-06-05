#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>
#define nt 4
/* --
 * Exchange ghost cell data with neighboring processors
 */
void ghost_exchange(double* u, int n, int rank, int size)
{
    if (size == 1)
        return;

    /* YOUR SOLUTION HERE */

    // send endpoint values
    if (!rank == 0) {
        // send left endpoint values to processes to the "left"
        MPI_Send(&u[1], 1, MPI_DOUBLE, rank-1,
                 1, MPI_COMM_WORLD);
    }
    if (rank < size - 1) {
        // send right endpoint values to processes to the "right"
        MPI_Send(&u[n-1], 1, MPI_DOUBLE, rank+1,
                 1, MPI_COMM_WORLD);
    }

    //recv endpoint values
    MPI_Status status;
    if (rank < size - 1) {
        // receive right endpoint values
        MPI_Recv(&u[n], 1, MPI_DOUBLE, rank+1,
                     1, MPI_COMM_WORLD, &status);
    }
    if (rank > 0) {
        // receive left endpoint values
        MPI_Recv(&u[0], 1, MPI_DOUBLE, rank-1,
                     1, MPI_COMM_WORLD, &status);
    }
}

/* --
 * Do nsweeps sweeps of Jacobi iteration on a 1D Poisson problem
 * 
 *    -u'' = f
 *
 * discretized by n+1 equally spaced mesh points on [0,1].
 * u is subject to Dirichlet boundary conditions specified in
 * the u[0] and u[n] entries of the initial vector.
 */
void jacobi(int nsweeps, int n, double* u, double* f, double h2, 
            int rank, int size)
{
    int i, sweep;
    double h  = 1.0 / n;
    double* utmp = (double*) malloc( (n+1) * sizeof(double) );

    utmp[0] = u[0];
    utmp[n] = u[n];
    
    for (sweep = 0; sweep < nsweeps; sweep += 2) {
        
        /* Exchange ghost cells */
        ghost_exchange(u, n, rank, size);
        utmp[0] = u[0];
        utmp[n] = u[n];
        
        /* Sweep */
        #pragma omp for nowait
        for (i = 1; i < n; ++i)
            utmp[i] = (u[i-1] + u[i+1] + h2*f[i])/2;
        
        /* Exchange ghost cells */
        #pragma omp barrier
        ghost_exchange(utmp, n, rank, size);
        u[0] = utmp[0];
        u[n] = utmp[n];

        /* Old data in utmp; new data in u */
        #pragma omp for nowait
        for (i = 1; i < n; ++i)
            u[i] = (utmp[i-1] + utmp[i+1] + h2*f[i])/2;
    }

    free(utmp);
}


void write_solution(int n, int nloc, double* uloc, const char* fname,
                    int rank, int size)
{
    double h = 1.0 / n;
    MPI_Status status;

    if (rank == 0) {

        /* Output at processor 0 */
        FILE* fp = fopen(fname, "w+");
        int p, nrecv;
        int i, j = 0;
        for (i = 0; i < nloc; ++i, ++j)
            fprintf(fp, "%g %g\n", j*h, uloc[i]);
        for (p = 1; p < size; ++p) {

            MPI_Recv(uloc, nloc+1, MPI_DOUBLE, p,
                     1, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &nrecv);
            nloc = nrecv-1;

            for (i = 1; i < nloc; ++i, ++j)
                fprintf(fp, "%g %g\n", j*h, uloc[i]);

        }
        fprintf(fp, "%g %g\n", j*h, uloc[i]);
        fclose(fp);

    } else {
        
        /* Send at processor p > 0 */
        MPI_Send(uloc, nloc+1, MPI_DOUBLE, 0,
                 1, MPI_COMM_WORLD);

    }
}
void writes(int n, int u, double elp, const char* fname){
    FILE* fp = fopen(fname, "a+");
    fprintf(fp, "%i %i %f\n",n,u,elp );
    fclose(fp);
}


int main(int argc, char** argv)
{
    int i;
    int n, nsteps;
    double* uloc;
    double* floc;
    double h;
    double tstart, tend, elapsed;
    char* fname;
    int rank, size;
    int ioffset, nper, nloc;

    /* Initialize MPI and get rank and size */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Process non-MPI arguments */
    n      = (argc > 1) ? atoi(argv[1]) : 100;
    nsteps = (argc > 2) ? atoi(argv[2]) : 100;
    fname  = (argc > 3) ? argv[3] : NULL;
    h      = 1.0/n;

    /* Print a diagnostic message */
    if (rank == 0)
        printf("Processes: %d\n", size);

    /* Split internal index range among processors */
    nper    = (n+size-1)/size;
    ioffset = rank*nper;
    nloc    = (rank == size-1) ? (n-ioffset) : nper+1;

    /* Allocate and initialize local arrays */
    uloc = (double*) malloc( (nloc+1) * sizeof(double) );
    floc = (double*) malloc( (nloc+1) * sizeof(double) );
    memset(uloc, 0, (nloc+1) * sizeof(double));
    for (i = 0; i <= nloc; ++i)
        floc[i] = (ioffset+i) * h;

    /* Run the solver */
    tstart = MPI_Wtime();
    jacobi(nsteps, nloc, uloc, floc, h*h, rank, size);
    tend = MPI_Wtime();
    elapsed = tend-tstart;

    /* Timing summary */
    if (rank == 0)
        printf("n: %d\n"
               "nsteps: %d\n"
               "Elapsed time: %g s\n", 
               n, nsteps, tend-tstart);

    /* Write the results */
    if (fname){
        //write_solution(n, nloc, uloc, fname, rank, size);
        writes(n, nsteps,elapsed, fname);
    }

    /* Clean everything up */
    free(floc);
    free(uloc);

    MPI_Finalize();
    return 0;
}