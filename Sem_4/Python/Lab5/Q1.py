def count_lines_words_characters(file_name):
    with open(file_name, 'r') as file:
        num_lines = 0
        num_words = 0
        num_characters = 0

        for line in file:
            num_lines += 1
            words = line.split()
            num_words += len(words)
            num_characters += len(line)

    print(f"Number of lines: {num_lines}")
    print(f"Number of words: {num_words}")
    print(f"Number of characters: {num_characters}")

file_name = "Lab5/lab5_test.txt"  
count_lines_words_characters(file_name)
