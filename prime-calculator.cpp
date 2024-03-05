#include <iostream>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

bool isPrime(std::size_t n)
{
    for(int i = 2; i * i <= n; ++i)
    {
        if(n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int m_th_prime_number(int m)
{
    int index = 1;
    while(m != 0)
    {
        ++index;
        if(isPrime(index))
        {
            --m;
        }
    }
    return index;
}

int main()
{
    int pipe_1[2];
    int pipe_2[2];

    int res_pipe_1 = pipe(pipe_1);
    if(res_pipe_1 < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    int res_pipe_2 = pipe(pipe_2);
    if(res_pipe_2 < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    int pid = fork();

    if(pid < 0)
    {
        std::cerr << std::endl;
        exit(errno);
    }
    if(pid > 0)
    {
        close(pipe_1[0]);
        close(pipe_2[1]);

        std::string m;

        while(true)
        {
            std::cout << "[Parent] Please Enter the number " << std::endl;
            std::cin >> m;

        if(m == "exit")
        {
            return 0;
        }

        int n = stoi(m);

        std::cout << "[Parant] Sending" << m << "to child process..." << std::endl;

        int  res_write = write(pipe_1[1], &n, sizeof(int));
        if(res_write < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }

        std::cout << "[Parant] Waiting for the response from the child process.." << std::endl;
        int number;
        int res_read = read(pipe_2[0], &number, sizeof(int));
        if(res_read < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }

        std::cout << "[Parant] Recived calculation result of prime("   << m << ") = " 
                  << number << std::endl;
        }
    }

        if(pid == 0)
        {
            close(pipe_1[1]);
            close(pipe_2[0]);

            int m;
            while(true)
            {
                int res_read = read(pipe_1[0], &m, sizeof(int));
                if(res_read < 0)
                {
                    std::cerr << strerror(errno) << std::endl;
                    exit(errno);
                }

                std::cout << "[Child] calculating " << m << "-th prime number ..." << std::endl;

                int number = m_th_prime_number(m);

                std::cout << "[Child] Sending calculating result of prime("<< m << ")..."<< std::endl;

                int res_write = write(pipe_2[1], &number, sizeof(int));
                if(res_write < 0)
                {
                    std::cerr << strerror(errno) << std::endl;
                    exit(errno);
                }

            }

        }

    return 0;
}
