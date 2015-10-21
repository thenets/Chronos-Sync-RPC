#!/bin/bash

# =================================================== #
#
# Alguns pacotes necessários:
# 		sudo apt-get install rpcbind gcc build-essential
#
# =================================================== #


# Interface's name
# ======================
$INTERFACE_NAME = "chronos"



clear

echo -e "\e[36mAccess lib directory\e[31m"
cd lib

echo -e "\e[36mRemoving old files...\e[33m"
rm *.[coh]
rm ../dist/*
 
echo -e "\e[36mGenerating RPC structure from chronos.x ...\e[31m"
rm chronos.x
cp -f ../app/chronos.x ./chronos.x
rpcgen -C chronos.x

echo -e "\e[36mGenerating interface compiled library...\e[31m"
gcc -c chronos_xdr.c -o chronos_xdr.o

echo -e "\e[36mGenerating server binaries...\e[31m"
gcc -c ../app/server.c -o server.o -lm
gcc -c chronos_svc.c -o chronos_svc.o -lm
gcc -o ../dist/server server.o chronos_xdr.o chronos_svc.o -lm

echo -e "\e[36mGenerating client binaries...\e[31m"
gcc -c ../app/client.c -o client.o
gcc -c chronos_clnt.c -o chronos_clnt.o
gcc -o ../dist/client client.o chronos_xdr.o chronos_clnt.o	

echo -e "\e[36mGiving run rules to client and server executables...\e[31m"
cd ../dist/
chmod +x client server

echo -e "\e[36mAll done!\e[39m" 