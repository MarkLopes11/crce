#!/bin/bash
echo -e "Menu:\n1.ls\n2.pwd\n3.ls -l\n4.ps -fe"
echo -e "Please select one from the menu(1 | 2 | 3| 4):"
read ans
case "$ans" in
 1)
   echo "The output of ls is:"
   ls;;
 2)
   echo "The output of pwd is:"
   pwd;;
 3)
   echo "The output of ls -l is:"
   ls -l;;
 4)
   echo "The output of ps -fe is:"
   ps -fe;;
 *)
   echo "Please select a valid option"
   ;;
esac
