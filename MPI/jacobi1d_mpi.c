/* Include benchmark-specific header */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


double bench_t_start, bench_t_end;

static double rtclock()
{
    struct timeval Tp; 
	/* Structure including fields:
       long tv_sec - seconds since Jan. 1, 1970 
       long tv_usec - microseconds */
    int stat;
    stat = gettimeofday (&Tp, NULL); /* Get current time and data in seconds and mircroseconds (fill fields of struct Tp) */
    if (stat != 0) /* 0 - succcess, (-1) - failure */
        printf ("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void bench_timer_start()
{
    bench_t_start = rtclock (); /* Remember time of start */
}

void bench_timer_stop()
{
    bench_t_end = rtclock (); /* Remember time of end */
}

void bench_timer_print()
{
    printf ("Time in seconds = %0.6lf\n", bench_t_end - bench_t_start); /* Print time */
}

int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    int tsteps = atoi(argv[2]);
    double *A; 
    double *B;
    int i,it;
    int ll,shift;
    
    MPI_Request req[4];
    int myrank, ranksize;
    int startelem, lastelem, nelem;
    
    MPI_Status status[4];
    bench_timer_start();
    
    MPI_Init (&argc, &argv); /* Initialize processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);/* Rank of the process */
    MPI_Comm_size (MPI_COMM_WORLD, &ranksize); /* Number of processes */
    MPI_Barrier(MPI_COMM_WORLD);

    startelem = (myrank * n) / ranksize;
    lastelem = (((myrank + 1) * n) / ranksize)-1;
    nelem = lastelem - startelem + 1;
    
   	/* Initialize arrays */
    A = (double(*))malloc((nelem + 2) * sizeof(double));
    B = (double(*))malloc((nelem + 2) * sizeof(double));
    int correction = (myrank == 0) ? 1 : 0; 
    for (i = 0; i < nelem + 2; i++) {
        A[i] = ((double) i + startelem + 2 - correction) / n;
        B[i] = ((double) i + startelem + 3 - correction) / n;
    }
    
    /* Iteration loop */
    for(it = 0; it < tsteps; it++)
    {   
        if(myrank != 0)
            MPI_Irecv(&A[0], 1, MPI_DOUBLE, myrank - 1, 1215, MPI_COMM_WORLD, &req[0]);
        if(myrank != ranksize - 1)
            MPI_Isend(&A[nelem], 1, MPI_DOUBLE, myrank + 1, 1215, MPI_COMM_WORLD,&req[2]);
        if(myrank != ranksize - 1)
            MPI_Irecv(&A[nelem + 1], 1, MPI_DOUBLE, myrank + 1, 1216, MPI_COMM_WORLD, &req[3]);
        if(myrank != 0)
            MPI_Isend(&A[1], 1, MPI_DOUBLE, myrank - 1, 1216, MPI_COMM_WORLD, &req[1]);
        ll = 4; 
        shift = 0;
        if (myrank == 0) 
        {
            ll = 2;
            shift = 2;
        }
        if (myrank == ranksize - 1) 
        {
            ll = 2;
        }
        MPI_Waitall(ll, &req[shift], &status[0]);

        for(i = 1; i <= nelem; i++) 
        {
            if (((i == 1) && (myrank == 0)) || ((i == nelem) && (myrank == ranksize - 1))) 
                continue;
            B[i] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
        }

        if(myrank != 0)
            MPI_Irecv(&B[0], 1, MPI_DOUBLE, myrank - 1, 1215, MPI_COMM_WORLD, &req[0]);
        if(myrank != ranksize - 1)
            MPI_Isend(&B[nelem], 1, MPI_DOUBLE, myrank + 1, 1215, MPI_COMM_WORLD,&req[2]);
        if(myrank != ranksize - 1)
            MPI_Irecv(&B[nelem + 1], 1, MPI_DOUBLE, myrank + 1, 1216, MPI_COMM_WORLD, &req[3]);
        if(myrank != 0)
            MPI_Isend(&B[1], 1, MPI_DOUBLE, myrank - 1, 1216, MPI_COMM_WORLD, &req[1]);
        ll = 4; 
        shift = 0;
        if (myrank == 0) 
        {
            ll = 2;
            shift = 2;
        }
        if (myrank == ranksize - 1) 
        {
            ll = 2;
        }
        MPI_Waitall(ll, &req[shift], &status[0]);

        for(i = 1; i <= nelem; i++) 
        {
            if (((i == 1) && (myrank == 0)) || ((i == nelem) && (myrank == ranksize - 1))) 
                continue;
            A[i] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
        }
    }
    /* End of iteration loop */
    
    free(A);
    free(B);
    
    bench_timer_stop();
    double sum = bench_t_end - bench_t_start;
    
    if (myrank == ranksize - 1)
        printf("%d,%d,%d,%f\n", ranksize, n, tsteps, sum);
    
    MPI_Finalize(); 

    return 0;
}
