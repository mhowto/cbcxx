#!/bin/bash
if test -d aftercpp
then
	rm aftercpp -rf
fi

mkdir -p aftercpp/data-structure

for file in `ls .`
do
	if test -f $file
	then
		gcc -E -P $file > aftercpp/$file
	fi
done

for file in `ls data-structure/*`
do
	if test -f $file
	then
		gcc -E -P $file > aftercpp/$file
	fi
done
