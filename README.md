# Process Scheduling Simulator - FCFS and SJF 

This program simulates two classic **CPU scheduling algorithms** — **First Come First Served (FCFS)** and **Shortest Job First (SJF)** — implemented in C under Linux. It calculates and displays **Waiting Time (WT)**, **Turnaround Time (TAT)**, and **Response Time (RT)** for each process, along with a Gantt Chart showing the execution order. 

## Compilation and Execution 
### **1. Compile**
```bash
gcc scheduling_simulator.c -o scheduling_simulator
```
**2. Run**
```bash
./scheduling_simulator
```

## Input Format 
The program first asks for the number of processes, then for each process’s arrival time and burst time.
```bash
Enter the number of processes: 4
Enter the arrival time and burst time for process 1: 0 8
Enter the arrival time and burst time for process 2: 1 4
Enter the arrival time and burst time for process 3: 2 9
Enter the arrival time and burst time for process 4: 3 5
```

## Output Screenshots
The output screenshots for both **FCFS** and **SJF** algorithms can be found in this repository.
