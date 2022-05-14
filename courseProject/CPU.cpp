#include"CPU.h"
#include<algorithm>
#include<iostream>

CPUcommands::CPUcommands()
{
    name = "cat /proc/cpuinfo | grep 'model name'";
    cacheSize = "cat /proc/cpuinfo | grep 'cache size'";
    coreNumb = "cat /proc/cpuinfo | grep 'cpu cores'";
    flag = "cat /proc/cpuinfo | grep 'flags'";
    temperature = "cat /sys/class/hwmon/hwmon4/temp*";
    frequency = "cat /proc/cpuinfo | grep 'cpu MHz'";
    utilization = "top 1 -b -i -n 1 | grep Cpu | tr ',' '.'";
}

void CPU::setCpuName()
{
    FILE* nameInfo;
    
    if (!(nameInfo = popen(cmd.name.c_str(), "r")))
    {
        cpuName = "-";
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get CPU name\n";
        raise(SIGUSR1);
    } 

    char Buf[127];
    if (fgets(Buf, sizeof(Buf), nameInfo) == NULL)
    {
        cpuName = "-";
        pclose(nameInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU name\n";
        raise(SIGUSR1);
    }

    cpuName = Buf;
    while (cpuName[0] != ':') 
    {
        cpuName.erase(cpuName.begin());
    }

    cpuName.erase(cpuName.begin());
    pclose(nameInfo);
}
void CPU::setTemperature()
{
    FILE* tempInfo;

    if (!(tempInfo = popen(cmd.temperature.c_str(), "r")))
    {
        mainTemperture.first = coolerTemperature.first = 0.0;
        mainTemperture.second = coolerTemperature.second = "-";
        goto(0,0);
        std::cout << "Error (command): Can't get CPU temperature\n";
        raise(SIGUSR1);
    }

    char Buf[127];

    if (fgets(Buf, sizeof(Buf), tempInfo) == NULL)
    {
        mainTemperture.first = coolerTemperature.first = 0.0;
        mainTemperture.second = coolerTemperature.second = "-";
        pclose(tempInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU temperature\n";
        raise(SIGUSR1);
    }
    mainTemperture.first = std::stod(Buf) / 1000;

    if (fgets(Buf, sizeof(Buf), tempInfo) == NULL)
    {
        mainTemperture.first = coolerTemperature.first = 0.0;
        mainTemperture.second = coolerTemperature.second = "-";
        pclose(tempInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU temperature\n";
        raise(SIGUSR1);
    }
    mainTemperture.second = Buf;
    mainTemperture.second[4] = '\0';

    if (fgets(Buf, sizeof(Buf), tempInfo) == NULL)
    {
        coolerTemperature.first = 0.0;
        coolerTemperature.second = "-";
        pclose(tempInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU temperature\n";
        raise(SIGUSR1);
    }
    coolerTemperature.first = std::stod(Buf) / 1000;

    if (fgets(Buf, sizeof(Buf), tempInfo) == NULL)
    {
        coolerTemperature.first = 0.0;
        coolerTemperature.second = "-";
        pclose(tempInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU temperature\n";
        raise(SIGUSR1);
    }
    coolerTemperature.second = Buf;
    coolerTemperature.second[4] = '\0';

    pclose(tempInfo);
}
void CPU::setCacheSize()
{
    FILE* cacheSizeInfo;

    if (!(cacheSizeInfo = popen(cmd.cacheSize.c_str(), "r")))
    {
        cacheSize = "-";
        goto(0,0);
        std::cout << "Error (command): Can't get CPU cache\n";
        raise(SIGUSR1);
    }

    char Buf[127];

    if (fgets(Buf, sizeof(Buf), cacheSizeInfo) == NULL)
    {
        cacheSize = "-";
        pclose(cacheSizeInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU cache\n";
        raise(SIGUSR1);
    }
    cacheSize = Buf;
    size_t position = cacheSize.find_first_of(":");
    cacheSize.erase(0, position + 2);
    position = cacheSize.find_first_of(" ");

    cacheSize.insert(cacheSize.begin() + position, ' ');
    
    pclose(cacheSizeInfo);
}
void CPU::setCoreNumber()
{
    FILE* coreNumberInfo;

    if (!(coreNumberInfo = popen(cmd.coreNumb.c_str(), "r")))
    {
        physicalCoreNumber = 0;
        goto(0,0);
        std::cout << "Error (command): Can't get CPU cores\n";
        raise(SIGUSR1);
    }

    char Buf[127];

    if (fgets(Buf, sizeof(Buf), coreNumberInfo) == NULL)
    {
        physicalCoreNumber = 0;
        pclose(coreNumberInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU cores\n";
        raise(SIGUSR1);
    }
    std::string tmp = Buf;
    while(!isdigit(tmp[0])) 
    {
        tmp.erase(0, 1);
    }
    physicalCoreNumber = std::stoi(tmp);

    pclose(coreNumberInfo);
}
void CPU::setFrequency()
{
    FILE* frequencyInfo;

    std::vector<double> freq;
    if (!(frequencyInfo = popen(cmd.frequency.c_str(), "r")))
    {   
        logicalCoreNumber = 0;
        goto(0,0);
        std::cout << "Error (command): Can't get CPU frequency\n";
        raise(SIGUSR1);
    }

    char Buf[127];

    while (fgets(Buf, sizeof(Buf), frequencyInfo) != NULL)
    {
        std::string tmp = Buf;
        while(!isdigit(tmp[0])) {
            tmp.erase(0, 1);
        }
        freq.push_back(std::stod(tmp));
    }

    //Logical Cores Number
    frequency = freq;
    logicalCoreNumber = frequency.size();

    pclose(frequencyInfo);
}
void CPU::setFlags()
{
    FILE* flagInfo;

    if (!(flagInfo = popen(cmd.flag.c_str(), "r")))
    {
        flags.first = flags.second = "-";
        goto(0,0);
        std::cout << "Error (command): Can't get CPU flags\n";
        raise(SIGUSR1);
    }

    char Buf[1000];

    if (fgets(Buf, sizeof(Buf), flagInfo) == NULL)
    {
        flags.first = flags.second = "-";
        pclose(flagInfo);
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get CPU flags\n";
        raise(SIGUSR1);
    }

    std::string tmp = Buf;
    size_t pos;

    pos = tmp.find("ht");
    flags.first = "Hyperthreading";
    if (pos != std::string::npos)
        flags.first = "Hyperthreading: +\n";
    else 
        flags.first = "Hyperthreading: -\n";

    pos = tmp.find(" lm");
    if (pos != std::string::npos)
        flags.second = "Architecture: x86_64\n";
    else
        flags.second = "Architecture: 32-bit\n";

    pclose(flagInfo);
}
void CPU::setUtilization()
{
    FILE* utilInfo;
    std::vector<double> util;
    if (!(utilInfo = popen(cmd.utilization.c_str(), "r")))
    {
        goto(0,0);
        std::cout << "Error (command): Can't get CPU utilization\n";
        raise(SIGUSR1);
    }
    char utilBuf[1200];
    while (fgets(utilBuf, sizeof(utilBuf), utilInfo) != NULL)
    {
        std::string tmp = utilBuf;
        while(tmp[0] != 'i') tmp.erase(0, 1);
        if (tmp[0] == 'i') tmp.erase(0, 1);
        if (tmp[0] == '.') tmp.erase(0, 1);
        if (tmp[0] == ' ') tmp.erase(0, 1);
        double idle = std::stod(tmp);
        util.push_back(100 - idle);
    }
    utilization = util;

    averageUtilization = 0;

    for (int i = 0; i < utilization.size(); i++)
    {
        averageUtilization += utilization[i];
    }
    //This method is called after setFrequency(), so the number of logical cores is known
    averageUtilization /= logicalCoreNumber;

    pclose(utilInfo);
}

std::string CPU::getCpuName()
{
    return cpuName;
}
std::pair<double, std::string> CPU::getCoolerTemperature()
{
    return coolerTemperature;
}
std::pair<double, std::string> CPU::getMainTemperature()
{
    return mainTemperture;
}
std::string CPU::getCacheSize()
{
    return cacheSize;
}
short int CPU::getPhysicalCoreNumber()
{
    return physicalCoreNumber;
}
short int CPU::getLogicalCoreNumber()
{
    return logicalCoreNumber;
}
std::vector<double> CPU::getFrequency()
{
    return frequency;
}
std::pair<std::string, std::string> CPU::getFlags()
{
    return flags;
}
std::vector<double> CPU::getUtilization()
{
    return utilization;
}
double CPU::getAverageUtilization()
{
    return averageUtilization;
}

void CPU::collectInformation()
{
    refreshInformation();
    setCpuName();
    setCacheSize();
    setCoreNumber();
    setFlags();
}
void CPU::refreshInformation()
{
    setTemperature();
    setFrequency();
    setUtilization();
}