# Basic agent-based infectious disease simulation using Python
import numpy as np
import matplotlib.pyplot as plt

# Parameters
population = 500
initial_infected = 5
infection_rate = 0.05
recovery_time = 14
days = 100

# States: 0 = Susceptible, 1 = Infected, 2 = Recovered
states = np.zeros(population, dtype=int)
infection_days = np.zeros(population, dtype=int)

# Infect initial individuals
infected_indices = np.random.choice(population, initial_infected, replace=False)
states[infected_indices] = 1

# Metrics tracking
susceptible_counts = []
infected_counts = []
recovered_counts = []

# Simulation loop
for day in range(days):
    new_states = states.copy()
    for i in range(population):
        if states[i] == 1:  # Infected
            # Try to infect others
            for j in range(population):
                if states[j] == 0 and np.random.rand() < infection_rate:
                    new_states[j] = 1
            infection_days[i] += 1
            if infection_days[i] >= recovery_time:
                new_states[i] = 2  # Recovered

    states = new_states
    susceptible_counts.append(np.sum(states == 0))
    infected_counts.append(np.sum(states == 1))
    recovered_counts.append(np.sum(states == 2))

# Calculate metrics
peak_infected = max(infected_counts)
peak_day = infected_counts.index(peak_infected)
final_attack_rate = recovered_counts[-1] / population * 100

# Plotting results
plt.figure(figsize=(10, 6))
plt.plot(susceptible_counts, label='Susceptible')
plt.plot(infected_counts, label='Infected')
plt.plot(recovered_counts, label='Recovered')
plt.axvline(peak_day, color='r', linestyle='--', label='Peak Infection Day')
plt.title('Infectious Disease Simulation (Agent-Based Model)')
plt.xlabel('Days')
plt.ylabel('Number of Individuals')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

peak_infected, peak_day, final_attack_rate
