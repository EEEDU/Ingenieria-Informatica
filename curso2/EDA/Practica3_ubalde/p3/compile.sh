#!/bin/bash

mkdir output 2> /dev/null || true

g++ -std=c++11 *.cpp -o output/test
