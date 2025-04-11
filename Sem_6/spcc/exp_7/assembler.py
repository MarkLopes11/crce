def process_assembly_code(code):
    # Clean up input - remove extra asterisks and split into lines
    lines = [line.strip().replace('*', '') for line in code.split('\n')]
    lines = [line for line in lines if line]  # Remove empty lines

    # First pass: build symbol table and assign addresses
    symbol_table = {}
    location_counter = 0
    pass1_result = []

    for line in lines:
        parts = line.split()
        if not parts:  # Skip empty lines
            continue

        # Handle different line formats (with or without label)
        if parts[0] in ['START', 'USING', 'END']:
            # These are directives without labels
            label = ""
            operation = parts[0]
            operands = ' '.join(parts[1:]) if len(parts) > 1 else ""
            size = 0
        else:
            # Check if first part is a label or operation
            if parts[0] in ['L', 'A', 'ST', 'DC', 'DS']:
                # No label
                label = ""
                operation = parts[0]
                operands = ' '.join(parts[1:])
            else:
                # Has label
                label = parts[0]
                operation = parts[1]
                operands = ' '.join(parts[2:]) if len(parts) > 2 else ""

        # Determine size based on operation
        if operation in ['L', 'A', 'ST']:
            size = 4
        elif operation == 'DC':
            if 'H' in operands:
                size = 2
            elif 'F' in operands:
                size = 4
            else:
                size = 0
        elif operation == 'DS':
            if 'H' in operands:
                size = 2
            elif 'F' in operands:
                size = 4
            else:
                size = 0
        else:
            size = 0

        # Store symbol in symbol table if present
        if label and operation != 'END':
            symbol_table[label] = {'address': location_counter, 'size': size} # Store address and size

        # Record this line's information
        pass1_result.append({
            'address': location_counter,
            'label': label,
            'operation': operation,
            'operands': operands,
            'size': size
        })

        # Update location counter
        location_counter += size

    # Second pass: no object code resolution
    pass2_result = []

    for entry in pass1_result:
        pass2_result.append({
            'address': entry['address'],
            'label': entry['label'],
            'operation': entry['operation'],
            'operands': entry['operands'],
            'size': entry['size']
            # Object code logic removed
        })

    return symbol_table, pass1_result, pass2_result

def format_output(symbol_table, pass1_result, pass2_result):
    # Format the symbol table
    print("SYMBOL TABLE:")
    print("Symbol\tAddress\tSize")
    print("------\t-------\t----")
    for symbol, symbol_info in symbol_table.items():
        print(f"{symbol}\t{symbol_info['address']}\t{symbol_info['size']}")
    print()

    # Format Pass 2 result - Object Code column removed
    print("OUTPUT:")
    print("Address\tLabel\tOperation\tOperands\tSize")
    print("-------\t-----\t---------\t--------\t----")
    for entry in pass2_result:
        print(f"{entry['address']}\t{entry['label']}\t{entry['operation']}\t\t{entry['operands']}\t\t{entry['size']}")


def main():
    # Ask for the input file name
    file_name = 'input.txt'

    try:
        # Read the assembly code from the file
        with open(file_name, 'r') as file:
            assembly_code = file.read()

        # Process the code
        symbol_table, pass1_result, pass2_result = process_assembly_code(assembly_code)

        # Output the results
        format_output(symbol_table, pass1_result, pass2_result)

    except FileNotFoundError:
        print(f"Error: File '{file_name}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()