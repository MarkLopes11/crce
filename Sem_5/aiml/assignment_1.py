import matplotlib.pyplot as plt
import numpy as np

# Example data (replace with actual values)
time_points = ['Dec 2020', 'Jun 2021', 'Sep 2021', 'Dec 2021', 'Mar 2022', 'Sep 2022']
israel_vaccinations = [0, 8, 12, 14, 16, 18]  # Example values in millions
algeria_vaccinations = [0, 2, 6, 8, 10, 14]   # Example values in millions

x = np.arange(len(time_points))  # the label locations
width = 0.35  # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, israel_vaccinations, width, label='Israel')
rects2 = ax.bar(x + width/2, algeria_vaccinations, width, label='Algeria')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_xlabel('Time Points')
ax.set_ylabel('Vaccinations (in millions)')
ax.set_title('Vaccinations in Israel and Algeria over time')
ax.set_xticks(x)
ax.set_xticklabels(time_points)
ax.legend()

fig.tight_layout()

plt.show()
