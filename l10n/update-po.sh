#!/bin/sh

podir=./po
modir=./mo
srcdir=../src

merge_file()
{
    if test -e $podir/$1.pot; then
        if test -e $podir/$2/$1.po; then
            rm -f $podir/$2/$1.po~
            mv $podir/$2/$1.po $podir/$2/$1.po~
            msgmerge --lang=$3 $podir/$2/$1.po~ $podir/$1.pot > $podir/$2/$1.po
        else
            msginit -i $podir/$1.pot -o $podir/$2/$1.po --locale=$2
        fi
    fi
}

merge()
{
    test -e $podir/$1 || mkdir $podir/$1
    for filename in `ls $podir/*.pot`; do
        filename=`echo $filename | sed -e 's|\(.*/\)*\([^/]*\).pot|\2|'`
        echo "    $filename"
        merge_file $filename $1 $2
    done
}

if [ $# -eq 0 ]; then
    echo "Usage: update-po.sh <LL> <Language> [...]"
else
    while [ $# -ne 0 ]; do
        if [ $# -eq 1 ]; then
            echo "Update language: $1 [$1]"
            merge $1 $1
            shift
        else
            echo "Update language: $2 [$1]"
            merge $1 $2
            shift
            shift
        fi
    done
fi

