#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void child_process(int n) {
  usleep(5000);
  for (int idx = 1; idx <= n; ++idx) {
    printf("Child [%d]: %d\n", getpid(), idx);
    sleep(1);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    printf(
        "Usage: ./sched <time-slice> <c1-execution-time> "
        "<c2-execution-time>\n");
    exit(1);
  }

  int num_children = 2;
  int time_slice = atoi(argv[1]);
  int children_exec_time[2];
  for (int idx = 2; idx < num_children + 2; ++idx) {
    children_exec_time[idx - 2] = atoi(argv[idx]);
  }

  printf("timeslice: %d, c1 execution time: %d, c2 execution time: %d\n",
         time_slice, children_exec_time[0], children_exec_time[1]);

  // ******************************
  //    YOUR SOLUTION STARTS HERE
  // ******************************
  int cpid1 = fork();
  if (cpid1 == 0) {
    child_process(children_exec_time[0]);
    return 0;
  }
  int cpid2 = fork();
  if (cpid2 == 0) {
    child_process(children_exec_time[1]);
    return 0;
  } 

  kill(cpid1,SIGSTOP);
  kill(cpid2,SIGSTOP);

  //schedule process
  int status[2];
  status[0] = 0;
  status[1] = 0;

  while (status[0] != -1 && status[1] != -1) {
    if (status[0] == 0 && status[1] == 0) {
      status[0] = 1;
      kill(cpid1,SIGCONT);
    }
    else if (status[0] == 1 && status[1] == 0) {
      status[1] = 1;
      if (waitpid(cpid1,NULL,WNOHANG) == 0) {
        status[0] = 0;
        kill(cpid2,SIGSTOP);
      }
      else {
        status[0] = -1;
      }
      kill(cpid2,SIGCONT);
    }
    else if (status[0] == 0 && status[0] == 1) {
      status[0] = 1;
      if (waitpid(cpid2,NULL,WNOHANG) == 0) {
        status[1] = 0;
        kill(cpid2,SIGSTOP);
      }
      else {
        status[1] = -1;
      }
      kill(cpid1,SIGCONT);
    }
    else if (status[0] == 1 && status[0] == -1) {
      waitpid(cpid1,NULL,NULL);
      break;
    }
    else if (status[0] == -1 && status[0] == 0) {
      waitpid(cpid2,NULL,NULL);
      break;
    }
    sleep(time_slice);
  }

 
  // ******************************
  //    YOUR SOLUTION ENDS HERE
  // ******************************

  printf("Parent [%d] terminates!!\n", getpid());
}
