#!/bin/sh
make tcp && make upd
./tcpsres --buf_size 100 --port 501 &
./tcpclres --buf_size 100 --ip 127.0.0.1 --port 501

./udpsres --buf_size 100 --port 503 &
./udpclres --buf_size 100 --ip 127.0.0.1 --port 503
