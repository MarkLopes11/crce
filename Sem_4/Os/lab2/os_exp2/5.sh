unique=$(bat firstFile.txt | tr ' ' '\n' | sort | uniq)
uniqueCount=$(bat firstFile.txt | tr ' ' '\n' | sort | uniq | wc -l)
echo -e "The count of unique words is: ${uniqueCount}\n"
echo -e "Word\t\t Count"
for i in $unique; do
  printf "$i\t\t $(bat firstFile.txt | tr ' ' '\n' | rg "$i" | wc -l)\n"
done
