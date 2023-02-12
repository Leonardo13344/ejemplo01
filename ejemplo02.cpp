#include <iostream>
#include <mpi.h>
//#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>


//para utilizar la sintaxis 2000ms
using namespace std::chrono_literals;

int main(int argc, char** argv) {
   
   int rank;
   int size;
   int data[125];
   
   std::srand(std::time(nullptr));
   int tiempo = std::rand() % 30;
   
   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

    //sincronizar todos los procesos
    

   //Espera a que todos terminen
    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    //trabajo
    //tiempo aleatorio entre 1 y 30s
    
    std::chrono::milliseconds dura(tiempo * 1000);

    std::this_thread::sleep_for(dura);

    std::printf("Time for rank %d is %d\n", rank, tiempo);

    //volver a sincronizar
    MPI_Barrier(MPI_COMM_WORLD);
    double main_time = MPI_Wtime() - start_time;

    
   if (rank == 0) {
   std::printf("Time for work is %lf seconds\n", main_time);
   }

   MPI_Finalize();


   return 0;
}
