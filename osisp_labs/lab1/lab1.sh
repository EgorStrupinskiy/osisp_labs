#!/bin/bash
path=$(readlink -f $1)
str=$2
for i in $((find $path -type d) 2>>/tmp/errors.err); do
	count=0
	size=0
	for  j in $((find $i -maxdepth 1 -type f)2>>/tmp/errors.err); do
		if [[ "$j" == *"$str"* ]]; then
			let count=count+1
			let size=$(( stat -c %s $j)2>>/tmp/errors.err)+size
		fi
	done
	if (($count > 0)); then
		echo "$i $count $size"
	fi
done
while read line; do
	echo "${0##*/}: $line" 1>&2
done <  /tmp/errors.err
rm /tmp/errors.err
