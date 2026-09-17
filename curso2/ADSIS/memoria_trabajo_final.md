# 816106, Guerrero Serrano, Eduardo, T, 1, B

---

## Introducción

El objetivo del trabajo es la instalación y configuración del sistema de una infraestructura de 6 maquinas Debian, divididas en 3 redes internas, con una de estas maquinas actuando como firewall, router y servidor.

### Maquinas virtuales utilizados

| Máquina | Rol | Redes |
|---|---|---|
| debian1 | Firewall + Router + DHCP | NAT, Host-only, Red Interna 1, Red Interna 2 |
| debian2 | Servidor Web (nginx) | Red Interna 1 |
| debian3 | Genérica (DHCP) | Red Interna 2 |
| debian4 | Genérica (DHCP) | Red Interna 2 |
| debian5 | Servidor SSH | Red Interna 3 |
| debian6 | Pasarela | Red Interna 2, Red Interna 3 |

---

## Decisiones de administrador

### Rangos de red elegidos

| Red | Subred | Propósito |
|---|---|---|
| Host-only | 192.168.104.0/24 | Comunicación Host ↔ debian1 |
| Red Interna 1 | 172.168.1.0/24 | debian1 y debian2 |
| Red Interna 2 | 172.168.2.0/24 | debian1, debian3, debian4, debian6 |
| Red Interna 3 | 172.168.3.0/24 | debian5 y debian6 |

### IPs estáticas asignadas

| Máquina | Interfaz | IP |
|---|---|---|
| debian1 | enp0s8 (Host-only) | 192.168.104.10 |
| debian1 | enp0s9 (Red Interna 1) | 172.168.1.1 |
| debian1 | enp0s10 (Red Interna 2) | 172.168.2.1 |
| debian2 | enp0s3 (Red Interna 1) | 172.168.1.2 |
| debian3 | enp0s3 (Red Interna 2) | DHCP 172.168.2.100 - 172.168.2.200 |
| debian4 | enp0s3 (Red Interna 2) | DHCP 172.168.2.100 - 172.168.2.200 |
| debian5 | enp0s3 (Red Interna 3) | 172.168.3.5 |
| debian6 | enp0s3 (Red Interna 2) | 172.168.2.6 |
| debian6 | enp0s8 (Red Interna 3) | 172.168.3.6 |

---

## Creación de maquinas virtuales de VirtualBox

Crear una máquina virtual a partir de https://cloud.unizar.es/index.php/s/KiiX7H2QJQfbk44 y crear 6 clonaciones enlazadas

Modificar cada adaptador de red, en función de las redes conectas

**debian1** (4 adaptadores):
- Adaptador 1: NAT
- Adaptador 2: Solo-anfitrión (vboxnet0)
- Adaptador 3: Red interna `red1`
- Adaptador 4: Red interna `red2`

**debian2** (1 adaptador):
- Adaptador 1: Red interna `red1`

**debian3** (1 adaptador):
- Adaptador 1: Red interna `red2`

**debian4** (1 adaptador):
- Adaptador 1: Red interna `red2`

**debian5** (1 adaptador):
- Adaptador 1: Red interna `red3`

**debian6** (2 adaptadores):
- Adaptador 1: Red interna `red2`
- Adaptador 2: Red interna `red3`


## Puesta en punto de cada máquina virtual

Por cada máquina virtual se han realizado estos pasos para ponerla a punto.

1. Actualización
```bash
apt-get update
```
2. Instalar sudo
```bash
apt install sudo
```
3. Agregar el usuario as en grupo sudo
```bash
/usr/sbin/usermod -aG sudo as
```
4. Cambiar nombre a la maquina virtual 
```bash
sudo hostnamectl set-hostname debianX
```
5. Modificar tambien el nombre en `/etc/hosts` 
```bash
127.0.1.1 debianX
```

--- 

## Configuración de red 

Para la configuración de red hay que modificar el fichero `/etc/network/interfaces`

### debian1 
```
auto lo
iface lo inet loopback

# NAT 
auto enp0s3
iface enp0s3 inet dhcp

# Red Host-only
auto enp0s8
iface enp0s8 inet static
    address 192.168.104.10
    netmask 255.255.255.0

# Red interna 1
auto enp0s9
iface enp0s9 inet static
    address 172.168.1.1
    netmask 255.255.255.0

# Red interna 2
auto enp0s10
iface enp0s10 inet static
    address 172.168.2.1
    netmask 255.255.255.0

# Ruta estática hacia Red Interna 3 via debian6
post-up ip route add 172.168.3.0/24 via 172.168.2.6
```

### debian2

```
auto lo
iface lo inet loopback

# Red interna 1
auto enp0s3
iface enp0s3 inet static
    address 172.168.1.2
    netmask 255.255.255.0
    gateway 172.168.1.1
```

### debian3 y debian4

```
auto lo
iface lo inet loopback

# Red interna 2 - DHCP
auto enp0s3
iface enp0s3 inet dhcp
```

### debian5

```
auto lo
iface lo inet loopback

# Red interna 3
auto enp0s3
iface enp0s3 inet static
    address 172.168.3.5
    netmask 255.255.255.0
    gateway 172.168.3.6
```

### debian6 — `/etc/network/interfaces`

```
auto lo
iface lo inet loopback

# Red interna 2
auto enp0s3
iface enp0s3 inet static
    address 172.168.2.6
    netmask 255.255.255.0
    gateway 172.168.2.1

# Red interna 3
auto enp0s8
iface enp0s8 inet static
    address 172.168.3.6
    netmask 255.255.255.0
```

--- 

## Instalación y configuración Servidor DHCP en debian1

### Instalación
```bash
sudo apt install isc-dhcp-server
```

### Configuración de interfaz — `/etc/default/isc-dhcp-server`

```
INTERFACESv4="enp0s10"
```

### Configuración del servidor — `/etc/dhcp/dhcpd.conf`

```
subnet 172.168.2.0 netmask 255.255.255.0 {
    range 172.168.2.100 172.168.2.200;
    option routers 172.168.2.1;
}
```

### Inicializar
```bash
sudo systemctl restart isc-dhcp-server
sudo systemctl enable isc-dhcp-server
```

---

## Configurar la máquina debian1 para actuar como router

### Activar IP forwarding en debian1 y debian6

En ambas máquinas entrar en  `/etc/sysctl.conf` y descomentar :

```
net.ipv4.ip_forward=1
```

Para aplicar sin reiniciar:
```bash
sudo sysctl -p
```

### Conectar red interna 3 con debian1

Como debian1 no está conectado directamente a la Red Interna 3, necesita saber que para llegar a ella tiene que pasar por debian6 (172.168.2.6). Por ello hay que agregar en `/etc/network/interfaces`:

```
post-up ip route add 172.168.3.0/24 via 172.168.2.6
```

---

## Servidor SSH en debian5

### Instalación
```bash
sudo apt install openssh-server
```

### Activación
```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```

No fue necesario modificar la configuración por defecto de SSH.

---

## Servidor web en debian2

### Instalación
```bash
sudo apt install nginx
```

### Activación
```bash
sudo systemctl enable nginx
sudo systemctl start nginx
```

### Verificación
Desde debian1:
```bash
wget -q -O- http://172.168.1.2
```

---

## Firewall en debian1

Se creó el script `/etc/firewall.sh` con todas las reglas de iptables:

```bash
#!/bin/bash

# Limpiar reglas anteriores
iptables -F
iptables -t nat -F
iptables -X

# Política por defecto: denegar todo entrante y forward
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT

# Permitir loopback
iptables -A INPUT -i lo -j ACCEPT

# Permitir tráfico ya establecido
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT

# a. Permitir todo el tráfico intranet
iptables -A INPUT -i enp0s9 -j ACCEPT
iptables -A INPUT -i enp0s10 -j ACCEPT
iptables -A FORWARD -i enp0s9 -j ACCEPT
iptables -A FORWARD -i enp0s10 -j ACCEPT

# b. Permitir HTTP hacia debian2 desde extranet
iptables -A FORWARD -i enp0s8 -p tcp --dport 80 -d 172.168.1.2 -j ACCEPT

# b. Permitir SSH hacia debian5 desde extranet
iptables -A FORWARD -i enp0s8 -p tcp --dport 22 -d 172.168.3.5 -j ACCEPT

# c. Bloquear ping desde extranet, permitir desde intranet
iptables -A INPUT -i enp0s8 -p icmp -m icmp --icmp-type echo-request -j DROP
iptables -A INPUT -p icmp -m icmp --icmp-type echo-request -j ACCEPT

# d. Log de tráfico denegado desde extranet
iptables -A INPUT -i enp0s8 -j LOG --log-prefix "FIREWALL DROP: " --log-level 4
iptables -A FORWARD -i enp0s8 -j LOG --log-prefix "FIREWALL DROP: " --log-level 4

# NAT hacia Internet
iptables -t nat -A POSTROUTING -o enp0s3 -j MASQUERADE

```

### Activación
Convertirlo en ejecutable
```bash
sudo chmod +x /etc/firewall.sh
```
Ejecutarlo
```bash
sudo bash /etc/firewall.sh
```

---

## NAT

### Agregar en el firewall

```bash
iptables -t nat -A POSTROUTING -o enp0s8 -j MASQUERADE
```

---

## DNAT

### Agregar en el firewall

```bash
iptables -t nat -A PREROUTING -i enp0s8 -p tcp --dport 80 -j DNAT --to-destination 172.168.1.2:80
iptables -t nat -A PREROUTING -i enp0s8 -p tcp --dport 22 -j DNAT --to-destination 172.168.3.5:22
```

--- 

## Funcionamiento automático

### Guardar reglas actuales de iptables

```bash
sudo apt install iptables-persistent
```

### Crewar script de arranque para el firewall

```bash
sudo vim /etc/rc.local
```
```bash
#!/bin/bash
bash /etc/firewall.sh
exit 0
```
Convertirlo en ejecutable y activarlo
```bash
sudo chmod +x /etc/rc.local
sudo systemctl enable rc-local
```

---

## Verificación final

| Prueba | Comando | Resultado esperado |
|---|---|---|
| Ping Host → debian1 | `ping 192.168.104.10` | Sin respuesta (firewall) |
| Web desde Host | `http://192.168.104.10` en navegador | Página por defecto nginx |
| SSH desde Host | `ssh as@192.168.104.10` | Conecta a debian5 |
| Ping intranet | `ping a cualquier maquina desde cualquier máquina | Responde |
| DHCP | `ip a` en debian3/debian4 | IP en rango 172.168.2.100-200 |
