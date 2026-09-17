#!/bin/bash
#816106, Guerrero Serrano, Eduardo, T, 1, B

# Comprobar que el usuario tiene permisos de administrador
if [ "$(id -u)" -ne 0 ]; then
    echo "Este script necesita privilegios de administracion"
    exit 1
fi

# Comprobar numero de parametros
if [ "$#" -ne 3 ]; then
    echo "Numero incorrecto de parametros" >&2
    exit 1
fi

ACCION="$1"
FICHERO_USUARIOS="$2"
FICHERO_IPS="$3"
SSH_KEY="$HOME/.ssh/id_as_ed25519"
SSH_OPTS="-i $SSH_KEY -o StrictHostKeyChecking=no -o ConnectTimeout=5 -o BatchMode=yes"

# Crear archivo de log
LOG="$(date +"%Y_%m_%d")_user_provisioning.log"
if [ ! -f "$LOG" ]; then
    touch "$LOG"
fi

# imprimir mensaje
print_log() {
    echo "$1"
    echo "$1" >> "$LOG"
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

# Comprobar ficheros
assert_exist_file "$FICHERO_USUARIOS"
assert_exist_file "$FICHERO_IPS"

# Validar accion
if [ "$ACCION" != "-a" ] && [ "$ACCION" != "-s" ]; then
    echo "Opción invalida" >&2
    exit 1
fi

# Leer cada IP del fichero
while read -r IP; do
    # Ignorar líneas vacías
    [ -z "$IP" ] && continue

    # Comprobar si la máquina es accesible
    ssh $SSH_OPTS as@"$IP" "exit" 2>/dev/null
    if [ $? -ne 0 ]; then
        print_log "$IP no es accesible"
        continue
    fi

    # Leer fichero de usuarios y actuar en remoto
    oldIFS=$IFS
    IFS=","
    while read -r user pass name; do
        # Comprobar campos vacíos
        if ! assert_data_not_empty "$user" || ! assert_data_not_empty "$pass" || ! assert_data_not_empty "$name"; then
            continue
        fi

        if [ "$ACCION" = "-a" ]; then
            # Crear usuario en remoto
            RESULT=$(ssh $SSH_OPTS as@"$IP" "
                sudo bash -c '
                    if id \"$user\" &>/dev/null; then
                        echo \"El usuario $user ya existe\"
                    else
                        uid=1815
                        while id -u \$uid &>/dev/null; do uid=\$((uid + 1)); done
                        groupadd \"$user\" 2>/dev/null
                        useradd -m -s /bin/bash -u \$uid -c \"$name\" -g \"$user\" \"$user\"
                        echo \"$user:$pass\" | chpasswd
                        chage -M 30 \"$user\"
                        echo \"$user ha sido creado\"
                    fi
                '
            " 2>/dev/null)
            print_log "[$IP] $RESULT"

        elif [ "$ACCION" = "-s" ]; then
            # Eliminar usuario en remoto
            RESULT=$(ssh $SSH_OPTS as@"$IP" "
                sudo bash -c '
                    if ! id \"$user\" &>/dev/null; then
                        exit 0
                    fi
                    mkdir -p /extra/backup
                    tar -cf /extra/backup/${user}.tar -C / home/$user
                    if [ \$? -eq 0 ]; then
                        userdel -r \"$user\" 2>&1 | grep -v \"mail spool\"
                        echo \"$user ha sido eliminado\"
                    else
                        echo \"Error al hacer el backup de $user\"
                    fi
                '
            " 2>/dev/null)
            print_log "[$IP] $RESULT"
        fi

    done < "$FICHERO_USUARIOS"
    IFS=$oldIFS

done < "$FICHERO_IPS"