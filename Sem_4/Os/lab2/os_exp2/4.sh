#!/bin/bash
sumOfNumbers=0
product=1
for i in {1..4}; do
 printf "Enter a number\n"
 read num
 sumOfNumbers=$(( sumOfNumbers +  num ))
 product=$(( product * num ))
done
echo -e "The sum is ${sumOfNumbers}.\nThe product is ${product}.\nThe average of the numbers is $(( sumOfNumbers / 4 ))"
