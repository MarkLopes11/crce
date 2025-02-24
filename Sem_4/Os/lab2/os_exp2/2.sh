#!/bin/bash
printf "Please enter the file name: "
read filename
# -x is used to know whether the file is executable or not
[[ -x $filename ]] && echo "The file is executable" && exit 0
echo "The file is not executable"
exit 1
