#!/bin/bash

for i in {1..100}; do
    echo "Ejecución número $i"
    python3 generator.py
    ./output/test salida_grande.txt
    echo "----------------------"
done
