#include <iostream>
#include <chrono>
#include <cmath>
#include <mpi.h>

using namespace std;

double funcion(double x){
   double f= 4/(1+pow(x,2));
   return f;
}

double trapecioMPI(double a, double b, int n){

    double h = (b-a) / n;
    double fa = funcion(a)/2;
    double fb = funcion(b)/2;
    int rank;
    int size;
    int* data = new int [n];

    MPI_Init(NULL,NULL);
   
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int div = n/size;

    if(rank==0){
       
        for(int i = 0 ; i < n ; i++){
            data[i] = i + 1;
        }
        int part=div-1;
        for(int i=1;i<4;i++){
            MPI_Send(&data[part],div,MPI_INT,i,0,MPI_COMM_WORLD);
            part+=div;
        }

        double suma=0.0;
         
        for(int i=0;i<div-1;i++){
             suma=suma+funcion(a+data[i]*h);     
        }
      
        double sumaTotal=0.0;
        for(int i=1;i<4;i++){
            double sumapar=0.0;
            MPI_Recv(&sumapar,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            sumaTotal=sumapar+sumaTotal;
        }

       double total=(h)*(fa+suma+sumaTotal+fb);

       return total;

    }else{
        MPI_Recv(data, div, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        double suma = 0.0;
        for(int i = 0 ; i < div ; i++){
            
            suma = suma + funcion(a + data[i] * h);
          
        }
        MPI_Send(&suma,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }

    return 0.0;
}

int main(int argc, char** argv){
    double a = 0;
    double b = 1;
    int n = 10000000;
    auto start = std::chrono::high_resolution_clock::now();
    double resultado = trapecioMPI(a, b, n);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> tiempo_total = end - start;
    if (MPI::COMM_WORLD.Get_rank() == 0) {
        printf("Tiempo total: %f s, resultado=%f\n", tiempo_total.count()/1000.0, resultado);
    }
    MPI_Finalize(); 
    return 0;
}