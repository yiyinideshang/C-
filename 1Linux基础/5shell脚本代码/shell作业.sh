#!/bin/bash

touch $1.cpp

echo "#include<iostream>" >> $1.cpp
echo "using namespace std" >> $1.cpp
echo "int main()" >> $1.cpp
echo "{" >> $1.cpp
echo "" >> $1.cpp
echo "  return 0;" >> $1.cpp
echo "}" >> $1.cpp

