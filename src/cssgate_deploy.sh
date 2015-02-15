#!/bin/bash
USERNAME=k3a

cd ~/win/src

echo "Compiling..."
rm ./tcss558
make

echo "Copying to Node 1"
scp -o StrictHostKeyChecking=no ~/win/src/tcss558 ${USERNAME}@172.22.71.7:~    # n01
scp -o StrictHostKeyChecking=no ~/win/src/serverlist.txt ${USERNAME}@172.22.71.7:~    # n01

NODE=2
for COUNTER in {27..37}
do
  echo "Copying to Node ${NODE}"
  scp -o StrictHostKeyChecking=no ~/win/src/tcss558 ${USERNAME}@172.22.71.${COUNTER}:~           # n02
  scp -o StrictHostKeyChecking=no ~/win/src/serverlist.txt ${USERNAME}@172.22.71.${COUNTER}:~    # n01
  ((NODE++))
done 


