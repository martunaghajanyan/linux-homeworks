#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char** argv)
{
    

    if(argc < 2)
    {
        std::cerr << "Please provide file path"<<std::endl;
        exit(1);
    }
    char* file = argv[1];
    int sd = open(file, O_RDONLY);
    if(sd < 0)
    {
        std::cerr << strerror(errno)<<std::endl; 
        exit(errno);  
    }
    char array[SIZE + 1];


    while(true)
    {
        int read_bytes = read(sd, array, SIZE);
        if(read_bytes < 0)
        {
            std::cerr << strerror(errno)<<std::endl; 
            exit(errno);       
        }
        if(read_bytes == 0)
        {
            break;
        }

        array[read_bytes] = '\0';
        std::cout<<array;

    }
    std::cout<<std::endl;

    int close_sd = close(sd);
    if (close_sd < 0)
    {
        std::cerr << strerror(errno)<<std::endl;
    }

    return 0;
}