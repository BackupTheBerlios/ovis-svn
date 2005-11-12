#!/bin/sh
# it should be evident what's this script purpose. 

echo "lines of code on " 
 date 
echo "========================================================================"
echo "total lines of code in .cpp files: "
 find . -name *.cpp -exec cat {} \; | wc -l
echo "total lines of code in .hh files: " 
 find . -name *.hh -exec cat {} \; | wc -l
echo "========================================================================"
