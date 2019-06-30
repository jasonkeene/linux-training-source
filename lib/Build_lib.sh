#!/bin/sh

sh Build_ename.sh > ename.c.inc

echo "======================================================================"
echo
echo "You can probably ignore errors that you see in the following compilation"
echo
cc -c *.c
echo "======================================================================"

set -v
ar rs ../libtlpi.a
