#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <sys/times.h>


void do_command(char** argv) 
{

    struct tms start,end;
    clock_t start_real, 
    end_real;

    start_real = times(&start);

    pid_t pid;
    int status;
    pid = fork();

    if (pid == -1) {

        std::cerr << "NO" << std::endl;
        exit(1);

    }
    else if (pid == 0) 
    {

        int res;
        res = execvp(argv[0], argv);

        if (res == -1) 
        {
            std::cerr << "NO!" << strerror(errno) << std::endl;
            exit(errno);
        }

    }
    else 
    {

        waitid(pid, &status, 0);
        times(&end);
        end_real = times(&end);

        clock_t ticks_second = sysconf(_SC_CLK_TCK);
        double duration = (double)(end_real - start_real) / ticks_second;

        std::cout << "Command complited with "
            << WEXITSTATUS(status)
            << " exit code and took "
            << duration << " seconds."
            << std::endl;
    }

}

int main(int argc, char* argv) 
{

    if (argc < 2) 
    {
        std::cerr << "Please provide the command." << std::endl;
        return 1;
    }

    char** command_args = new char* [argc];
    for (int i = 1; i < argc; i++) {
        command_args[i - 1] = argv[i];
    }
    command_args[argc - 1] = nullptr;

    do_command(command_args);

    delete[] command_args;
    return 0;
}