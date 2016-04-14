#!/bin/bash

scp -r zeromq4-1/ "${1}":~
scp -r zeromq/lib/libzmq.so.5.0.0 root@${1}:/usr/lib
ssh root@${1} ldconfig
ssh root@${1} systemctl stop firewalld
