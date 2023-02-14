# ejemplo 01

## Instalación MPI en Ubuntu


sudo apt-get update
sudo apt-get install libopenmpi-dev


## Verificar Instalación

mpiexec --version

## Compilación 

mpicxx main.cpp -o ejemplo01

## Ejecutar 

mpiexec -n 4 ./ejemplo01