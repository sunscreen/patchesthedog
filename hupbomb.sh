#!/bin/bash
# Basic while loop
args=("$@")
echo $# arguments passed
echo ${args[0]} ${args[1]} ${args[2]}

counter=1
pid=${args[0]}
while [ $counter -le 100000 ]
do
echo "$counter huping pid $pid"
kill -HUP $pid
((counter++))
done
echo All done
