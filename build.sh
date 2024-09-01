#!/bin/bash

FLAGS="-Wall -Werror -Wextra -O3 -std=c++23 -fno-exceptions -fno-rtti -fno-omit-frame-pointer"
CC="clang++"

SRC="main.cpp phonology.cpp american_english.cpp"
EXE="generator"

$CC $FLAGS $SRC -o $EXE
