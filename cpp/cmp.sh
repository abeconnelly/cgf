#!/bin/bash

#g++ -g cgb.cpp cgb_read.cpp cgb_print.cpp dlug.c -o cgb
g++ -O3 main.cpp cgb.cpp cgb_read.cpp cgb_print.cpp dlug.c -o cgb

# g++ -g dlug.c -o dlug
