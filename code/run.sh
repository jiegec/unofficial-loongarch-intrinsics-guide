#!/bin/sh
set -x -e

for program in $@
do
	./$program
done
