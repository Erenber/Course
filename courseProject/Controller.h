#include"console.h"

class Controller
{
private:
    Console console;
    GPU gpu;
    CPU cpu;
    int *flags;
    
public:
    Controller(int *flags)
    {
        this->flags = flags;
    };
    ~Controller(){};

    void start();
    void cpuRefresh();
    void gpuRefresh();
};