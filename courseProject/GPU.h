#include<string>
#include<signal.h>

#define goto(x, y) printf("%c[%d;%dH", 0x1B, x, y)

struct GPUcommands
{
public:
    std::string command;
    std::string name;
    std::string utilization;
    std::string temperature;
    std::string total;
    std::string usage;
    std::string cores;
    std::string clocks;

    GPUcommands();
    ~GPUcommands(){};
};

class GPU
{
private:
    GPUcommands cmd;
    std::string gpuId;//nvidia-settings -t -q gpus | sed -e 's/^ *//' | sed '/ *#/d; /^ *$/d'
    std::string gpuName;
    double graphicUtilization;
    int temperature;
    int totalMemory;
    double memoryUsage;
    int cudaCores;
    int memoryClocks;
    int graphicClocls;

public:
    GPU(){};
    ~GPU(){};

    void setGpuName();//also id
    void setGraphicUtilization();
    void setTemperature();
    void setMemoryUsage();
    void setCudaCores();
    void setClocks();

    int getMemoryClocks();
    int getGraphicClocks();
    int getCudaCores();
    std::string getGpuName();
    std::string getGpuId();
    double getGraphicUtilization();
    int getTemperature();
    double getMemoryUsage();
    int getTotalMemory();

    void collectInformation();
    void refreshInformation();
};