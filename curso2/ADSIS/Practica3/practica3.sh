#!/bin/bash
#816106, Guerrero Serrano, Eduardo, T, 1, B

# Comprobar que el usuario tiene permisos de administrador
if [ "$(id -u)" -ne 0 ]; then
    echo "Este script necesita privilegios de administracion"
    exit 1
fi

# Comprobar numero de parametros
if [ "$#" -ne 2 ]; then
    echo "Numero incorrecto de parametros" >&2
    exit 1
fi

# Crear archivo de log
LOG="$(date +"%Y_%m_%d")_user_provisioning.log"
if [ ! -f $LOG ]; then
    touch $LOG
fi

# imprimir mensaje
print_log() {
    echo "$1"
    echo "$1" >> $LOG
}

# Comprobar que el ficher de usuarios existe
assert_exist_file() {
    if [ ! -f "$1" ]; then
        echo "El fichero de usuarios no existe" >&2
        exit 1
    fi
}

# Comprobar que los campos del archivo no estan vacios
assert_data_not_empty() {
    if [ -z "$1" ]; then
        echo "Campo invalido"
        return 1
    fi

    return 0
}

# Buscar el siguiente UID disponible mayor que 1815
get_next_uid() {
    local uid=1815
    while id -u $uid &>/dev/null; do
        uid=$((uid + 1))
    done
    echo $uid
}

# Anadir usuarios
if [ "$1" = "-a" ]; then
    assert_exist_file "$2"
    oldIFS=$IFS
    IFS=","
    while read -r user pass name
    do
        # Si alguno de los campos esta vacio, saltar la linea
        if ! assert_data_not_empty "$user" || ! assert_data_not_empty "$pass" || ! assert_data_not_empty "$name"; then
            continue
        fi
        # Si el usuario ya existe, saltar la ejecucion
        if id "$user" &>/dev/null; then
            print_log "El usuario $user ya existe"
            continue
        fi

        # Crear usuario
        uid=$(get_next_uid)
        groupadd "$user" 2>/dev/null
        useradd -m -s /bin/bash -u $uid -c "$name" -g "$user" "$user"
        echo "$user:$pass" | chpasswd
        chage -M 30 "$user"
        print_log "$user ha sido creado" # Segun la practica deberia ser $name, segun test $user
    done < "$2"
    IFS=$oldIFS
# Borrar usuarios
elif [ "$1" = "-s" ]; then
    assert_exist_file "$2"
    # Crear carpeta de backup si no existe
    if [ ! -d "/extra/backup" ]; then
        mkdir -p /extra/backup
    fi
    oldIFS=$IFS
    IFS=","
    while read -r user rest
    do
        # Si el campo esta vacio, saltar la linea
        if ! assert_data_not_empty "$user"; then
            continue
        fi
        # Si el usuario no existe, no hacer nada
        if ! id "$user" &>/dev/null; then
            continue
        fi
        
        tar -cf /extra/backup/${user}.tar -C / home/$user
        if [ $? -eq 0 ]; then
            userdel -r $user 2>&1 | grep -v "mail spool"  # Eliminar warning de mail spool
            if [ ${PIPESTATUS[0]} -eq 0 ]; then
                print_log "$user ha sido eliminado"
            else
                print_log "Error al eliminar el usuario $user"
            fi
        else
            print_log "Error al hacer el backup de $user"
        fi
    done < "$2"
    IFS=$oldIFS
# Error, $1 no es ni -a ni -s
else
    echo "Opción invalida" >&2
    exit 1
fi