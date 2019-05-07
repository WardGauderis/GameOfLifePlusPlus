#!/usr/bin/env bash
for dir in ./*
do
	if [ -d $dir ]; then
		cd $dir
		cmake CMakeLists.txt
		make
		cd ..
	fi
done;
