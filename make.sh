#! /bin/bash

if [ $# -ne 1 ] ; then
    echo "# Usage: sh $0 [filename (w/o .cc)]"
    exit 0
fi
g++ -Wall -O3 `root-config --cflags --libs` src/$1.cc -o bin/$1
