#include <stdio.h>

struct Employee
{
    int id;
    char name[50];
    int rank;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int completionTime; // New field to store completion time
};

void sortEmployeesByArrivalTime(struct Employee employees[], int n)
{
    struct Employee temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            // If arrival times are same, compare ranks
            if (employees[j].arrivalTime == employees[j + 1].arrivalTime)
            {
                if (employees[j].rank > employees[j + 1].rank)
                {
                    temp = employees[j];
                    employees[j] = employees[j + 1];
                    employees[j + 1] = temp;
                }
            }
            // Otherwise, sort based on arrival time
            else if (employees[j].arrivalTime > employees[j + 1].arrivalTime)
            {
                temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }
}

void calculateTimes(struct Employee employees[], int n)
{
    employees[0].waitingTime = 0;
    employees[0].turnaroundTime = employees[0].burstTime;
    employees[0].completionTime = employees[0].turnaroundTime + employees[0].arrivalTime;

    for (int i = 1; i < n; i++)
    {
        employees[i].waitingTime = employees[i - 1].completionTime > employees[i].arrivalTime ? employees[i - 1].completionTime - employees[i].arrivalTime : 0;
        employees[i].turnaroundTime = employees[i].waitingTime + employees[i].burstTime;
        employees[i].completionTime = employees[i].turnaroundTime + employees[i].arrivalTime;
    }
}

void displayMeetingSchedule(struct Employee employees[], int n)
{
    printf("\nMeeting Schedule:\n");
    printf("Employee\tRank\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\tCompletion Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%s\t\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", employees[i].name, employees[i].rank,
               employees[i].arrivalTime, employees[i].burstTime, employees[i].waitingTime,
               employees[i].turnaroundTime, employees[i].completionTime);
    }
}

int main(void)
{
    int n;

    printf("Enter the number of employees in the meeting: ");
    scanf("%d", &n);

    struct Employee employees[n];

    for (int i = 0; i < n; i++)
    {
        employees[i].id = i + 1;
        printf("Enter the name of Employee %d: ", i + 1);
        scanf("%s", employees[i].name);
        printf("Enter the rank (priority) for Employee %d: ", i + 1);
        scanf("%d", &employees[i].rank);
        printf("Enter the arrival time for Employee %d: ", i + 1);
        scanf("%d", &employees[i].arrivalTime);
        printf("Enter the burst time (time taken for presentation) for Employee %d: ", i + 1);
        scanf("%d", &employees[i].burstTime);
    }

    // Sort employees by arrival time
    sortEmployeesByArrivalTime(employees, n);

    // Calculate waiting time, turnaround time, and completion time
    calculateTimes(employees, n);

    // Display meeting schedule
    displayMeetingSchedule(employees, n);

    // Calculate averages
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++)
    {
        totalWaitingTime += employees[i].waitingTime;
        totalTurnaroundTime += employees[i].turnaroundTime;
    }

    float averageWaitingTime = totalWaitingTime / n;
    float averageTurnaroundTime = totalTurnaroundTime / n;

    // Print averages
    printf("\nAverage Waiting Time (AWT): %.2f\n", averageWaitingTime);
    printf("Average Turnaround Time (TAT): %.2f\n", averageTurnaroundTime);

    return 0;
}
