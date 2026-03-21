#!/bin/bash
#816106, Guerrero Serrano, Eduardo, T, 1, B

# Comprobar que el usuario tiene permisos de administrador
if [ "$(id -u)" -ne 0 ]; then
    echo "Este script necesita privilegios de administracion"
    exit 1
fi

# Comprobar numero de parametros
if [ "$#" -ne 2 ]; then
    echo "Numero incorrecto de parametros"
    exit 1
fi

# Comprobar que el ficher de usuarios existe
if [ ! -f "$2" ]; then
    echo "El fichero de usuarios no existe"
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

# Comprobar que los campos del archivo no estan vacios
assert_data_not_empty() {
    if [ -z "$1" ]; then
        echo "Campo invalido"
        exit 1
    fi

    return 0
}

# Buscar el siguiente UID disponible mayor que 1815
get_next_uid() {
    local uid=1816
    while id -u $uid &>/dev/null; do
        uid=$((uid + 1))
    done
    echo $uid
}

# Anadir usuarios
if [ "$1" = "-a" ]; then
    oldIFS=$IFS
    IFS=","
    while read -r user pass name
    do
        assert_data_not_empty "$user"
        assert_data_not_empty "$pass"
        assert_data_not_empty "$name"
        if id "$user" &>/dev/null; then
            print_log "El usuario $user ya existe"
        else
            uid=$(get_next_uid)
            expiracion=$(date -d "+30 days" +"%Y-%m-%d")
            groupadd $name 2>/dev/null
            useradd -m -s /bin/bash -u $uid -e $expiracion -c $name -g $name $user
            echo "$user:$pass" | chpasswd
            print_log "$name ha sido creado"
        fi
    done < "$2"
    IFS=$oldIFS
# Borrar usuarios
elif [ "$1" = "-s" ]; then
    if [ ! -d "/extra/backup" ]; then
        mkdir -p /extra/backup
    fi
    while read -r user
    do
        assert_data_not_empty "$user"
        if id "$user" &>/dev/null; then
            tar -cf /extra/backup/${user}.tar -C / home/$user
            if [ $? -eq 0 ]; then
                userdel -r $user 2>&1 | grep -v "mail spool"
                if [ ${PIPESTATUS[0]} -eq 0 ]; then
                    print_log "$user ha sido eliminado"
                else
                    print_log "Error al eliminar el usuario $user"
                fi
            else
                print_log "Error al hacer el backup de $user"
            fi
        fi
    done < "$2"
# Error, $1 no es ni -a ni -s
else
    echo "Opción inválida" >&2
    exit 1
fi