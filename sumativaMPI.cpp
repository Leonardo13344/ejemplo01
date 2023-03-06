#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <map>
#include<fstream>


namespace ch = std::chrono;

void remove(std::vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }
    v.erase(end, v.end());
}


std::vector<int> conteoSerial(std::vector<int> &parte, std::vector<int> &sinRep){
    std::sort(parte.begin(), parte.end()); //ordenamos
    std::vector<int> auxV = parte; //copia
    std::vector<int> rep(sinRep.size());
    //Contamos cuantas veces se repiten los valores de A en la copia
    for(int i = 0; i < sinRep.size(); i++){
        int aux = 0;
        for(int j = 0; j < auxV.size(); j++){
            if(sinRep[i] == auxV[j]){
                aux++;
            }
        }
        rep[i] = aux;
    }
    return rep;
}


int main(int argc, char** argv){
    int rank;
    int size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    

    if(rank == 0){
        std::vector<int> datos = {60,38,39,12,41,38,54,53,23,21,61,75,74,63,66,32,33,3,36,15,44,52,18,41,45,
        51,14,35,72,17,21,43,49,10,40,6,50,72,24,61,86,24,85,63,59,31,20,30,2,25,
        25,8,27,27,5,2,51,37,25,15,12,8,0,62,35,42,76,39,35,16,14,4,41,30,6,
        0,37,54,51,49,44,8,64,61,44,40,10,71,21,4,42,48,72,40,22,12,24,22,55,59};
        std::vector<int> aux(datos.size());
        aux = datos;
        remove(aux);
        std::vector<int> sinRep = aux;
        printf("A %ld \n", datos.size());
        printf("B %ld \n", sinRep.size());

        //Enviamos datos
        MPI_Send( &datos[25], 25, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send( &datos[50], 25, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send( &datos[75], 25, MPI_INT, 3, 0, MPI_COMM_WORLD);

        //Envio datos sin repetir
        int tamanio = sinRep.size();
        MPI_Bcast(&tamanio, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(sinRep.data(), tamanio, MPI_INT, 0, MPI_COMM_WORLD);
        
        //Proceso lo mio
        std::vector<int> miParte(25);
        for(int i = 0; i<25; i++){
            miParte[i] = datos[i];
        }

        std::vector<int> repTMP0(tamanio);
        repTMP0 = conteoSerial(miParte, sinRep);

        std::vector<int> repTMP1(tamanio);
        std::vector<int> repTMP2(tamanio);
        std::vector<int> repTMP3(tamanio);

        MPI_Recv(repTMP1.data(), tamanio, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(repTMP2.data(), tamanio, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(repTMP3.data(), tamanio, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::vector<int> repTOTAL;
        repTOTAL.insert(repTOTAL.end(), repTMP0.begin(), repTMP0.end());
        repTOTAL.insert(repTOTAL.end(), repTMP1.begin(), repTMP1.end());
        repTOTAL.insert(repTOTAL.end(), repTMP2.begin(), repTMP2.end());
        repTOTAL.insert(repTOTAL.end(), repTMP3.begin(), repTMP3.end());

        for(int i = 0; i < repTOTAL.size(); i++){
            printf("El %d se repite %d veces \n", datos[i], repTOTAL[i]);
        }
        
    }else{
        //Recibo los datos del rank 0
        std::vector<int> datos(25);
        MPI_Recv(datos.data(), 25, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("El rank %d tiene desde %d hasta %d \n", rank, datos[0], datos[24]);
        int tamanio;
        MPI_Bcast(&tamanio, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Tamanio recibido %d \n", tamanio);
        std::vector<int> sinRepPart(tamanio);
        MPI_Bcast(sinRepPart.data(), sinRepPart.size(), MPI_INT, 0, MPI_COMM_WORLD);

        //Calculo mi parte
        std::vector<int> repTMP(tamanio);
        repTMP = conteoSerial(datos, sinRepPart);

        //envio los repetidos temporales
        MPI_Send(repTMP.data(), repTMP.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}