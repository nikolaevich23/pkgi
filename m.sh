#!/bin/sh
clear
make clean
make
cp -r ./RUS/build ./
make pkg
make clean
