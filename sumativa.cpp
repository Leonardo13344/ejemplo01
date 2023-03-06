#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>
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

void conteoSerial(std::vector<int> &A){
    std::sort(A.begin(), A.end()); //ordenamos
    std::vector<int> conDup = A; //copia
    remove(A);//quitamos duplicados
    std::vector<int> repeticiones(A.size());
    //Contamos cuantas veces se repiten los valores de A en la copia
    for(int i = 0; i < A.size(); i++){
        int aux = 0;
        for(int j = 0; j < conDup.size(); j++){
            if(A[i] == conDup[j]){
                aux++;
            }
        }
        repeticiones[i] = aux;
    }
    //Imprimimos
    for(int i = 0; i<repeticiones.size(); i++){
        printf("El %d se repite %d veces\n", A[i], repeticiones[i]);
    }
}

void conteoParalelo(std::vector<int> &A){
    std::sort(A.begin(), A.end());
    std::vector<int> auxA = A;
    remove(A);
    std::vector<int> rep;
#pragma omp parallel shared(rep)
{
    #pragma omp for
    for(int i = 0; i < A.size(); i++){
        int aux = 0;
        for(int j = 0; j < auxA.size(); j++){
            if(A[i] == auxA[j]){
                aux++;
            }
        }
        rep.push_back(aux);
    }
}

    for(int i = 0; i<rep.size(); i++){
        printf("El %d se repite %d veces\n", A[i], rep[i]);
    }
    
}



int main(){

    std::vector<int> A = {60,38,39,12,41,38,54,53,23,21,61,75,74,63,66,32,33,3,36,15,44,52,18,41,45,51,14,35,72,17,21,43,49,10,40,6,50,72,24,61,86,24,85,63,59,31,20,30,2,25,25,8,27,27,5,2,51,37,25,15,12,8,0,62,35,42,76,39,35,16,14,4,41,30,6,0,37,54,51,49,44,8,64,61,44,40,10,71,21,4,42,48,72,40,22,12,24,22,55,59};
    
    std::vector<int> datos;
    std::ifstream is("datos.txt");
    std::string tp;
    while(std::getline(is,tp)){
        int valor = std::stoi(tp);
        datos.push_back(valor);
    }

    auto start = ch::high_resolution_clock::now();{
        //conteoSerial(datos);
    }
    auto end = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> duracion = end - start;
    std::printf("Tiempo serial: %lf segundos\n", duracion.count() * 0.001);
    printf("\n");

    auto start2 = ch::high_resolution_clock::now();{
        conteoParalelo(datos);
    }
    auto end2 = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> duracion2 = end2 - start2;
    std::printf("Tiempo paralelo: %lf segundos\n", duracion2.count() * 0.001);
    

    return 0;
}