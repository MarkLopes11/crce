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
            elif 'F' in operands or operands.isdigit() or operands.startswith('1'):
                size = 4
            else:
                size = 0
        else:
            size = 0

        # Store symbol in symbol table if present
        if label and operation != 'END':
            symbol_table[label] = {'address': location_counter, 'size': size}  # Store address and size

        # Set pass1 and pass2 columns based on operation
        if operation == 'L':
            pass1_text = f"L, {operands.split(',')[0]}, -"
            if ',' in operands:
                operand_name = operands.split(',')[1]
                if operand_name in symbol_table:
                    addr = symbol_table[operand_name]['address']
                    pass2_text = f"06, {operands.split(',')[0]}, {addr}"
                else:
                    pass2_text = f"06, {operands.split(',')[0]}, ??"
            else:
                pass2_text = "??"
        elif operation == 'A':
            pass1_text = f"A, {operands.split(',')[0]}, -"
            if ',' in operands:
                operand_name = operands.split(',')[1]
                if operand_name in symbol_table:
                    addr = symbol_table[operand_name]['address']
                    pass2_text = f"01, {operands.split(',')[0]}, {addr}"
                else:
                    pass2_text = f"01, {operands.split(',')[0]}, ??"
            else:
                pass2_text = "??"
        elif operation == 'ST':
            pass1_text = f"ST, {operands.split(',')[0]}, -"
            if ',' in operands:
                operand_name = operands.split(',')[1]
                if operand_name in symbol_table:
                    addr = symbol_table[operand_name]['address']
                    pass2_text = f"05, {operands.split(',')[0]}, {addr}"
                else:
                    pass2_text = f"05, {operands.split(',')[0]}, ??"
            else:
                pass2_text = "??"
        elif operation == 'DC':
            pass1_text = "-"
            if "'" in operands:
                value = operands.split("'")[1]
                pass1_text = f"{value}"
                pass2_text = f"{location_counter + size},{value}"
            else:
                pass2_text = f"_"
        elif operation == 'DS':
            pass1_text = "-"
            pass2_text = f"_"
        elif operation == 'USING':
            pass1_text = "-"
            pass2_text = "-"
        elif operation == 'START':
            pass1_text = "-"
            pass2_text = "-"
        elif operation == 'END':
            pass1_text = "-"
            pass2_text = f"_"
        else:
            pass1_text = "-"
            pass2_text = "-"

        # Record this line's information
        pass1_result.append({
            'address': location_counter,
            'label': label,
            'operation': operation,
            'operands': operands,
            'size': size,
            'pass1': pass1_text,
            'pass2': pass2_text
        })

        # Update location counter
        location_counter += size

    # Second pass: resolve symbols
    pass2_result = []

    for entry in pass1_result:
        # If it's a reference to a symbol, look it up in the symbol table
        if entry['operation'] in ['L', 'A', 'ST']:
            if ',' in entry['operands']:
                operand_name = entry['operands'].split(',')[1]
                if operand_name in symbol_table:
                    addr = symbol_table[operand_name]['address']
                    entry['pass2'] = entry['pass2'].replace('??', str(addr))

        pass2_result.append(entry)

    return symbol_table, pass1_result, pass2_result

def format_output(symbol_table, pass1_result, pass2_result):
    print("SYMBOL TABLE:")
    print("Symbol\tAddress\tSize")
    print("------\t-------\t----")
    for symbol, symbol_info in symbol_table.items():
        print(f"{symbol}\t{symbol_info['address']}\t{symbol_info['size']}")
    print()

    print("OUTPUT:")
    print("Label\tOpcode\tOperands\tLC\tPass 1\t\tPass 2")
    print("-------\t-----\t---------\t--\t------\t\t------")
    for entry in pass2_result:
        print(f"{entry['label']:<7} {entry['operation']:<8} {entry['operands']:<14} {entry['address']:<8} {entry['pass1']:<15} {entry['pass2']}")


def main():
    with open('input1.txt', 'r') as file:
        assembly_code = file.read()

    try:
        symbol_table, pass1_result, pass2_result = process_assembly_code(assembly_code)

        format_output(symbol_table, pass1_result, pass2_result)

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()