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
    

    if(argc < 3)
    {
        std::cerr << "Input file path"<<std::endl;
        exit(1);
    }

    char* copy = argv[1];

    int fd = open(copy, O_RDONLY);
    if(fd < 0)
    {
        std::cerr << strerror(errno)<<std::endl; 
        exit(errno);  
    }
    char array[SIZE + 1];

    char* copy_2 = argv[2];

    int fd_2 = open(copy_2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_2 < 0)
    {
        std::cerr << strerror(errno)<<std::endl; 
        exit(errno);  
    }
   
    while(true)
    {
        ssize_t read_bytes = read(fd, array, SIZE);

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
        
        int write_bytes = write(fd_2, array, read_bytes);
        if(write_bytes < 0)
        {
            std::cerr << strerror(errno)<<std::endl; 
            exit(errno);       
        }

    }
    int close_fd = close(fd);
    if (close_fd < 0)
    {
        std::cerr << strerror(errno)<<std::endl;
    }

    return 0;
}