#!/bin/sh

podir=./po
modir=./mo

if [ $# -eq 0 ]; then
    echo "Usage: update-gmo.sh <LL> [...]"
else
    while [ $# -ne 0 ]; do
        echo "Compile language: $1"
        msgcat $podir/$1/*.po | msgfmt -c -f --statistics -o $modir/$1.gmo -
        shift
    done
fi

