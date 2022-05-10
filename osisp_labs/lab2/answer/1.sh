#!/bin/bash
>$2
IFS=$'\n'
path=$( readlink -f $1  2>/tmp/errors.err )
res=""
for i in $((find "$path" -type d) 2>>/tmp/errors.err); do
 count=0
 size=0
 for  j in $((find $i -maxdepth 1 -type f)2>>/tmp/errors.err); do
  let count=count+1
  let size=$(( stat -c %s $j)2>>/tmp/errors.err)+size
 done
 res="${res} $i $size $count\n"
done
echo -ne "${res}">>$2
while read line; do
 echo "${0##*/}: $line"
done <  /tmp/errors.err
rm /tmp/errors.err
