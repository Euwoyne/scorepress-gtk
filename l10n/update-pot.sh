#!/bin/sh

PACKAGE="scorepress-gtk"
VER="0.1.2"
COPYRIGHT="Dominik Lehmann"
MAIL="dominik.lehmann-dev@arcor.de"

podir=./po
modir=./mo
srcdir=../src

update_file()
{
    name=$1; shift
    xgettext -L C++ -k_ -kN_ -c/ -o $podir/$name.pot --package-name="$PACKAGE" --package-version="$VER" --foreign-user --copyright-holder="$COPYRIGHT" --msgid-bugs-address="$MAIL" $@
}

update_file general  -D$srcdir about_dialog.cpp application.cpp rsvg_renderer.cpp
update_file mainwnd  -D$srcdir mainwnd.cpp
update_file messages --exclude-file=$podir/general.pot --exclude-file=$podir/mainwnd.pot $srcdir/*.cpp

