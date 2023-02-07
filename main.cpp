#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
   
   int rank;
   int size;
   int data[125];

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

   if(rank == 0) {
      for(int i = 0; i < 125; i++){
         data[i] = i;
      }
      std::printf("RANK0 enviando .... \n");
      MPI_Send(&data[25], 25, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&data[50], 15, MPI_INT, 2, 0, MPI_COMM_WORLD);
      MPI_Send(&data[65], 60, MPI_INT, 3, 0, MPI_COMM_WORLD);

      int suma_parcial = 0;
      for(int i = 0; i < 25; i++){
         suma_parcial += data[i];
      }

      int suma_parcial1 = 0;
      MPI_Recv(&suma_parcial1, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      int suma_parcial2 = 0;
      MPI_Recv(&suma_parcial2, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      int suma_parcial3 = 0;
      MPI_Recv(&suma_parcial3, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


      std::printf("La suma parcial = %d suma parcial 1 = %d suma parcial 2 = %d suma parcial 3 = %d\n", suma_parcial,suma_parcial1, suma_parcial2, suma_parcial3);
      int suma_total = suma_parcial + suma_parcial1 + suma_parcial2 + suma_parcial3;
      
      std::printf("La suma total es: %d\n", suma_total);
   }
   else if(rank == 1){
      MPI_Recv(data, 25, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::printf("RANK%d recibiendo....datos[99]=%d\n", rank, data[0]);

      int suma_parcial = 0;
      for(int i = 0; i < 25; i++){
         suma_parcial += data[i];
      }
      std::printf("RANK%d devolviendo....suma= %d\n", rank, suma_parcial);
      MPI_Send( &suma_parcial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }
   else if(rank == 2){
      MPI_Recv(data, 15, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::printf("RANK%d recibiendo....datos[99]=%d\n", rank, data[0]);

      int suma_parcial = 0;
      for(int i = 0; i < 15; i++){
         suma_parcial += data[i];
      }

      std::printf("RANK%d devolviendo....suma= %d\n", rank, suma_parcial);
      MPI_Send( &suma_parcial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }
   else if(rank == 3){
      MPI_Recv(data, 60, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::printf("RANK%d recibiendo....datos[99]=%d\n", rank, data[0]);

      int suma_parcial = 0;
      for(int i = 0; i < 60; i++){
         suma_parcial += data[i];
      }

      std::printf("RANK%d devolviendo....suma= %d\n", rank, suma_parcial);
      MPI_Send( &suma_parcial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }
   

   MPI_Finalize();


   return 0;
}
