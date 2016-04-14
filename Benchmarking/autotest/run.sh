#!/bin/bash

while [[ $# > 1 ]]
do
key="$1"

case $key in
    -c|--client)
    CLIENT="$2"
    shift # past argument
    ;;
    -s|--server)
    SERVER="$2"
    shift # past argument
    ;;
    -l|--lib)
    LIB="$2"
    shift # past argument
    ;;
    -t|--taskset)
    TASKSET="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done
echo "${CLIENT}"
echo "${SERVER}"
echo "${LIB}"
echo ${TASKSET}

TASKSET="taskset -c ${TASKSET}"

echo ssh ${SERVER} "${TASKSET} ~/zeromq4-1/local_thr tcp://${SERVER}:5555 100000 1000000 > /dev/null 2>&1" &
sleep 1
echo ssh ${CLIENT} "${TASKSET} ~/zeromq4-1/remote_thr tcp://${SERVER}:5555 100000 1000000 > /dev/null 2>&1"
