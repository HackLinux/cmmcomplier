#!/bin/bash

for i in `ls`;
do
    [[ $i == *.c ]] && printf "\n_______________%s_______________\n" $i;
    [[ $i == *.c ]] && ./parser $i $i.s;
done