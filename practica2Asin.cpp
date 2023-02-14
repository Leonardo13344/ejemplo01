#include <iostream>
#include <omp.h>
#include <chrono>
#include <list>
#include <mpi.h>
#include <vector>

using namespace std::chrono_literals;

using namespace std;

//[5,6,4,8,10,22,54,23,1]

std::list<int> bubbleSort(const std::list<int>& myList) {
    std::list<int> sortedList = myList;
    int n = sortedList.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            auto it1 = std::next(sortedList.begin(), j);
            auto it2 = std::next(sortedList.begin(), j + 1);
            if (*it1 > *it2) {
                std::swap(*it1, *it2);
            }
        }
    }

    return sortedList;
}

int main(int argc, char** argv){
    int rank;
    int size;
    std::vector<int> data {8,23,19,67,45,35,1,24,13,30,3,5};

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   

    std::printf("Hola, soy el rank %d, de un total de %d procesos\n", rank, size);

    if(rank == 0)
    {
        MPI_Request request;

        std::list<int> data_parcial;
        for(int i = 0; i < data.size() / 2; i++)
        {
            data_parcial.push_back(data[i]);
            std::printf("datos en lista 1 %d \n", data[i]);

        }   

        std::list<int> data_parcial1;
        for(int i = data.size() - 1; i > (data.size() / 2) - 1; i--)
        {
            data_parcial1.push_back(data[i]);
            std::printf("datos en lista 2 %d \n", data[i]);
        }

        

        //envio a ranks
        int tamanioList2 = data_parcial1.size();
        MPI_Isend(&tamanioList2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        
        for(int i: data_parcial1)
        {
            std::printf("Estoy enviando a Rank 1 los siguientes elementos: %d \n", i);
            MPI_Isend(&i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
            
        }

        //Lo que hace el rank0
        std::list<int> parcial_sortedList = bubbleSort(data_parcial);

        //Recibo de ranks
        std::list<int> parcial1_sortedList;
        int sizeSortedList;
        
        MPI_Irecv(&sizeSortedList, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        //elementos ordenados
        for(int i= 0; i < sizeSortedList; i++){
            int element;
            MPI_Irecv(&element, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, MPI_STATUS_IGNORE);
            parcial1_sortedList.push_back(element);
        }

        

        for(int i: parcial_sortedList)
        {
            std::printf("Parcial Sorted List  %d \n", i);
        }

        for(int i: parcial1_sortedList){
            std::printf("Parcial Sorted List 1 %d \n", i);
            parcial_sortedList.push_back(i);
        }

        

        std::list<int> sortedList = bubbleSort(parcial_sortedList);

        for (auto it = sortedList.begin(); it != sortedList.end(); ++it)
        {
            std::cout << *it << " ";
        }
    }
    else if( rank == 1)
    {
        std::printf("Entrando al rank %d \n", rank);
        MPI_Request request;
        std::list<int> listaRecibida;
        int listSize;
        
        MPI_Irecv(&listSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        for(int i = 0; i < listSize; i++)
        {
            int elemento;
            MPI_Irecv(&elemento, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, MPI_STATUS_IGNORE);
            std::printf("Elemento %d que va a devolver de Rank %d \n", elemento, rank);
            listaRecibida.push_back(elemento);
        }
        std::list<int> sortedList = bubbleSort(listaRecibida);
        int tamanioListSorted = sortedList.size();
        MPI_Isend(&tamanioListSorted, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        for(int i: sortedList)
        {
            MPI_Isend(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        }
    }
    MPI_Finalize();
    return 0;
}