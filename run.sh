#!/bin/sh
usage="Usage: $0 [#process] <program> <program argument>"
pathToMpiexec="/usr/lib64/openmpi/bin/mpiexec"
if [ $# -eq 2 ]; then
    $pathToMpiexec -hostfile hostfile $1 $2
elif [ $# -eq 3 ]; then
    $pathToMpiexec -n $1 -hostfile hostfile $2 $3
else
    echo $usage
    exit 1
fi
