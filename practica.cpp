#include <iostream>
#include <omp.h>
#include <chrono>
#include <list>
#include <mpi.h>

using namespace std::chrono_literals;

using namespace std;

bool isPerfectNumber(int num){
    int sum = 0;
    for(int i = 1; i <= num / 2; i++){
        if(num % i == 0){
            sum += i;
        }
    }
    return sum == num;
}

int main(int argc, char** argv){
    int rank;
    int size;
    int data[100];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

    if(rank == 0)
    {
        for(int i = 0; i < 100; i++)
        {
            data[i] = i;
        }
        
        for(int i = 1; i < size; i++)
        {
            MPI_Send(&data[25 * i], 25, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        std::list<int> num_parciales;
        for(int i = 0; i < 25; i++)
        {
            if(isPerfectNumber(i))
            {
                //::printf("Los numeros perfectos son: %d ", i);
                num_parciales.push_back(i);
            }
        }

        for(int i = 1; i < size; i++)
        {
            int encontre;
            MPI_Recv(&encontre, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int j = 0; j < encontre; j++)
            {
                int numero_perfecto;
                MPI_Recv(&numero_perfecto, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                num_parciales.push_back(numero_perfecto);
            }
        }

        for (auto it = num_parciales.begin(); it != num_parciales.end(); ++it)
        {
            std::cout << *it << " ";
        }
    }
    else
    {
        MPI_Recv(data, 25, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::printf("RANK %d recibiendo....desde %d hasta %d \n", rank, data[0], data[24]);
        
        //int encontre = 0;
        //int parcial[25];

        std::list<int> parcial;
        for(int i = 0; i < 25; i++)
        {
            if(isPerfectNumber(data[i]))
            {
                parcial.push_back(data[i]);
            }
        }
        int encontre = parcial.size();
        std::printf("RANK%d devolviendo....\n", rank);
        MPI_Send(&encontre, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for(int i: parcial)
        {
            MPI_Send(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}