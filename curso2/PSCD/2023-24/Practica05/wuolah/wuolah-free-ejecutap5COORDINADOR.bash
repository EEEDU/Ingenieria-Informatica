#!/bin/bash
#Jesus Lizama, 816473
#Marcos Perez, 820532

#limpiamos todos los ejecutables que pudiera haber
make -f Makefile_p5_COORDINADOR clean  &> /dev/null
echo "clean realizado"

#compilamos el proyecto
make -f Makefile_p5_COORDINADOR  &> /dev/null
echo "proyecto compilado"

#indicamos el puerto por el que esucha el coordinador
puerto=3221

echo "./Coordinador" "$puerto"

#ejecutamos el servidor
./practica_5_COORDINADOR "$puerto"
