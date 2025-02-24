with open("Lab5/Q4.txt", 'r') as input_file:
    data = input_file.read()

data_without_digits = ''.join(char for char in data if not char.isdigit())

with open("Lab5/Q4output.txt", 'w') as output_file:
    output_file.write(data_without_digits)

