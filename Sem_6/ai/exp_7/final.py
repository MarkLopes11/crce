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

    Args:
        burglary (bool): True if burglary occurred, False otherwise.
        earthquake (bool): True if earthquake occurred, False otherwise.
        alarm (bool): True if alarm is ringing, False otherwise.
        john_calls (bool): True if John calls, False otherwise.
        mary_calls (bool): True if Mary calls, False otherwise.

    Returns:
        float: The joint probability for the given combination of events.
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

    p_j_given_a = P_J_true_given_AT if a else P_J_true_given_AF
    p_m_given_a = P_M_true_given_AT if a else P_M_true_given_AF

    return p_b * p_e * p_a_given_be * p_j_given_a * p_m_given_a

if __name__ == "__main__":
    print("Enter the states (True/False/t/f) for each event:")
    burglary_input_str = input("Burglary (True/False/t/f): ").strip().lower()
    burglary_input = burglary_input_str == 'true' or burglary_input_str == 't'

    earthquake_input_str = input("Earthquake (True/False/t/f): ").strip().lower()
    earthquake_input = earthquake_input_str == 'true' or earthquake_input_str == 't'

    alarm_input_str = input("Alarm (True/False/t/f): ").strip().lower()
    alarm_input = alarm_input_str == 'true' or alarm_input_str == 't'

    john_calls_input_str = input("JohnCalls (True/False/t/f): ").strip().lower()
    john_calls_input = john_calls_input_str == 'true' or john_calls_input_str == 't'

    mary_calls_input_str = input("MaryCalls (True/False/t/f): ").strip().lower()
    mary_calls_input = mary_calls_input_str == 'true' or mary_calls_input_str == 't'
    joint_probability = calculate_joint_probability_user_input(
        burglary_input, earthquake_input, alarm_input, john_calls_input, mary_calls_input
    )

    print("\nJoint Probability for the given scenario:")
    print(f"P(Burglary={burglary_input}, Earthquake={earthquake_input}, Alarm={alarm_input}, JohnCalls={john_calls_input}, MaryCalls={mary_calls_input}) = {joint_probability}")