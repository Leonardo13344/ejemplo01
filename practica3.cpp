#include <iostream>
#include <omp.h>
#include <chrono>
#include <list>
#include <mpi.h>
#include <vector>
#include <cmath>


using namespace std::chrono_literals;

using namespace std;

//Conjetura de Goldbach

bool isPrimo(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}



int main(int argc, char** argv){
    int rank;
    int size;
    int num = 100;
    int pares[num / 2];


    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   //int parte_por_rank = (data.size() / 2 ) / 4;

    std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

    if(rank == 0)
    {
        std::vector<int> aux;
        for(int i = 2; i < num * 2; i+=2)
        {
            aux.push_back(i);
            if(aux.size() == num)
            {
                break;
            }
        }

        for(int i = 0; i < aux.size(); i++)
        {
            pares[i] = aux[i];
        }

        for(int i: pares)
        {
            std::printf("Pares %d \n", i);
        }

        //lo que hace cada rank
        //rank1 = 28 
        //rank2 = 78
        //rank3 = 
    
        MPI_Send(&pares[5], 15, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&pares[20], 15, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&pares[35], 15, MPI_INT, 3, 0, MPI_COMM_WORLD);
        

        for(int i = 2; i <= 10; i++)
        {
            bool encontre = false;
            for(int j = 2; j <= i/2; j++)
            {   
                if(isPrimo(j) && isPrimo(i-j))
                {
                    std::printf("%d + %d = %d \n", j, i-j, i);
                    encontre = true;
                    break;
                }
            }
        }
    }
    else
    {
        MPI_Recv(pares, 15, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::printf("Rank %d recibiendo desde %d hasta %d \n",rank, pares[0], pares[14]);

        for(int i = 0; i <= 14; i++)
        {
            bool encontre = false;
            for(int j = 2; j <= pares[i]/2; j++)
            {   
                if(isPrimo(j) && isPrimo(pares[i]-j))
                {
                    std::printf("%d + %d = %d \n", j, pares[i]-j, pares[i]);
                    encontre = true;
                    break;
                }
            }
        }
    }    

    MPI_Finalize();
    return 0;
}