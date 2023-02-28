
#include <iostream>
#include <omp.h>
#include <cmath>
#include <chrono>

namespace ch = std::chrono;

double funcion(double x){
    double f= 4/(1+pow(x,2));
    return f;
}

double trapecioSerial(double a, double b,int n){
    double h= (b-a) / n;
    double fa=funcion(a)/2;
    double fb=funcion(b)/2;
    double suma=0.0;
    for(int i=1;i<n;i++){
        suma=suma+funcion(a+i*h);
    }
    suma=(h)*(fa+suma+fb);
    return suma;
}

double trapecioParallel(double a, double b, int n) {
    double h = (b-a) / n;
    double fa=funcion(a)/2;
    double fb=funcion(b)/2;
    double suma=0.0;
#pragma omp parallel shared(suma)
    {
#pragma omp for
        for(int i=1;i<n;i++){
            //printf("TEs\n");
            suma=suma+funcion(a+i*h);
        }
    }
    suma=(h)*(fa+suma+fb);
    return suma;
}

int main(){
    double a = 0;
    double b = 1;
    int n= 100;

    double resultado;
    auto start = ch::high_resolution_clock::now();
    {
        resultado = trapecioSerial(a,b,n);
    }
    auto end = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> duracion = end - start;

    std::printf("Tiempo serial: %lf segundos, resultado=%lf\n", duracion.count() * 0.001, resultado);

    double resultado2;
    auto start1 = ch::high_resolution_clock::now();
    {
        resultado2 = trapecioParallel(a,b,n);
    }
    auto end1 = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> duracion1 = end1 - start1;

    std::printf("Tiempo paralelo: %lf segundos, resultado=%lf\n", duracion1.count() * 0.001, resultado2);

    return 0;
}