/* Os project #1
 * Simulation of Various CPU Queing algorithms
 * A code by Tareq Kirresh (#1142828) and Mahmoud Abufarha (-#1131014) 
 * Note:this code was made with Unix Systems in Mind , as it uses command line launch arguments
 * بسم الله الرحمن الرحيم  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define Q 3//the time quantum
#define clear() printf("\033[H\033[J")//clear the screen


typedef struct
{
    int pid;
    int priority;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;



void EP_schedueling(Process [],int );
void SJF_schedueling(Process [],int );
void RR_schedueling(Process [],int);
void SRTF_schedueling(Process[],int);
void scan_file(Process [],FILE * ,int *);
void print_gantt_chart(Process [], int );
void print_table(Process [], int );
void calc_and_print(Process [],int );






int main(int argc, char* argv[]){
/* The main function in this program simply calls the functions and CLI, it does not do any queueing.*/	
Process p[MAX];
int i=MAX;
int* n=&i;//the number of processes
FILE* fp;
char buff[80];

if(argc>1)//if we want to use launch arguments
{
		fp=fopen(argv[2],"r");
		scan_file(p,fp,n);
		
		if(strcmp(argv[1],"RR")==0){
			printf("Simulating RR schedueling on file %s \npress enter to continue \n",argv[2]);
			getchar();
			RR_schedueling(p,i);
			
			exit(0);
		}
		else if(strcmp(argv[1],"SJF")==0)
		{
			printf("Simulating SJF schedueling on file %s \npress enter to continue \n",argv[2]);
			getchar();
			SJF_schedueling(p,i);
			exit(0);	
		}
		else if(strcmp(argv[1],"SRTF")==0)
		{
			printf("Simulating SRTF schedueling on file %s \npress enter to continue \n",argv[2]);
			getchar();
			SRTF_schedueling(p,i);
			exit(0);
		}
		else if(strcmp(argv[1],"EP")==0)
		{
			printf("Simulating EP schedueling on file %s \npress enter to continue \n",argv[2]);
			getchar();
			EP_schedueling(p,i);
			exit(0);
		}
		else
		{
			printf("invalid arguments, try again\n");
			exit(1); 
		}
		
}

else{//if we want to execute the program normally.
	
	printf("\n\n\n\n\n\n\n\tWelcome to the Proccess Schedueling Simulator-inator 9000!\n\n\n\t\t\tpress any key to continue");
	getchar();
	label2:clear();
	printf("please enter the file you would like to extract process info from:\n");
	scanf("%s",buff);
	fp=fopen(buff,"r");
	if(fp==NULL){
		printf("File does not exist , press any key to try again");
		getchar();
		getchar();
		goto label2;//we could have used a while loop here , but we didnt 
	}
	scan_file(p,fp,n);
	label:clear();
	printf("Bellow are the selection of queues you can Simulate\n");
	printf("1-Explicit Priority\n2-Round Robin\n3-Shortest Job First\n4-Shortest Remianing Time First\n\n\nPlease enter the number you want to simulate or q to exit:");
	getchar();
	int j=getchar();
	j=j-48;//converting from ASCII to decimal
	switch(j){
		case 1:
			printf("Simulating EP schedueling on file %s \npress enter to continue",buff);
			getchar();
			getchar();
			EP_schedueling(p,i);
			break;
		case 2:
		    printf("Simulating RR schedueling on file %s \npress enter to continue",buff);
			getchar();
			getchar();
			RR_schedueling(p,i);
			break;	
			
		case 3:		    
			printf("Simulating SJF schedueling on file %s \npress enter to continue",buff);
			getchar();
			getchar();
			SJF_schedueling(p,i);
			break;	
		case 4:
			printf("Simulating SRTF schedueling on file %s \npress enter to continue",buff);
			getchar();
			getchar();
			SRTF_schedueling(p,i);
			break;
		case 65://The ASCII value of q - 48
			exit(0);
			break;
		default:
				goto label;//we do this here to avoid making a menu function and using the stack or using a while loop and complicating the code;it makes life easier 
				break;
				
				}
}
return 0;


}

void scan_file(Process p[],FILE * fp,int *size)//scans from a file containing process information
{
	int i=0;
	
	while(fscanf(fp,"%d,%d,%d,%d",&p[i].pid,&p[i].arrival_time,&p[i].burst_time,&p[i].priority)!=EOF)//each line in the file contains 4 values separated by commas.
	{
		i++;
	}
	
	*size=i;
}


void EP_schedueling(Process p[],int n)//excplicit priority schedueling 
{


	
    int endTime,i=0,smallest;
	Process q[MAX];//a queue of processes
	int q_len=0;//length of the queue
	q[q_len].pid=-1;

    int remain=0,time,sum_wait=0,sum_turnaround=0;

    for(i=0;i<n;i++)
    {
        p[i].remaining_time=p[i].burst_time;//remaining time for a process is its burst time
    }
    printf("\n\nProcess\t|Turnaround Time| Waiting Time|Priority|Arrival Time\n\n");
    p[MAX-1].priority=99999;//we put this particular line so that we dont end up with an infinite loop
    	int min=p[0].arrival_time;
	for(i=0;i<n;i++)
	{
		if(p[i].arrival_time<min)
		{
			min=p[i].arrival_time;
		}
	}
	time=min;//we start calculating turnaround time from the time the first process arrives
    for(time=min;remain!=n;time++)
    {
        smallest=MAX-1;
      
        for(i=0;i<n;i++)
        {
            if(p[i].arrival_time<=time && p[i].priority<p[smallest].priority && p[i].remaining_time>0)
            {
				
                smallest=i;
               
            }
     
        }
        
        if(q[q_len].pid!=p[smallest].pid &&smallest!=99){
			q_len++;
			q[q_len-1]=p[smallest];
			q[q_len-1].burst_time=0;
			q[q_len-1].turnaround_time=time-min+1;
		}
		q[q_len-1].burst_time++;
        p[smallest].remaining_time--;
        
        if(p[smallest].remaining_time==0)//if a process has finished all its burst time , and is finished
        {
         
            remain++;//count it as finished 
            endTime=time+1;//the time it ended at is the current moment+1
            printf("\nP[%d]\t|\t%d\t|\t%d\t|\t%d|\t%d|",p[smallest].pid,endTime-p[smallest].arrival_time,endTime-p[smallest].burst_time-p[smallest].arrival_time,p[smallest].priority,p[smallest].arrival_time);//some info about the process
            sum_wait+=endTime-p[smallest].burst_time-p[smallest].arrival_time;//total wait time
            sum_turnaround+=endTime-p[smallest].arrival_time;//total turnaround time
        }
    }
    printf("\n\nAverage waiting time = %f\n",sum_wait*1.0/n);//calculating the avg wait time
    printf("Average Turnaround time = %f\n",sum_turnaround*1.0/5);//calculating the avg turnaround time
    printf("\t GANTT CHART \n \t****************\n");
    
 
	print_gantt_chart(q,q_len);//printing the gantt chart
		
	
	
}


void SJF_schedueling(Process p[],int n)//shortest job first schedueling
{

	int i,j;

	Process temp;
	for(i=0;i<(n-1);i++)//sort the processes based on arrival time. we do this so we can print them later
	{
		for(j=0;j<(n-i-1);j++)
		{
			if(p[j].arrival_time>p[j+1].arrival_time)
			{
				temp=p[j];
				p[j]=p[j+1];
				p[j+1]=temp;
			}
		}
	}
	
		for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(p[i].arrival_time==p[j].arrival_time && p[i].burst_time<p[j].burst_time)//then we sort the processes having the same arrival time by burst time
			{
				temp=p[j];
				p[j]=p[i];
				p[i]=temp;
			}
		}
	}
	calc_and_print(p,n);
	
	
}

void RR_schedueling(Process p[],int n)
{
	int endTime,i=0;
	Process q[MAX];
	int q_len=0;
	

    int remain=0,time,sum_wait=0,sum_turnaround=0;

    for(i=0;i<n;i++)
    {
        p[i].remaining_time=p[i].burst_time;
    }
    printf("\n\nProcess\t|Turnaround Time|Waiting Time|Arrival Time\n\n");
	int min=p[0].arrival_time;
	for(i=0;i<n;i++)
	{
		if(p[i].arrival_time<min)
		{
			min=p[i].arrival_time;
		}
	}
	time=min;//again here , we do this so we start calculating the turnaround time from the moment the first processes arrives
	q[0].pid=-9999;
      while(remain!=n){
        for(i=0;i<n;i++)
        {
            if(p[i].arrival_time<=time && p[i].remaining_time > Q&&p[i].pid!=q[q_len-1].pid)//if a process has more than one quantum's worth of time left
            {
					
					q[q_len]=p[i];//then place it in the queue 
					q[q_len].burst_time=Q;//with burst time of one quantum
					time+=Q;//increment the total time by one Quantum
					q[q_len].turnaround_time=time-min;//the turnaround time is calculated based on the first process that arrived
					p[i].remaining_time=p[i].remaining_time-Q;//decrease the remaining time of the process by one quantum
					q_len++;//increment the queue length
					
				}	
					if(p[i].remaining_time<=Q&&p[i].pid!=q[q_len-1].pid)//if the process has one or less quantum's worth of time left
					{
						q[q_len]=p[i];//place it in the queue
						q[q_len].burst_time=p[i].remaining_time;//place what is left of the time as the brust time
						time+=p[i].remaining_time;//add what is left of the remaining time to the total time
						q[q_len].turnaround_time=time-min;//again , the turnaround time is calculated based on the first process that arrived
						p[i].remaining_time=0;//the processes is now complete ; it has no remaining time
						remain++;//increment the ammount of finished processes
						endTime=time+1;//this processes ended at total time +1
						
						printf("\nP[%d]\t|\t%d\t|\t%d\t|\t%d|",p[i].pid,endTime-p[i].arrival_time,endTime-p[i].burst_time-p[i].arrival_time,p[i].arrival_time);//print some info
						sum_wait+=endTime-p[i].burst_time-p[i].arrival_time;//calculate the total wait time
						sum_turnaround+=endTime-p[i].arrival_time;//calculate the total turnaround time
						q_len++;//increase the length of the queue
					}
				
			}
		
		}
	
	
        
		
		
        
        
    printf("\n\nAverage waiting time = %f\n",sum_wait*1.0/n);//calculate and pring that avg waiting time
    printf("Average Turnaround time = %f\n",sum_turnaround*1.0/5);//calculate and print the avg turnaround time
    printf("\t GANTT CHART \n \t****************\n");
    
 
	print_gantt_chart(q,q_len);//print the gantt chart


}
void SRTF_schedueling(Process p[],int n)//Shortest Remaining Time First Schedueling
{

    int endTime,i=0,smallest;
	Process q[MAX];
	int q_len=0;
	q[q_len].pid=-1;

    int remain=0,time,sum_wait=0,sum_turnaround=0;

    for(i=0;i<n;i++)
    {
        p[i].remaining_time=p[i].burst_time;
    }
    printf("\n\nProcess\t|Turnaround Time|Waiting Time|Arrival Time\n\n");
    p[MAX-1].remaining_time=999999999;//this is done as above to prevent infinite loops
    int min=p[0].arrival_time;//finding the first proccess's arrival time
	for(i=0;i<n;i++)
	{
		if(p[i].arrival_time<min)
		{
			min=p[i].arrival_time;
		}
	}
	
    for(time=min;remain!=n;time++)//we start from the arrival time of the first processes and while all processes have not finished , we increment it
    {
        smallest=MAX-1;//assume a value is the smallest
      
        for(i=0;i<n;i++)
        {
            if(p[i].arrival_time<=time && p[i].remaining_time<p[smallest].remaining_time && p[i].remaining_time>0)//find the smallest value
            {
				
                smallest=i;
               
            }
     
        }
        
        if(q[q_len].pid!=p[smallest].pid &&smallest!=99){//if the smallest processes is not the same one as the last 
			q_len++;//increase the size of the queue
			q[q_len-1]=p[smallest];//place the smallest process in the queue
			q[q_len-1].burst_time=0;//start it at burst time 0
			q[q_len-1].turnaround_time=time-min+1;//with turnaround time relative to the first processes to arrive
		}
		q[q_len-1].burst_time++;//increment the burst time
        p[smallest].remaining_time--;//decrement the remaining time
        
        if(p[smallest].remaining_time==0)//if the process has finished running
        {
         
            remain++;//increment the amount of finished processes
            endTime=time+1;//the end time of this processs is the current time+1
            printf("\nP[%d]\t|\t%d\t|\t%d\t|\t%d|",p[smallest].pid,endTime-p[smallest].arrival_time,endTime-p[smallest].burst_time-p[smallest].arrival_time,p[smallest].arrival_time);//print some info
            sum_wait+=endTime-p[smallest].burst_time-p[smallest].arrival_time;//calculate the total wait time
            sum_turnaround+=endTime-p[smallest].arrival_time;//calculate the total turnaround time
        }
    }
    printf("\n\nAverage waiting time = %f\n",sum_wait*1.0/n);//calculate and print the total wait time
    printf("Average Turnaround time = %f\n",sum_turnaround*1.0/5);//calculate and print the total turnaround time
    printf("\t GANTT CHART \n \t****************\n");
    
 
	print_gantt_chart(q,q_len);//print the gantt chart
}

void calc_and_print(Process p[],int n)
{
	
	/* this function can be considered a "legacy" sort of function ; it is only here to serve the SJF function , because we went about creating
	 * the functions in this project in different ways and ended up picking the best , SJF stayed this way , EP,SRTF, and RR were changed, 
	 * the difference being is that the turnaround time for SJF is calculated after arranging the processes in the queue , for the rest of the queues
	 * they are calculated During placing them in the queue*/
	
	int sum_waiting_time = 0, sum_turnaround_time=0;
	int i;
	 // calculate waiting time and turnaround time
    p[0].turnaround_time = p[0].burst_time+p[0].arrival_time;

    for(i=1; i<n; i++) {
        p[i].waiting_time = p[i-1].waiting_time + p[i-1].burst_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
    }

    // calculate sum of waiting time and sum of turnaround time
    for(i=0; i<n; i++) {
            sum_waiting_time += p[i].waiting_time;
            sum_turnaround_time += p[i].turnaround_time;
    }

    // print table
    puts(""); // Empty line
    print_table(p, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double)n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);

    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
    print_gantt_chart(p, n);
	
}
	

void print_table(Process p[], int n)//prints the chart calculated in the function calc_and_print
{
    int i;

    puts("+-----+------------+--------------+-----------------+");
    puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
    puts("+-----+------------+--------------+-----------------+");

    for(i=0; i<n; i++) {
        printf("| %2d  |     %2d     |      %2d      |        %2d       |\n"
               , p[i].pid, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time );
        puts("+-----+------------+--------------+-----------------+");
	}
    

}


void print_gantt_chart(Process p[], int n)//printing the gantt chart using an array of processes
{
    int i, j;
    // print top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time; j++) printf("--");
        printf("  ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time - 1; j++) printf(" ");
    if(p[i].pid==-1){
		printf(" w ");
	}
	else{
        printf("P%d", p[i].pid);
	}
        for(j=0; j<p[i].burst_time - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].burst_time; j++) printf("--");
        printf("  ");
    }
    printf("\n");


    // printing the time line
    printf("0");
    for(i=0 ;i<n; i++) {
        for(j=0; j<p[i].burst_time; j++) printf("  ");
        if(p[i].turnaround_time > 9) printf("\b"); // backspace : remove 1 space
        printf("%d|", p[i].turnaround_time);

    }
    printf("\n");

} 



