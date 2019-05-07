#!/usr/bin/env bash
for dir in .
do
	cmake CMakeLists.txt
	make
done;
