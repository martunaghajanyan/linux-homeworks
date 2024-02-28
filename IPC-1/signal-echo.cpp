#include <iostream>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>


void sigusr1_handler(int signum, siginfo_t* info, void* context)
{
    pid_t pid = info->si_pid;
    uid_t uid = info->si_uid;
    struct passwd* pwd = getpwuid(uid);

    const char* username = pwd->pw_name;

    ucontext_t* _context = (ucontext_t*) context;

    unsigned long eip = _context->uc_mcontext.gregs[REG_RIP];
    unsigned long eax = _context->uc_mcontext.gregs[REG_RAX];
    unsigned long ebx = _context->uc_mcontext.gregs[REG_RBX];

    std::cout << "Recive a signal SIGURSR1 signal from " << pid
              << "executed by " << uid << "( " << username << " )" << std::endl;

    std::cout << "State of the context: EIP = " << eip << "EAX = " << eax <<"EBX = " << ebx;


}

int main()
{
    std::cout << getpid() << std::endl;

    struct sigaction action;
    memset(&action, 0 , sizeof(action));

    action.sa_sigaction = sigusr1_handler;
    action.sa_flags = SA_SIGINFO;

    int sigaction_result = sigaction(SIGUSR1, &action, NULL);
    if(sigaction_result < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    while(true)
    {
        sleep(10);
    }
    
    
    return 0;
}