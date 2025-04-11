import csv

# Probabilities from the diagram (same as before)
P_B_true = 0.001
P_B_false = 1 - P_B_true
P_E_true = 0.002
P_E_false = 1 - P_E_true

P_A_true_given_BT_ET = 0.95
P_A_false_given_BT_ET = 1 - P_A_true_given_BT_ET
P_A_true_given_BT_EF = 0.94
P_A_false_given_BT_EF = 1 - P_A_true_given_BT_EF
P_A_true_given_BF_ET = 0.29
P_A_false_given_BF_ET = 1 - P_A_true_given_BF_ET
P_A_true_given_BF_EF = 0.001
P_A_false_given_BF_EF = 1 - P_A_true_given_BF_EF

P_J_true_given_AT = 0.90
P_J_false_given_AT = 1 - P_J_true_given_AT
P_J_true_given_AF = 0.05
P_J_false_given_AF = 1 - P_J_true_given_AF

P_M_true_given_AT = 0.70
P_M_false_given_AT = 1 - P_M_true_given_AT
P_M_true_given_AF = 0.01
P_M_false_given_AF = 1 - P_M_true_given_AF

def calculate_joint_probability_user_input(burglary, earthquake, alarm, john_calls, mary_calls):
    """
    Calculates the joint probability P(B, E, A, J, M) for user-specified boolean assignments.
    """
    b = burglary
    e = earthquake
    a = alarm
    j = john_calls
    m = mary_calls

    p_b = P_B_true if b else P_B_false
    p_e = P_E_true if e else P_E_false

    if b and e:
        p_a_given_be = P_A_true_given_BT_ET if a else P_A_false_given_BT_ET
    elif b and not e:
        p_a_given_be = P_A_true_given_BT_EF if a else P_A_false_given_BT_EF
    elif not b and e:
        p_a_given_be = P_A_true_given_BF_ET if a else P_A_false_given_BF_ET
    else:  # not b and not e
        p_a_given_be = P_A_true_given_BF_EF if a else P_A_false_given_BF_EF

    # Calculate P(J|A) based on the state of Alarm (a)
    # Note: The original code had a mistake here, it should depend on 'a', not 'j'
    p_j_given_a = P_J_true_given_AT if a else P_J_true_given_AF
    # Use the correct probability for John calling (True) or not calling (False)
    p_j_actual = p_j_given_a if j else (1 - p_j_given_a)

    # Calculate P(M|A) based on the state of Alarm (a)
    # Note: The original code had a mistake here, it should depend on 'a', not 'm'
    p_m_given_a = P_M_true_given_AT if a else P_M_true_given_AF
    # Use the correct probability for Mary calling (True) or not calling (False)
    p_m_actual = p_m_given_a if m else (1 - p_m_given_a)


    # The joint probability is the product of the relevant probabilities
    return p_b * p_e * p_a_given_be * p_j_actual * p_m_actual

if __name__ == "__main__":
    dataset_filename = 'data.csv'  # Make sure this file exists and is correctly formatted

    try:
        # Open the CSV file, specifying newline='' for universal newline handling
        with open(dataset_filename, mode='r', newline='') as csvfile:
            # --- Specify tab delimiter ---  <--- THIS IS THE KEY CHANGE
            csv_reader = csv.DictReader(csvfile, delimiter='\t')

            # --- Print detected headers ---
            print(f"Detected column headers: {csv_reader.fieldnames}")

            # --- Validate headers ---
            expected_headers = ['Burglary', 'Earthquake', 'Alarm', 'JohnCalls', 'MaryCalls']
            if not csv_reader.fieldnames or not all(header in csv_reader.fieldnames for header in expected_headers):
                 print(f"Error: CSV header mismatch.")
                 print(f"Expected headers: {expected_headers}")
                 print(f"Found headers: {csv_reader.fieldnames}")
                 print(f"Please check the header row and delimiter in '{dataset_filename}'.")
                 exit() # Exit if headers are not as expected

            scenario_count = 0
            # Iterate through each row in the CSV file (each row is a dictionary)
            for row in csv_reader:
                try:
                    # Extract values and convert to boolean
                    # Handles 'true', 't', '1' as True, others as False
                    burglary_input = row['Burglary'].strip().lower() in ['true', 't', '1']
                    earthquake_input = row['Earthquake'].strip().lower() in ['true', 't', '1']
                    alarm_input = row['Alarm'].strip().lower() in ['true', 't', '1']
                    john_calls_input = row['JohnCalls'].strip().lower() in ['true', 't', '1']
                    mary_calls_input = row['MaryCalls'].strip().lower() in ['true', 't', '1']

                    # Calculate the joint probability for the current scenario
                    joint_probability = calculate_joint_probability_user_input(
                        burglary_input, earthquake_input, alarm_input, john_calls_input, mary_calls_input
                    )

                    scenario_count += 1
                    print(f'\nScenario {scenario_count}:')
                    # Print the raw input from the CSV row
                    print(f"  Input: Burglary={row['Burglary']}, Earthquake={row['Earthquake']}, Alarm={row['Alarm']}, JohnCalls={row['JohnCalls']}, MaryCalls={row['MaryCalls']}")
                    # Print how the input was interpreted as boolean values
                    print(f"  Processed as: Burglary={burglary_input}, Earthquake={earthquake_input}, Alarm={alarm_input}, JohnCalls={john_calls_input}, MaryCalls={mary_calls_input}")
                    # Print the calculated joint probability
                    print(f"  Joint Probability: {joint_probability}")

                except KeyError as ke:
                    # Handle cases where a column expected by the code is missing in a row
                    print(f"\nError processing row {scenario_count + 1}: Missing column key {ke}")
                    print(f"  Row data: {row}")
                    print(f"  Make sure the column names in the CSV header exactly match the code's expectations.")
                    break # Stop processing further rows if a key error occurs
                except Exception as row_e:
                     # Handle any other unexpected errors during row processing
                     print(f"\nError processing row {scenario_count + 1}: {row_e}")
                     print(f"  Row data: {row}")
                     # Depending on the error, you might want to 'continue' or 'break'

            print(f'\nProcessed {scenario_count} scenarios from the dataset.')

    except FileNotFoundError:
        # Handle the case where the specified CSV file does not exist
        print(f"Error: Dataset file '{dataset_filename}' not found. Make sure the file exists in the same directory as the script, or provide the correct file path.")
    except Exception as e:
        # Catch any other unexpected errors during file opening or initial setup
        print(f"An unexpected error occurred: {e}")