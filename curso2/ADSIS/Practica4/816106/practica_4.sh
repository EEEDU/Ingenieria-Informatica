#!/bin/bash
#816106, Guerrero Serrano, Eduardo, T, 1, B

# Comprobar numero de parametros
if [ "$#" -ne 3 ]; then
    echo "Numero incorrecto de parametros" >&2
    exit 1
fi

ACCION="$1"
FICHERO_USUARIOS="$2"
FICHERO_IPS="$3"
SSH_KEY="/home/as/.ssh/id_as_ed25519"
SSH_OPTS=(-i "$SSH_KEY" -o StrictHostKeyChecking=no -o ConnectTimeout=5 -o BatchMode=yes -o SendEnv=U_USER,U_PASS,U_NAME)

# Crear archivo de log local
LOG="/home/as/$(date +"%Y_%m_%d")_user_provisioning.log"
touch "$LOG"

# Imprimir mensaje en pantalla y log local
print_log() {
    echo "$1"
    echo "$1" >> "$LOG"
}

# Validar accion
if [ "$ACCION" != "-a" ] && [ "$ACCION" != "-s" ]; then
    echo "Opción invalida" >&2
    exit 1
fi

# Comprobar ficheros
if [ ! -f "$FICHERO_USUARIOS" ]; then
    echo "El fichero de usuarios no existe" >&2
    exit 1
fi
if [ ! -f "$FICHERO_IPS" ]; then
    echo "El fichero de IPs no existe" >&2
    exit 1
fi

# Script remoto de añadir usuario
remote_add() {
    local ip="$1" user="$2" pass="$3" name="$4"
    ssh "${SSH_OPTS[@]}" as@"$ip" "sudo bash -s" << ENDSSH
if id "$user" &>/dev/null; then
    echo "El usuario $user ya existe"
else
    uid=1815
    while id -u \$uid &>/dev/null; do uid=\$((uid + 1)); done
    groupadd "$user" 2>/dev/null
    useradd -m -s /bin/bash -u \$uid -c "$name" -g "$user" "$user"
    echo "$user:$pass" | chpasswd
    chage -M 30 "$user"
    echo "$user ha sido creado"
fi
ENDSSH
}

# Script remoto de borrar usuario
remote_del() {
    local ip="$1" user="$2"
    ssh "${SSH_OPTS[@]}" as@"$ip" "sudo bash -s" << ENDSSH
if ! id "$user" &>/dev/null; then
    exit 0
fi
mkdir -p /extra/backup
tar -cf /extra/backup/${user}.tar -C / home/$user
if [ \$? -eq 0 ]; then
    userdel -r "$user" 2>&1 | grep -v "mail spool"
    echo "$user ha sido eliminado"
else
    echo "Error al hacer el backup de $user"
fi
ENDSSH
}

# Para cada IP
while read -r -u3 IP; do
    [ -z "$IP" ] && continue

    # Comprobar si la máquina es accesible
    ssh "${SSH_OPTS[@]}" as@"$IP" "exit" 2>/dev/null
    if [ $? -ne 0 ]; then
        print_log "$IP no es accesible"
        continue
    fi

    oldIFS=$IFS
    IFS=","
    while read -r -u4 user pass name; do
        if [ -z "$user" ]; then continue; fi

        if [ "$ACCION" = "-a" ]; then
            if [ -z "$pass" ] || [ -z "$name" ]; then
                echo "Campo invalido"
                continue
            fi
            RESULT=$(remote_add "$IP" "$user" "$pass" "$name" 2>/dev/null)
            print_log "[$IP] $RESULT"

        elif [ "$ACCION" = "-s" ]; then
            RESULT=$(remote_del "$IP" "$user" 2>/dev/null)
            [ -n "$RESULT" ] && print_log "[$IP] $RESULT"
        fi

    done 4< "$FICHERO_USUARIOS"
    IFS=$oldIFS

done 3< "$FICHERO_IPS"