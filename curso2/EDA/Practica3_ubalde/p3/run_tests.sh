#!/bin/bash

# Script para ejecutar casos de prueba y guardar sus salidas

# Directorio base del proyecto (asume que el script se ejecuta desde la raíz)
BASE_DIR=$(pwd)
CASOS_DIR="$BASE_DIR/tests/casos"
SALIDAS_DIR="$BASE_DIR/tests/salidas"
ENTRADA_FILE="$BASE_DIR/entrada.txt"
EJECUTABLE="$BASE_DIR/output/test"

# 1. Compilar el proyecto
echo "--- Compilando el proyecto ---"
# Usamos la tarea de compilación de VSCode si está disponible, si no, un g++ genérico
if [ -f "$BASE_DIR/.vscode/tasks.json" ]; then
    # Intenta encontrar el comando de compilación en tasks.json
    # Esto es una simplificación, puede no funcionar con todas las configuraciones
    g++ -std=c++11 -g *.cpp -o "$EJECUTABLE"
else
    g++ -std=c++11 -g *.cpp -o "$EJECUTABLE"
fi

if [ $? -ne 0 ]; then
    echo "Error de compilación. Abortando pruebas."
    exit 1
fi
echo "Compilación exitosa."
echo ""

# 2. Crear directorio de salidas si no existe
mkdir -p "$SALIDAS_DIR"

# 3. Iterar sobre todos los ficheros de prueba en el directorio de casos
for caso_path in "$CASOS_DIR"/test_*.txt; do
    # Extraer el nombre del fichero del caso de prueba
    nombre_caso=$(basename "$caso_path")
    nombre_salida="salida_${nombre_caso}"
    salida_path="$SALIDAS_DIR/$nombre_salida"

    echo "--- Ejecutando prueba: $nombre_caso ---"

    # Copiar el contenido del caso de prueba al fichero de entrada principal
    cp "$caso_path" "$ENTRADA_FILE"
    if [ $? -ne 0 ]; then
        echo "Error: No se pudo copiar '$caso_path' a '$ENTRADA_FILE'."
        continue # Saltar a la siguiente prueba
    fi

    # Ejecutar el programa y redirigir la salida al fichero correspondiente
    "$EJECUTABLE" "$salida_path"
    if [ $? -ne 0 ]; then
        echo "Error durante la ejecución de '$nombre_caso'. La salida puede estar incompleta."
    else
        echo "Prueba completada. Salida guardada en: $salida_path"
    fi
    echo ""
done

# Limpieza final
rm -f "$ENTRADA_FILE"
echo "--- Todas las pruebas han finalizado. ---"
