#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define MAX 100
#define MAX_PROCESS 100
int numberOfProcesses, totalCPUBurstTime, * arrivalTime, * CPUBurstTime, * processNumber, minimumArrivalTime, * priority;
float averageWaitingTime = 0, averageTurnAroundTime = 0;
struct processes
{
 char name;
 int at,bt,ct,wt,tt;
 int completed;
 float ntt;
}p[10];
int n;
typedef struct {
   int pid;
   int burst_time;
   int waiting_time;
   int turnaround_time;
}
Process;

struct times {
   int p, art, but, wtt, tat, rnt;
};

void roundRobin();
void sortart(struct times a[], int pro);
void print_table(Process p[], int n);
void print_gantt_chart(Process p[], int n);
void fcfs();
void ps();
void drawGanttChart();
void srt();
void sortByArrival();
void hrrn();
void sjf();

void main() {
   int choice1 = -1;
   while (choice1 != 0) {
      printf("\n-Preemptive Scheduling:");
      printf("\n[1] Round Robin");
      printf("\n[2] Shortest Remaining Time");
      printf("\n-Non-preemptive Scheduling:");
      printf("\n[3] First Come First Serve");
      printf("\n[4] Shortest Job First");
      printf("\n[5] Highest Response Ratio Next");
      printf("\n[6] Priority Based");
      printf("\n[0] Exit");
      printf("\n=> Enter your choice: ");
      scanf("%d", & choice1);
      if (choice1 == 1) {
         roundRobin();
      } else if (choice1 == 2) {
	  srt();
      } else if (choice1 == 3) {
         fcfs();
      } else if (choice1 == 4) {
	sjf();
      } else if (choice1 == 5) {
	hrrn();
      } else if (choice1 == 6) {
         ps();
      }
   }
}

//ROUND ROBIN
void roundRobin() {
   int i, j, pro, time, remain, flag = 0, ts;
   struct times a[100];
   float avgwt = 0, avgtt = 0;
   printf("Round Robin Scheduling Algorithm\n");
   printf("Note -\n1. Arrival Time of at least on process should be 0\n2. CPU should never be idle\n");
   printf("Enter Number Of Processes : ");
   scanf("%d", & pro);
   remain = pro;
   for (i = 0; i < pro; i++) {
      printf("Enter arrival time and Burst time for Process P%d : ", i);
      scanf("%d%d", & a[i].art, & a[i].but);
      a[i].p = i;
      a[i].rnt = a[i].but;
   }
   sortart(a, pro);
   printf("Enter Time Slice OR Quantum Number : ");
   scanf("%d", & ts);
   printf("\n***************************************\n");
   printf("Gantt Chart\n");
   printf("0");
   for (time = 0, i = 0; remain != 0;) {
      if (a[i].rnt <= ts && a[i].rnt > 0) {
         time = time + a[i].rnt;
         printf(" -> [P%d] <- %d", a[i].p, time);
         a[i].rnt = 0;
         flag = 1;
      } else if (a[i].rnt > 0) {
         a[i].rnt = a[i].rnt - ts;
         time = time + ts;
         printf(" -> [P%d] <- %d", a[i].p, time);
      }
      if (a[i].rnt == 0 && flag == 1) {
         remain--;
         a[i].tat = time - a[i].art;
         a[i].wtt = time - a[i].art - a[i].but;
         avgwt = avgwt + time - a[i].art - a[i].but;
         avgtt = avgtt + time - a[i].art;
         flag = 0;
      }
      if (i == pro - 1)
         i = 0;
      else if (a[i + 1].art <= time)
         i++;
      else
         i = 0;
   }
   printf("\n\n");
   printf("**********************************************************\n");
   printf("Process\tArrival Time\tBurst Time\tTAT\tWaiting Time\n");
   printf("**********************************************************\n");
   for (i = 0; i < pro; i++) {
      printf("P%d\t\t%d\t\t%d\t%d\t%d\n", a[i].p, a[i].art, a[i].but, a[i].tat, a[i].wtt);
   }
   printf("**********************************************************\n");
   avgwt = avgwt / pro;
   avgtt = avgtt / pro;
   printf("Average Waiting Time : %.2f\n", avgwt);
   printf("Average Turnaround Time : %.2f\n", avgtt);
}
void sortart(struct times a[], int pro) {
   int i, j;
   struct times temp;
   for (i = 0; i < pro; i++) {
      for (j = i + 1; j < pro; j++) {
         if (a[i].art > a[j].art) {
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
         }
      }
   }
   return;
}

//FCFS
void fcfs() {
   Process p[MAX];
   int i, j, n;
   int sum_waiting_time = 0, sum_turnaround_time;
   printf("Enter total number of process: ");
   scanf("%d", & n);
   printf("Enter burst time for each process:\n");
   for (i = 0; i < n; i++) {
      p[i].pid = i + 1;
      printf("P[%d] : ", i + 1);
      scanf("%d", & p[i].burst_time);
      p[i].waiting_time = p[i].turnaround_time = 0;
   }

   // calculate waiting time and turnaround time
   p[0].turnaround_time = p[0].burst_time;

   for (i = 1; i < n; i++) {
      p[i].waiting_time = p[i - 1].waiting_time + p[i - 1].burst_time;
      p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
   }

   // calculate sum of waiting time and sum of turnaround time
   for (i = 0; i < n; i++) {
      sum_waiting_time += p[i].waiting_time;
      sum_turnaround_time += p[i].turnaround_time;
   }

   // print table
   puts(""); // Empty line
   print_table(p, n);
   puts(""); // Empty Line
   printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
   printf("Average Waiting Time    : %-2.2lf\n", (double) sum_waiting_time / (double) n);
   printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
   printf("Average Turnaround Time : %-2.2lf\n", (double) sum_turnaround_time / (double) n);

   // print Gantt chart
   puts(""); // Empty line
   puts("          GANTT CHART          ");
   puts("          ***********          ");
   print_gantt_chart(p, n);
}
void print_table(Process p[], int n) {
   int i;

   puts("+-----+------------+--------------+-----------------+");
   puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
   puts("+-----+------------+--------------+-----------------+");

   for (i = 0; i < n; i++) {
      printf("| %2d  |     %2d     |      %2d      |        %2d       |\n", p[i].pid, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
      puts("+-----+------------+--------------+-----------------+");
   }

}
void print_gantt_chart(Process p[], int n) {
   int i, j;
   // print top bar
   printf(" ");
   for (i = 0; i < n; i++) {
      for (j = 0; j < p[i].burst_time; j++) printf("--");
      printf(" ");
   }
   printf("\n|");

   // printing process id in the middle
   for (i = 0; i < n; i++) {
      for (j = 0; j < p[i].burst_time - 1; j++) printf(" ");
      printf("P%d", p[i].pid);
      for (j = 0; j < p[i].burst_time - 1; j++) printf(" ");
      printf("|");
   }
   printf("\n ");
   // printing bottom bar
   for (i = 0; i < n; i++) {
      for (j = 0; j < p[i].burst_time; j++) printf("--");
      printf(" ");
   }
   printf("\n");

   // printing the time line
   printf("0");
   for (i = 0; i < n; i++) {
      for (j = 0; j < p[i].burst_time; j++) printf("  ");
      if (p[i].turnaround_time > 9) printf("\b"); // backspace : remove 1 space
      printf("%d", p[i].turnaround_time);

   }
   printf("\n");
}

// PRIORITY SCHEDULING
void ps() {
   int i, j, temp;

   printf("Enter the number of processes : ");
   scanf("%d", & numberOfProcesses);
   arrivalTime = (int * ) malloc(sizeof(int) * numberOfProcesses);
   CPUBurstTime = (int * ) malloc(sizeof(int) * numberOfProcesses);
   processNumber = (int * ) malloc(sizeof(int) * numberOfProcesses);
   priority = (int * ) malloc(sizeof(int) * numberOfProcesses);

   minimumArrivalTime = 2147483647;
   for (i = 0; i < numberOfProcesses; i++) {
      processNumber[i] = i;
      printf("\nEnter the data for process number : %d\n", i);
      printf("\n");
      printf("Arrival Time   : ");
      scanf("%d", & arrivalTime[i]);
      printf("CPU Burst time : ");
      scanf("%d", & CPUBurstTime[i]);
      printf("Priority : ");
      scanf("%d", & priority[i]);

      totalCPUBurstTime += CPUBurstTime[i];
      if (minimumArrivalTime > arrivalTime[i])
         minimumArrivalTime = arrivalTime[i];
   }

   /*
                   implementing bubble sort algorithm
   */

   for (i = 0; i < numberOfProcesses; i++) {
      for (j = 0; j < numberOfProcesses - i - 1; j++) {
         if (priority[j] > priority[j + 1]) {
            temp = priority[j];
            priority[j] = priority[j + 1];
            priority[j + 1] = temp;

            temp = arrivalTime[j];
            arrivalTime[j] = arrivalTime[j + 1];
            arrivalTime[j + 1] = temp;

            temp = CPUBurstTime[j];
            CPUBurstTime[j] = CPUBurstTime[j + 1];
            CPUBurstTime[j + 1] = temp;

            temp = processNumber[j];
            processNumber[j] = processNumber[j + 1];
            processNumber[j + 1] = temp;
         }
      }
   }

   for (i = 0; i < numberOfProcesses; i++) {
      printf("P%d -> %d,%d\n", processNumber[i], arrivalTime[i], CPUBurstTime[i]);
   }

   drawGanttChart();

}
void drawGanttChart() {
   const int maxWidth = 100;
   int scalingFactor, i, counter, tempi, currentTime;
   printf("The gantt chart for the given processes is : \n\n");

   scalingFactor = maxWidth / totalCPUBurstTime;
   for (i = 0; i < scalingFactor * totalCPUBurstTime + 2 + numberOfProcesses; i++) {
      printf("-");
   }
   printf("\n|");
   counter = 0, tempi = 0;
   for (i = 0; i < scalingFactor * totalCPUBurstTime; i++) {
      if (i == CPUBurstTime[counter] * scalingFactor + tempi) {
         counter++;
         tempi = i;
         printf("|");
      } else if (i == (CPUBurstTime[counter] * scalingFactor) / 2 + tempi) {
         printf("P%d", processNumber[counter]);
      } else {
         printf(" ");
      }

   }
   printf("|");
   printf("\n");
   for (i = 0; i < scalingFactor * totalCPUBurstTime + 2 + numberOfProcesses; i++) {
      printf("-");
   }
   printf("\n");

   /* printing the time labels of the gantt chart */
   counter = 0;
   tempi = 0;
   currentTime = minimumArrivalTime;
   printf("%d", currentTime);
   for (i = 0; i < scalingFactor * totalCPUBurstTime; i++) {
      if (i == CPUBurstTime[counter] * scalingFactor + tempi) {

         tempi = i;
         currentTime += CPUBurstTime[counter];
         averageWaitingTime += currentTime;
         counter++;
         printf("%2d", currentTime);
      } else {
         printf(" ");
      }
   }
   currentTime += CPUBurstTime[counter];

   printf("%2d\n\n", currentTime);
   averageWaitingTime = averageWaitingTime / numberOfProcesses;
   averageTurnAroundTime = averageWaitingTime + totalCPUBurstTime / numberOfProcesses;

   printf("Average waiting Time     : %f\n", averageWaitingTime);
   printf("Average Turn Around Time : %f\n", averageTurnAroundTime);
}

void srt()
{
	int n, ari[10], bur[10], total = 0, i, j, small, temp, procs[100], k, waiting[10], finish[10]; 
 
  float tavg = 0.0, wavg = 0.0; 
 
  printf("ENTER THE NUMBER OF PROCESSES:"); 
 
  scanf("%d", & n); 
 
  for (i = 0; i < n; i++) 
 
  { 
 
    printf("ENTER THE ARRIVAL TIME OF PROCESS %d:\t", i); 
 
    scanf("%d", & ari[i]); 
 
    printf("ENTER THE BURST TIME OF PROCESS %d:\t", i); 
 
    scanf("%d", & bur[i]); 
 
    waiting[i] = 0; 
 
    total += bur[i]; 
 
  } 
 
  for (i = 0; i < n; i++) 
 
  { 
 
    for (j = i + 1; j < n; j++) 
 
    { 
 
      if (ari[i] > ari[j]) 
 
      { 
 
        temp = ari[i]; 
 
        ari[i] = ari[j]; 
 
        ari[j] = temp; 
 
        temp = bur[i]; 
 
        bur[i] = bur[j]; 
 
        bur[j] = temp; 
 
      } 
 
    } 
 
  } 
 
  for (i = 0; i < total; i++) 
 
  { 
 
    small = 3200; 
 
    for (j = 0; j < n; j++) 
 
    { 
 
      if ((bur[j] != 0) && (ari[j] <= i) && (bur[j] < small)) 
 
      { 
 
        small = bur[j]; 
        k = j; 
 
      } 
 
    } 
 
    bur[k]--; 
 
    procs[i] = k; 
 
  } 
 
  k = 0; 
 
  for (i = 0; i < total; i++) 
 
  { 
 
    for (j = 0; j < n; j++) 
 
    { 
 
      if (procs[i] == j) 
 
      { 
 
        finish[j] = i; 
 
        waiting[j]++; 
 
      } 
 
    } 
 
  } 
 
  for (i = 0; i < n; i++) 
 
  { 
 
    printf("\n PROCESS %d:-FINISH TIME==> %d TURNAROUND TIME==>%d WAITING TIME==>%d\n", i + 1, finish[i] + 1, (finish[i] - ari[i]) + 1, (((finish[i] + 1) - waiting[i]) - ari[i])); 
 
    wavg = wavg + (((finish[i] + 1) - waiting[i]) - ari[i]); 
 
    tavg = tavg + ((finish[i] - ari[i]) + 1); 
 
  } 
  
 
  printf("\n WAvG==>\t%f\n TAVG==>\t%f\n", (wavg / n), (tavg / n)); 
}

void sortByArrival()
{
struct processes temp;
int i,j;
for(i=0;i<n-1;i++)
for(j=i+1;j<n;j++)
{
 if(p[i].at>p[j].at)
 {
  temp=p[i];
  p[i]=p[j];
  p[j]=temp;
 }
}
}
//HRRN
void hrrn()
{
	int i,j,time,sum_bt=0;
 char c;
        float avgwt=0;
 printf("Enter no of processes:");
 scanf("%d",&n);
 for(i=0,c='A';i<n;i++,c++)
 {
 p[i].name=c;
 printf("\nEnter the arrival time and burst time of process%c: ",p[i].name);
 scanf("%d%d",&p[i].at,&p[i].bt);
 p[i].completed=0;
 sum_bt+=p[i].bt;

}
sortByArrival();

printf("\nName\tArrival Time\tBurst Time\tWaiting Time\tTurnAround Time\t Normalized TT");
  for(time=p[0].at;time<sum_bt;)
  {
 
   float hrr=-9999;
   int loc;
  for(i=0;i<n;i++)
  {
 
   if(p[i].at<=time && p[i].completed!=1)
            {
              float temp=(p[i].bt + (time-p[i].at))/p[i].bt;
              if(hrr < temp)
               {
                hrr=temp;
                loc=i;
               }
         
   }
   }
 
 
   time+=p[loc].bt;
   p[loc].wt=time-p[loc].at-p[loc].bt;
   p[loc].tt=time-p[loc].at;
   p[loc].ntt=((float)p[loc].tt/p[loc].bt);
   p[loc].completed=1;
   avgwt+=p[loc].wt;
printf("\n%c\t\t%d\t\t%d\t\t%d\t\t%d\t\t%f",p[loc].name,p[loc].at,p[loc].bt,p[loc].wt,p[loc].tt,p[loc].ntt);
  }
  

printf("\nAverage waiting time:%f\n",avgwt/n);	
}
void sjf()
{
	int bt[20],p[20],wt[20],tat[20],i,j,n,total=0,pos,temp;
    float avg_wt,avg_tat;
    printf("Enter number of process:");
    scanf("%d",&n);
 
    printf("\nEnter Burst Time:\n");
    for(i=0;i<n;i++)
    {
        printf("p%d:",i+1);
        scanf("%d",&bt[i]);
        p[i]=i+1;           //contains process number
    }
 
    //sorting burst time in ascending order using selection sort
    for(i=0;i<n;i++)
    {
        pos=i;
        for(j=i+1;j<n;j++)
        {
            if(bt[j]<bt[pos])
                pos=j;
        }
 
        temp=bt[i];
        bt[i]=bt[pos];
        bt[pos]=temp;
 
        temp=p[i];
        p[i]=p[pos];
        p[pos]=temp;
    }
 
    wt[0]=0;            //waiting time for first process will be zero
 
    //calculate waiting time
    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++)
            wt[i]+=bt[j];
 
        total+=wt[i];
    }
 
    avg_wt=(float)total/n;      //average waiting time
    total=0;
 
    printf("\nProcess\t    Burst Time    \tWaiting Time\tTurnaround Time");
    for(i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];     //calculate turnaround time
        total+=tat[i];
        printf("\np%d\t\t  %d\t\t    %d\t\t\t%d",p[i],bt[i],wt[i],tat[i]);
    }
 
    avg_tat=(float)total/n;     //average turnaround time
    printf("\n\nAverage Waiting Time=%f",avg_wt);
    printf("\nAverage Turnaround Time=%f\n",avg_tat);
}
