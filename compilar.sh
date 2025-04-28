#!/bin/bash

#Nombre del contenedor Docker
CONTAINER_NAME="SO-TP2"

#Iniciar el contenedor
docker start $CONTAINER_NAME

#Limpiar compilación en /root/Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root/Toolchain

#Limpiar compilación en /root
docker exec -it $CONTAINER_NAME make clean -C /root/

#Compilar en /root/Toolchain
docker exec -it $CONTAINER_NAME make -C /root/Toolchain

#Compilar en /root
docker exec -it $CONTAINER_NAME make -C /root/

#Detener el contenedor
docker stop $CONTAINER_NAME