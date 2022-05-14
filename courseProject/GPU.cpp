#include"GPU.h"
#include<iostream>

GPUcommands::GPUcommands()
{
    command = "nvidia-settings -t -q ";
    name = command + "gpus | sed -e 's/^ *//' | sed '/ *#/d; /^ *$/d'";
    utilization = "/GPUUtilization";
    temperature = "/GPUCoreTemp";
    total = "/TotalDedicatedGPUMemory -q ";
    usage = "/UsedDedicatedGPUMemory";
    cores = "CUDACores";
    clocks = "nvidia-smi --query-gpu=clocks.mem,clocks.gr --format=csv,nounits,noheader | tr ', ' ' '";
}

void GPU::setGpuName()
{
    FILE* nameInfo;

    if (!(nameInfo = popen(cmd.name.c_str(), "r")))
    {
        gpuId = gpuName = "-";
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU name\n";
        raise(SIGUSR1);
    } 

    char Buf[512];
    if (fgets(Buf, sizeof(Buf), nameInfo) == NULL)
    {
        gpuId = gpuName = "-";
        pclose(nameInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU name\n";
        raise(SIGUSR1);
    }

    std::string name;

    while (fgets(Buf, sizeof(Buf), nameInfo) != NULL)
    {
        if (Buf[0] == '[') name = Buf;
    }
    
    size_t startposofid = name.find_last_of("[");
    size_t endposofid = name.find_last_of("]");
    gpuId = name.substr(startposofid, endposofid - startposofid + 1);

    size_t startposofname = name.find_last_of("(");
    size_t endposofname = name.find_last_of(")");
    gpuName = name.substr(startposofname + 1, endposofname - startposofname - 1);

    pclose(nameInfo);
}
void GPU::setGraphicUtilization()
{
    std::string cmdUtil = cmd.command + gpuId + cmd.utilization;
    FILE* utilInfo;

    if (!(utilInfo = popen(cmdUtil.c_str(), "r")))
    {
        graphicUtilization = 0.0;
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU utilization\n";
        raise(SIGUSR1);
    } 

    char Buf[512];

    if (fgets(Buf, sizeof(Buf), utilInfo) == NULL)
    {
        graphicUtilization = 0.0;
        pclose(utilInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU utilization\n";
        raise(SIGUSR1);
    }

    std::string util = Buf;
    
    size_t startposofid = util.find_first_of("=");
    size_t endposofid = util.find_first_of(",");
    util = util.substr(startposofid + 1, endposofid - startposofid - 1);
    graphicUtilization = std::stod(util);

    pclose(utilInfo);
}
void GPU::setTemperature()
{
    std::string cmdTemp = cmd.command + gpuId + cmd.temperature; 
    FILE* tempInfo;

    if (!(tempInfo = popen(cmdTemp.c_str(), "r")))
    {   
        temperature = 0;
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU temperature\n";
        raise(SIGUSR1);
    } 

    char Buf[512];
    if (fgets(Buf, sizeof(Buf), tempInfo) == NULL)
    {
        temperature = 0;
        pclose(tempInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU temperature\n";
        raise(SIGUSR1);
    }

    std::string temp = Buf;
    temperature = std::stoi(temp);

    pclose(tempInfo);
}
void GPU::setMemoryUsage()
{
    std::string cmdMemory = cmd.command + gpuId + cmd.total + gpuId + cmd.usage;
    FILE* memInfo;

    if (!(memInfo = popen(cmdMemory.c_str(), "r")))
    {
        totalMemory = 0;
        memoryUsage = 0.0;
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU memory usage\n";
        raise(SIGUSR1);
    } 

    char Buf[512];
    if (fgets(Buf, sizeof(Buf), memInfo) == NULL)
    {
        totalMemory = 0;
        memoryUsage = 0.0;
        pclose(memInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU memory usage\n";
        raise(SIGUSR1);
    }

    std::string memory = Buf;
    totalMemory = std::stoi(memory);

    if (fgets(Buf, sizeof(Buf), memInfo) == NULL)
    {
        pclose(memInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU memory usage\n";
        raise(SIGUSR1);
    }

    memory = Buf;
    memoryUsage = std::stod(memory) / totalMemory * 100;

    pclose(memInfo);
}
void GPU::setCudaCores()
{
    std::string cmdCores = cmd.command + cmd.cores;
    FILE* cudaInfo;

    if (!(cudaInfo = popen(cmdCores.c_str(), "r")))
    {
        cudaCores = 0;
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU cores number\n";
        raise(SIGUSR1);
    } 

    char Buf[512];
    if (fgets(Buf, sizeof(Buf), cudaInfo) == NULL)
    {
        cudaCores = 0;
        pclose(cudaInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU cores number\n";
        raise(SIGUSR1);
    }

    std::string cores = Buf;
    cudaCores = std::stoi(cores);

    pclose(cudaInfo);
}
void GPU::setClocks()
{
    FILE* clocksInfo;
    
    if (!(clocksInfo = popen(cmd.clocks.c_str(), "r")))
    {
        memoryClocks = graphicClocls = 0;
        system("clear");
        goto(0,0);
        std::cout << "Error (command): Can't get GPU clocks\n";
        raise(SIGUSR1);
    } 

    char Buf[512];
    if (fgets(Buf, sizeof(Buf), clocksInfo) == NULL)
    {
        memoryClocks = graphicClocls = 0;
        pclose(clocksInfo);
        system("clear");
        goto(0,0);
        std::cout << "Error (wrong command format): Can't get GPU clocks\n";
        raise(SIGUSR1);
    }

    std::string clocks = Buf;
    memoryClocks = std::stoi(clocks);
    
    size_t firstspace = clocks.find_first_of(' ');
    firstspace++;

    clocks.erase(0, firstspace);
    graphicClocls = std::stoi(clocks);

    pclose(clocksInfo);
}

int GPU::getTotalMemory()
{
    return totalMemory;
}
int GPU::getGraphicClocks()
{
    return graphicClocls;
}
int GPU::getMemoryClocks()
{
    return memoryClocks;
}
int GPU::getCudaCores()
{
    return cudaCores;
}
double GPU::getMemoryUsage()
{
    return memoryUsage;
}
int GPU::getTemperature()
{
    return temperature;
}
double GPU::getGraphicUtilization()
{
    return graphicUtilization;
}
std::string GPU::getGpuId()
{
    return gpuId;
}
std::string GPU::getGpuName()
{
    return gpuName;
}

void GPU::refreshInformation()
{
    setGraphicUtilization();
    setTemperature();
    setMemoryUsage();
    setClocks();
}
void GPU::collectInformation()
{
    refreshInformation();
    setGpuName();
    setCudaCores();
}