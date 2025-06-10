#!/bin/bash

#Nombre del contenedor Docker
CONTAINER_NAME="SO-TP2"

BUDDY_FLAG=""
if [ "$1" = "-buddy" ]; then
    BUDDY_FLAG="BUDDY"
fi
#Iniciar el contenedor
docker start $CONTAINER_NAME

#Limpiar compilación en /root/Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root/Toolchain

#Limpiar compilación en /root
docker exec -it $CONTAINER_NAME make clean -C /root/

#Compilar en /root/Toolchain
docker exec -it $CONTAINER_NAME make MM=$BUDDY_FLAG -C /root/Toolchain

#Compilar en /root
docker exec -it $CONTAINER_NAME make MM=$BUDDY_FLAG -C /root/

#Detener el contenedor
docker stop $CONTAINER_NAME