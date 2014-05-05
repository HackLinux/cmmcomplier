#!/bin/bash

for i in `ls`;
do
    [[ $i == [ABC]* ]] && printf "\n_______________%s_______________\n" $i;
    [[ $i == [ABC]* ]] && ./parser $i;
done

