emp_list = [
    {"employee": {"arrival_time": 0, "burst_time": 10, "completion_time": -1, "turn_around_time": -1}},
    {"employee": {"arrival_time": 2, "burst_time": 5, "completion_time": -1, "turn_around_time": -1}},
    {"employee": {"arrival_time": 1, "burst_time": 8, "completion_time": -1, "turn_around_time": -1}}
]

time_quantum = 2  # Time quantum for Round Robin
completed_list = []

current_time = 0
while emp_list:
    for emp in emp_list:
        if emp["employee"]["burst_time"] > 0:
            if emp["employee"]["burst_time"] > time_quantum:
                current_time += time_quantum
                emp["employee"]["burst_time"] -= time_quantum
            else:
                current_time += emp["employee"]["burst_time"]  # Use remaining burst time
                emp["employee"]["completion_time"] = current_time
                emp["employee"]["turn_around_time"] = emp["employee"]["completion_time"] - emp["employee"]["arrival_time"]
                emp["employee"]["burst_time"] = 0
                completed_list.append(emp)
                emp_list.remove(emp)
                break  # Move to the next employee once the current one is completed

# Calculate AWT and ATAT
total_waiting_time = 0
total_turnaround_time = 0

for emp in completed_list:
    total_waiting_time += emp["employee"]["turn_around_time"] - emp["employee"]["burst_time"]
    total_turnaround_time += emp["employee"]["turn_around_time"]

# Calculate averages
total_employees = len(completed_list)
average_waiting_time = total_waiting_time / total_employees
average_turnaround_time = total_turnaround_time / total_employees

# Print completion times
for emp in completed_list:
    print("Arrival time:", emp["employee"]["arrival_time"], "Completion time:", emp["employee"]["completion_time"])

# Print AWT and ATAT
print("Average Waiting Time (AWT):", average_waiting_time)
print("Average Turnaround Time (ATAT):", average_turnaround_time)
print("Completed List:", completed_list)
