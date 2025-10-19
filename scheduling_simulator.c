#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int pid;
	int arrival_time;
	int burst_time;
	int turnaround_time;
	int waiting_time;
	int response_time;
	int start_time;
	int completion_time;
	int completed;
} Process;

typedef struct {
	int pid;
	int start;
	int end;
} Segment;

static int sort_by_arrival_time(const void *a, const void *b) {
	const Process *x = (const Process*)a, *y = (const Process*)b;
	if (x->arrival_time != y->arrival_time){
		return x->arrival_time - y->arrival_time;
	}
	return x->pid - y->pid;
}

static void reset(Process *p, int n) {
	for (int i = 0; i < n; ++i) {
		p[i].turnaround_time = p[i].waiting_time = p[i].response_time = 0;
		p[i].start_time = p[i].completion_time = 0;
		p[i].completed = 0;
	}
}

static void print(const Segment *g, int num_segments) {
	printf("Gantt Chart: ");
	for (int i = 0; i < num_segments; ++i) {
		int width = g[i].end - g[i].start;
		if (width < 1) {
			width = 1;
		}

		int inner = width >= 3 ? width - 2 : 1;
		printf("| P%d", g[i].pid);
		for (int t = 0; t < inner; ++t) {
			putchar(' ');
		}

		printf("| ");
	}

	printf("\n\n");
}

static void print_table(const Process *p, int n) {
	double average_w = 0, average_t = 0, average_r = 0;
	printf("PID    AT    BT    WT    TAT    RT\n");

	for(int i = 0; i < n; ++i) {
		printf("%-7d %-6d %-6d %-6d %-6d %-6d\n",
				p[i].pid, p[i].arrival_time, p[i].burst_time,
				p[i].waiting_time, p[i].turnaround_time, p[i].response_time);

        average_w += p[i].waiting_time;
	average_t += p[i].turnaround_time;
	average_r += p[i].response_time;
	}

        average_w /= n;
	average_t /= n;
	average_r /= n;

	printf("\nAverage Waiting Time: %.2f\n", average_w);
        printf("Average Turnaround Time: %.2f\n", average_t);
        printf("Average Response Time: %.2f\n", average_r);
}

static void fcfs(Process *src, int n) {
	Process *p = malloc(n * sizeof(Process));
	memcpy(p, src, n * sizeof(Process));
	qsort(p, n, sizeof(Process), sort_by_arrival_time);
	reset(p, n);

	Segment *g = malloc(n * sizeof(Segment));
	int num_segments = 0;

	int time = 0;
	for (int i = 0; i < n; ++i) {
		if (time < p[i].arrival_time) {
			time = p[i].arrival_time;
		}

		p[i].start_time = time;
		p[i].waiting_time = p[i].start_time - p[i].arrival_time;
		p[i].response_time = p[i].waiting_time;
		p[i].completion_time = p[i].start_time + p[i].burst_time;
		p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;

		g[num_segments++] = (Segment) { p[i].pid, p[i].start_time, p[i].completion_time };
		time = p[i].completion_time;
	}

	printf("=== First Come First Served (FCFS) ===\n");
        print(g, num_segments);
        print_table(p, n);
        putchar('\n');

        free(g);
        free(p);
}


static void sjf(Process *src, int n) {
	Process *p = malloc(n * sizeof(Process));
	memcpy(p, src, n * sizeof(Process));
	reset(p, n);

	Segment *g = malloc(n * sizeof(Segment));
	int num_segments = 0;

	int time = 0;
	int done = 0;

	while (done < n) {
		int best = -1;
		for (int i = 0; i < n; ++i) {
			if (!p[i].completed && p[i].arrival_time <= time) {
				if (best == -1) {
					best = i;
				}
	             		else {
					if (p[i].burst_time < p[best].burst_time ||
					   (p[i].burst_time == p[best].burst_time && p[i].arrival_time < p[best].arrival_time) ||
					   (p[i].burst_time == p[best].burst_time && p[i].arrival_time == p[best].arrival_time && p[i].pid < p[best].pid)) {
						best = i;
					}
				}
			}
		}

		if (best == -1) {
			int next_at = -1;
			for (int i = 0 ; i < n; ++i) {
				if (!p[i].completed) {
					if (next_at == -1 || p[i].arrival_time < next_at) {
						next_at = p[i].arrival_time;
					}
				}
			}

			time = next_at;
			continue;
		}

		p[best].start_time = time;
		p[best].waiting_time = p[best].start_time - p[best].arrival_time;
                p[best].response_time = p[best].waiting_time;
                p[best].completion_time = p[best].start_time + p[best].burst_time;
                p[best].turnaround_time = p[best].completion_time - p[best].arrival_time;
                p[best].completed = 1;

                g[num_segments++] = (Segment){ p[best].pid, p[best].start_time, p[best].completion_time };
                time = p[best].completion_time;
                ++done;
	}

        Process *printable = malloc(n * sizeof(Process));
        memcpy(printable, p, n * sizeof(Process));

        for (int i = 0; i < n; ++i) {
	    for (int j = i + 1; j < n; ++j) {
	        if (printable[j].pid < printable[i].pid) {
	           Process tmp = printable[i];
                   printable[i] = printable[j];
                 }
	    }
	}
      

	printf("=== Shortest Job First (SJF) ===\n");
	print(g, num_segments);
	print_table(printable, n);
	putchar('\n');

	free(printable);
	free(g);
	free(p);
}


int main(void) {
	int n;
	printf("Enter the number of processes: ");
	if (scanf("%d", &n) != 1 || n <= 0) {
		fprintf(stderr, "Invalid number of processes \n");
		return 1;
	}

	Process *procs = malloc(n * sizeof(Process));
	for (int i = 0; i < n; ++i) {
		procs[i].pid = i + 1;
		printf("Enter the arrival time and burst time for process %d: ",i + 1);
		if (scanf("%d %d", &procs[i].arrival_time, &procs[i].burst_time) != 2 || procs[i].burst_time < 0) {
			fprintf(stderr, "Invalid input for process %d \n", i + 1);
			free(procs);
			return 1;
		}
	}

	puts("");

	fcfs(procs, n);
	sjf(procs, n);

	free(procs);
	return 0;
}
