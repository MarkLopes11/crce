from collections import Counter
import re

def count_words_in_file(filename):
    try:
        with open(filename, 'r') as file:
            content = file.read().lower()  # convert to lowercase for uniform counting
            # Use regex to find all words (alphanumeric and underscore)
            words = re.findall(r'\b\w+\b', content)
            word_counts = Counter(words)
            return word_counts
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        return {}

# Example usage:
file_path = 'file1.txt'  # replace this with your file path
word_frequencies = count_words_in_file(file_path)

# Print word counts
for word, count in word_frequencies.items():
    print(f"{word}: {count}")
