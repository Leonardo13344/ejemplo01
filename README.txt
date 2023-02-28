# ejemplo 01

## Instalación MPI en Ubuntu


sudo apt-get update
sudo apt-get install libopenmpi-dev


## Verificar Instalación

mpiexec --version

## Compilación MPI

mpicxx main.cpp -o ejemplo01

## Ejecutar MPI

mpiexec -n 4 ./ejemplo01

##Instalacion OMP en ubuntu 

sudo apt-get update
sudo apt-get install -y libomp-dev

##Compilar 

g++ -fopenmp archivo.cpp -o archivo

##Ejecutar
./archivo