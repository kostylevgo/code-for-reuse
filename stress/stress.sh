#!/bin/bash

compile="g++ -std=c++20 -DSTRESS -D_GLIBCXX_DEBUG -fsanitize=address,undefined -fno-sanitize-recover -g -o"
$compile wa ../src/G.cpp
$compile ok ../src/G_ok.cpp

while [ true ]; do
  python3 gen.py > test.txt || break
  ./wa < test.txt > wa.txt || break
  ./ok < test.txt > ok.txt || break
  diff wa.txt ok.txt || break
done
