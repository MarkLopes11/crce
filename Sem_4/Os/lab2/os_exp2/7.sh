#!/bin/bash

# Prompt the user to enter a starting value
echo "Enter a starting value:"
read start_value

# Count down from the starting value
echo -n "$start_value"
while [ $start_value -gt 0 ]; do
    ((start_value--))
    echo -ne "\t$start_value"
done
echo
