def count_vowels_consonants(file_name):
    with open(file_name, 'r') as file:
        num_vowels = 0
        num_consonants = 0
        total_characters = 0
        vowels = "aeiou"
        
        for char in file.read():
            if char.isalpha():
                total_characters += 1
                if char.lower() in vowels:
                    num_vowels += 1
                else:
                    num_consonants += 1

        percent_vowels = (num_vowels / total_characters) * 100
        percent_consonants = (num_consonants / total_characters) * 100

        print(f"Percentage of vowels: {percent_vowels:.2f}%")
        print(f"Percentage of consonants: {percent_consonants:.2f}%")


file_name = "Lab5/lab5_test.txt"  
count_vowels_consonants(file_name)
