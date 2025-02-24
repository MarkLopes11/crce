def find_longest_word(file_name):
    longest_word = ""
    max_length = 0

    with open(file_name, 'r') as file:
        for line in file:
            words = line.split()
            for word in words:
                word = word.strip(".,!?")
                if len(word) > max_length:
                    longest_word = word
                    max_length = len(word)

    return longest_word

file_name = "Lab5/Q4.txt"
longest_word = find_longest_word(file_name)
print("The longest word in the file is:", longest_word)
