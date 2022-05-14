#include"CPU.h"
#include"GPU.h"
#include<iostream>
#include<iomanip>

class Console
{
private:
    bool isPrinted;
public:
    Console(){isPrinted = false;};
    ~Console(){};
    void PrintCpuTable(short);
    bool PrintCpuData(CPU& cpu);

    void PrintGpuTable();
    bool PrintGpuData(GPU& gpu);
};
