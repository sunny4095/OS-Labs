#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int main (int argc, char* argv[]) {
    int n;
    scanf("%d", &n);
    int cpid = 0;
    for (int i=0; i<n; i++) {
        cpid = fork();
        if (cpid == 0) {
            //child process
            printf("%d\n", (int) getpid());
            exit(0);
        }
    }
    for (int i=0; i<n; i++) {
        wait(NULL);
    }
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// int main() {
//     int N;
//     printf("Enter the number of fork calls (N): ");
//     scanf("%d", &N);

//     if (N <= 0) {
//         printf("Invalid input. N must be greater than 0.\n");
//         return 1;
//     }

//     int cpid = 0;
//     for (int i = 0; i < N; i++) {
//         cpid = fork();
//         if (cpid == 0) {
//             // Child process
//             printf("Child process with PID %d created.\n", getpid());
//             exit(0); // Ensure child process exits immediately
//         } else if (cpid < 0) {
//             perror("fork");
//             exit(1);
//         }
//     }

//     // Parent process: reap all child processes
//     for (int i = 0; i < (1 << N) - 1; i++) {
//         wait(NULL);
//     }

//     printf("All child processes have been reaped.\n");
//     return 0;
// }
