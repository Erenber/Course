#include<signal.h>
#include"Controller.h"

int* getFlags(int argc, char* argv[], int* flags);
void handler(int sig)
{
    system("stty echo");
    printf("%c[?25h\n", 0x1B);
    
    if (sig == SIGINT)
    {
        system("clear");
    }
    exit(0);
}

void* isValidConsoleSize(void*);

// 2  3  3       3  3 
//-h -c -C(0-8) -g -s(0 - 5) -error
int main(int argc, char* argv[])
{
    int flags[6] = {};
    printf("%c[?25l", 0x1B);
    printf("%c[=2h", 0x1B);
    getFlags(argc, argv, flags);
     
    system("stty -echo");
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, 0) == -1)
    {
        fprintf(stdout, "Sigaction error");
        exit(errno);
    }
    if (sigaction(SIGUSR1, &action, 0) == -1)
    {
        fprintf(stdout, "Sigaction error");
        exit(errno);
    }

    pthread_t termnalSizeThread;
    pthread_create(&termnalSizeThread, NULL, isValidConsoleSize, NULL);

    Controller controller(flags);
    controller.start();
    
    system("stty echo");
    printf("%c[?25h\n", 0x1B);

    return 0;
}

int* getFlags(int argc, char* argv[], int* flags)
{
    if (argc == 1) flags[0] = 1;
    if (argc > 3) flags[5] = 1;
    for (int i = 1; i < argc; i++)
    {
        std::string tmp = argv[i];
        if (tmp == "-h")
        { 
            flags[0] = 1;
            continue;
        }
        if (tmp == "-c") 
        {
            flags[1] = 1;
            continue;
        }
        if (tmp[0] == '-' && tmp[1] == 'C')
        { 
            if (tmp.length() > 3) flags[5] = 1;
            if (tmp.length() == 2){
                 flags[2] = 1;
                 continue;
            }
            flags[2] = tmp[2] - '0';
            if (flags[2] < 0 || flags[2] > 8) flags[5] = 1;
            continue;
        }
        if (tmp == "-g") 
        {
            flags[3] = 1;
            continue;
        }
        if (tmp[0] == '-' && tmp[1] == 's')
        {
            if (tmp.length() > 3) flags[5] = 1;
            flags[4] = tmp[2] - '0';
            if (flags[4] < 0 || flags[4] > 5) flags[5] = 1;
            continue;
        }
        flags[5] = 1; //none of possible variants
    }
    //-h -c -C -g -s(0 - 5) -error
    // 0  0  0  0  0         0
    if (flags[0] && (flags[1] || flags[2] || flags[3] || flags[4])) flags[5] == 1; //something with -help
    if (flags[1] && (flags[2] || flags[3])) flags[5] = 1;//some mods with -cpu
    if (flags[2] && flags[3]) flags[5] = 1;//some mods with -cpu(+load)
    if (flags[4] && !flags[1] && !flags[2] && !flags[3])flags [5] = 1;//speed without mode
    return flags;
}

//Function that every 0.2 sec checks termianl size
void* isValidConsoleSize(void*)
{
    struct timespec tw = {0, 200000000}; //nanosecs equal to 0,2 sec;
    struct timespec tr;

    while(1){

        FILE* consoleInfo;

        if (!(consoleInfo = popen("stty size", "r")))
        {   
            pclose(consoleInfo);
            system("clear");
            goto(0,0);
            std::cout << "Cant't get termianl size\n";
            raise(SIGUSR1);
        }
        char Buf[10];
        
        fgets(Buf, sizeof(Buf), consoleInfo);
        std::string tmp = Buf;
        for (int i = 0; tmp[i]; i++) 
            if (tmp[i] == '\n') tmp[i] = '\0';
        int size = std::stoi(tmp);
        if (size < 24) 
        {
            pclose(consoleInfo);
            system("clear");
            goto(0,0);
            std::cout << "Terminal size must be at least 80x24\n";
            raise(SIGUSR1);
        }
        
        size_t position = tmp.find_first_of(' ');
        tmp.erase(0, position + 1);
        size = std::stoi(tmp);
        if (size < 80) 
        {
            pclose(consoleInfo);
            system("clear");
            goto(0,0);
            std::cout << "Terminal size must be at least 80x24\n";
            raise(SIGUSR1);
        }
        pclose(consoleInfo);
        nanosleep(&tw, &tr);
    }
}