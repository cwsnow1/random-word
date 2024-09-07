#!/bin/bash

FLAGS="-Wall -Werror -Wextra -O3 -std=c++23 -fno-exceptions -fno-rtti -fno-omit-frame-pointer -Wno-unused-parameter"
CC="clang++"

SRC="main.cpp phonology.cpp metropolitan_french.cpp"
EXE="generator"

$CC $FLAGS $SRC -o $EXE
