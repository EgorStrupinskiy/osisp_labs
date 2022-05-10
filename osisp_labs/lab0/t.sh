#!/bin/bash
name1=`basename $1 .c`
name1="$name1.out"
if gcc $1 -o $name1;
then
	./$name1 ${@:2}
else
	echo "Unsuccessfull compilation"
fi 
