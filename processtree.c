#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

pid_t fork_process();

void print_info_child();

void print_create_process(pid_t pid);

int waiting_processes();

int main() 
{
    int i = 0;
    pid_t pid;

    printf("Process main() PID = %d (parent %d)\n\n",
            getpid(),
            getppid());

    pid = fork_process();

    if (0 == pid)
    {
        print_info_child();

        pid = fork_process();

        if (0 == pid)
        {
            print_info_child();
            sleep(1);
        }
        else
        {
            print_create_process(pid);

            pid = fork_process();

            if(0 == pid)
            {
                print_info_child();
                sleep(1);
            }
            else
            {
                print_create_process(pid);
            }
        }
    }
    else
    {
        print_create_process(pid);

        pid = fork_process();
        i++;
        
        if (0 == pid)
        {
            print_info_child();

            pid = fork_process();
            
            if(0 == pid)
            {
                print_info_child();
                sleep(1);
            }
            else
            {
                print_create_process(pid);
            }
        }
        else
        {
            print_create_process(pid);
        }

    }

    return waiting_processes();
}

pid_t fork_process()
{
    pid_t pid;

    pid = fork();
    if (-1 == pid)
    {
        perror("Error in fork()");
        exit(EXIT_FAILURE);
    }

    return pid;
}

void print_info_child()
{
    printf("----Child process PID = %d (parent %d)\n",
            getpid(),
            getppid());
}

void print_create_process(pid_t pid)
{
    printf("Process PID = %d create child %d\n",
            getpid(),
            pid);
}

int waiting_processes()
{
    int wstatus;
    int ret;
    int i = 0;

    while ((ret = waitpid(-1, &wstatus, 0)) > 0)
    {
	if (WIFEXITED(wstatus))
        {
	    printf("Child %d exited (%d), status = %d\n",
                    ret,
                    i,
                    WEXITSTATUS(wstatus));
	}
	else
        {
	    printf("Child %d interrupted or ended for another reason (%d)\n",
                    ret,
                    i);
        }
        i++;
    }

    if (ECHILD != errno)
    {
        perror("Error waitpid(...)");
        exit(EXIT_FAILURE);
    }

    return 0;
}
