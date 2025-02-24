#!/bin/sh

printf "Please enter your username:\t"
read username

[[ $username == $USER ]] && echo "$username has logged in" && exit 0
echo "$username has not logged in"
exit 1
