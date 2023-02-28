#include <iostream>
#include <omp.h>
#include <chrono>
#include <list>
#include <mpi.h>
#include <vector>
#include<stdlib.h>
#include <ctime>

using namespace std::chrono_literals;

using namespace std;

double distancia_euclidiana(const std::vector<double>& a, const std::vector<double>& b){
    double sum = 0.0;
    for (size_t i = 0; i < 3; i++) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(sum);
}

int encuentra_indice(const std::vector<double>& a, const std::vector<std::vector<double>>& c){
    double min_distance = distancia_euclidiana(a, c[0]);
    int indice_min = 0;
    for (size_t i = 1; i < c.size(); i++) {
        double distance = distancia_euclidiana(a, c[i]);
        if (distance < min_distance) {
            min_distance = distance;
            indice_min = i;
        }
    }
    return indice_min;
}

int main(int argc, char** argv){
    int rank;
    int size;

    int maxA = 30;
    int maxC = 3;

    std::vector<std::vector<double>> A;
    std::vector<std::vector<double>> C;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

    if(rank == 0)
    {
        std::srand(std::time(nullptr));
        for(int i = 0; i < maxA; i++){
            double aux1 = std::rand() % 10 + 1;
            double aux2 = std::rand() % 10 + 1;
            double aux3 = std::rand() % 10 + 1;
            A.push_back({aux1, aux2, aux3});
        }

        for(int i = 0; i < maxC; i++){
            double aux1 = std::rand() % 10 + 1;
            double aux2 = std::rand() % 10 + 1;
            double aux3 = std::rand() % 10 + 1;
            C.push_back({aux1, aux2, aux3});
        }

        //Mando los tamaños 
        MPI_Send(&maxA, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&maxC, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        //Mando con quien tienen que comparar
        MPI_Send(&C, maxC, MPI_INT, 1, 0, MPI_COMM_WORLD);

        //Particiono y Mando los datos
        MPI_Send(&A[6], 8, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&A[14], 8, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&A[22], 8, MPI_INT, 3, 0, MPI_COMM_WORLD);

        //Trabajo mis datos
        std::vector<int> indice_min
        for(int i = 0; i < 6; i++)
        {
            int aux = encuentra_indice(A[i], C);
            indice_min.push_back(aux);
        }

        std::vector<int> parcial;
        for(int i = 0; i < 8; i++){
            int aux;
            MPI_Recv(&aux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            indice_min.push_back(aux);
        }

        //Impresion



    }else
    {
        int reciboTamanioA;
        MPI_Recv(&reciboTamanio, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int reciboTamanioC;
        MPI_Recv(&reciboTamanioC, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::vector<std::vector<double>> reciboA;
        for(int i = 0; i < reciboTamanioA; i++)
        {
            std::vector<double> aux;
            MPI_Recv(&aux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reciboA.push_back(aux);
        }

        std::vector<std::vector<double>> reciboC;
        for(int i = 0; i < reciboTamanioC; i++)
        {
            std::vector<double> aux;
            MPI_Recv(&aux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reciboC.push_back(aux);
        }

        std::vector<int> indice_min;
        for(int i = 0; i < 8; i++)
        {
            int aux;
            aux = encuentra_indice(A[i], C);
            indice_min.push_back(aux);
        }

        //devolucion al rank0
        MPI_Send(&indice_min[0], indice_min.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}