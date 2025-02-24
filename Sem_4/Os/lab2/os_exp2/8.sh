#!/bin/bash
if ! [[ -f employee.txt ]]; then
 echo "EmpCode,EmpName,Grade,Years of experience,Basic Pay" >employee.txt
 echo "E001,John Doe,A,5,50000" >>employee.txt
 echo "E002,Jane Smith,B,3,45000" >>employee.txt
 echo "E003,Bob Johnson,C,8,60000" >>employee.txt
 echo "E004,Alice Williams,A,6,52000" >>employee.txt
 echo "E005,Charlie Brown,D,2,40000" >>employee.txt
 echo "E006,Emily smith,B,4,48000" >>employee.txt
 echo "E007,David Miller,C,7,55000" >>employee.txt
 echo "E008,Emma White,A,9,62000" >>employee.txt
 echo "E009,Frank Thompson,D,1,38000" >>employee.txt
 echo "E010,Grace Turner,B,5,51000" >>employee.txt
 echo "E011,Henry Harris,C,3,47000" >>employee.txt
 echo "E012,Isabel Clark,A,8,59000" >>employee.txt
 echo "E013,Jack Turner,D,2,42000" >>employee.txt
 echo "E014,Karen Anderson,B,4,49000" >>employee.txt
 echo "E015,Liam Smith,C,6,54000" >>employee.txt
 echo "E016,Mia Robinson,A,7,56000" >>employee.txt
 echo "E017,Noah Walker,D,5,53000" >>employee.txt
 echo "E018,Olivia Harris,B,2,43000" >>employee.txt
 echo "E019,Peter Turner,C,4,50000" >>employee.txt
 echo "E020,Quinn Adams,A,1,40000" >>employee.txt
fi

if ! [[ -f emp_name.txt ]]; then
 # sort the keys seperated by comma from 2nd column to 2nd column
 sort -t ',' -k 2,2 -o emp_name.txt employee.txt
fi

if ! [[ -f emp_salary.txt ]]; then
 # sort the 5 row by comparing the numeric value and print in reverse
 sort -t ',' -k 5,5 -nr -o emp_salary.txt employee.txt
fi

if ! [[ -f emp_year.txt ]]; then
 # sort the 5 row by comparing the numeric value and print in reverse
 sort -t ',' -k 4,4 -n -o emp_year.txt employee.txt
fi

# or we can do $(($wc -l employee.txt | awk '$1-=1;{print $1}) - 1 ))'
echo "The number of employees are: $(($(bat employee.txt | wc -l) - 1))"
# just grep the case insensitive smith word
echo -e "\nThe employees who have 'smith' in thier name are\n $(bat employee.txt | rg -i smith)"
# basically some regex magic that cannot be explained in simple words.
echo -e "\nThe employees whose name starts with 'B' are\n $(bat employee.txt | rg ".*[E0-9],B.*\ .*,[A-E],.*[0-9],,*[0-9]")"
