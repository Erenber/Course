#pragma once
#include<string>
#include<utility>
#include<vector>
#include<signal.h>

#define goto(x, y) printf("%c[%d;%dH", 0x1B, x, y)

struct CPUcommands
{
public:
    std::string name;
    std::string cacheSize;
    std::string coreNumb;
    std::string flag;
    std::string temperature;
    std::string frequency;
    std::string utilization;

    CPUcommands();
    ~CPUcommands(){};
};

class CPU
{
private:
    CPUcommands cmd;
    std::string cpuName;
    std::string cacheSize;
    short int physicalCoreNumber;
    short int logicalCoreNumber;//defines in setFrequency (as the size of vector)
    std::pair<std::string, std::string> flags; //hyperthreading and long mode(x64)
    std::pair<double, std::string> mainTemperture;
    std::pair<double, std::string> coolerTemperature;
    std::vector<double> frequency;  
    std::vector<double> utilization;
    double averageUtilization;

public:
    CPU(){};
    ~CPU(){};
    void setCpuName();
    void setCacheSize();
    void setCoreNumber();
    void setFlags();
    void setTemperature();
    void setFrequency();
    void setUtilization();

    std::string getCpuName();
    std::string getCacheSize();
    short int getPhysicalCoreNumber();
    short int getLogicalCoreNumber();
    std::pair<double, std::string> getMainTemperature();
    std::pair<double, std::string> getCoolerTemperature();  
    std::pair<std::string, std::string> getFlags();
    std::vector<double> getFrequency();
    std::vector<double> getUtilization();
    double getAverageUtilization();

    void collectInformation(); //every information
    void refreshInformation(); //only temperature, MHz, memory
};