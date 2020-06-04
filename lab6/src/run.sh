#!/bin/sh
make client && make server
./server.out --port 323 --tnum 2 &
./server.out --port 229 --tnum 3 &
./client.out --k 20 --mod 1234 --servers servers.txt &
