#include"console.h"

void Console::PrintCpuTable(short logicalCoreNumber)
{
    system("clear");
    std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n"
              << std::setw(79) << std::setfill(' ') << std::left << "| Cpu Name:                                             | Cores(P/L):         "
              << "|\n"
              << std::setw(79) << std::setfill(' ') << std::left << "|                                            Temperature:                "
              << "|\n"
              << std::setw(79) << std::setfill('-') << std::left << '|' << "|\n"
              << std::setw(79) << std::setfill(' ') << std::left << "|   Cpu#         Utililzation               Frequence             Cache "
              << "|\n"
              << std::setw(79) << std::setfill('=') << std::left << '|' << "|\n";
            for (int i = 0; i < logicalCoreNumber; i++)
            {
                std::cout << std::setw(79) << std::setfill(' ') << std::left << '|' << "|\n";
            }
            std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n";
}
bool Console::PrintCpuData(CPU& cpu)
{
        std::vector<double> utilization = cpu.getUtilization();
        std::vector<double> frequency = cpu.getFrequency();
        int logicalCores = cpu.getLogicalCoreNumber();
        printf("%c[s", 0x1B);
        goto(2, 12);
        std::cout << cpu.getCpuName();
        goto(2, 71);
        std::cout << cpu.getPhysicalCoreNumber() << " - " << cpu.getLogicalCoreNumber();
        goto(3, 3);
        std::cout << cpu.getFlags().first;
        goto(3, 21);
        std::cout << "|";
        goto(3,23);
        std::cout << cpu.getFlags().second;
        goto(3, 44);
        std::cout << "|";
        goto(3,59);
        std::cout << std::fixed << std::setprecision(2) << cpu.getMainTemperature().first << "°C - " << cpu.getMainTemperature().second;
            //goto(4,25);
            //std::cout << cpu.getCoolerTemperature().first << " " << cpu.getCoolerTemperature().second;
        goto(5, 30);
        std::cout << "(" << std::left <<std::setw(6) << std::setfill(' ') << std::fixed << std::setprecision(2) << cpu.getAverageUtilization() << "%)";
        int i = 0;
        for (int i = 0; i < logicalCores; i++)
        {
            goto(7 + i, 5);
            std::cout << "Cpu#" << i;
            goto(7 + i, 24);
            std::cout << std::setw(6) << std::setfill(' ') << std::right << std::fixed << std::setprecision(2) << utilization[i] << " %";
            goto(7 + i, 44);
            std::cout << std::setw(6) << std::setfill(' ') << std::right << std::setprecision(1) << std::fixed << frequency[i] << "  MHz";
            goto(7 + i, 66);
            std::cout << cpu.getCacheSize();
        }
        isPrinted = true;
            
        printf("%c[u", 0x1B);
    return true;
}
void Console::PrintGpuTable()
{
    system("clear");
    std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n"
              << std::setw(79) << std::setfill(' ') << std::left << "| Id:            Gpu Name: "
              << "|\n"
              << std::setw(79) << std::setfill('-') << std::left << '|' << "|\n"
              << std::setw(79) << std::setfill(' ') << std::left << "| Total memory:                 CUDA Cores:              Temperature: "
              << "|\n"
              << std::setw(79) << std::setfill('-') << std::left << '|' << "|\n"
              << std::setw(79) << std::setfill(' ') << std::left << "| Gpu-Utilization       Memory-Usage       Graphic-Clocks      Memory-Clocks"
              << "|\n"
              << std::setw(79) << std::setfill('=') << std::left << '|' << "|\n"
              << std::setw(79) << std::setfill(' ') << std::left << '|' << "|\n"
              << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n";
}

bool Console::PrintGpuData(GPU& gpu)
{
    //printf("%c[s", 0x1B);
    goto(2, 7);
    std::cout << gpu.getGpuId();
    goto(2, 28);
    std::cout << gpu.getGpuName();
    goto(4, 17);
    std::cout << gpu.getTotalMemory() << " Mb";
    goto(4, 45);
    std::cout << gpu.getCudaCores();
    goto(4, 71);
    std::cout << gpu.getTemperature() << " °C";
    goto(8, 7);
    std::cout << std::setw(5) << std::setfill(' ') << std::left << std::fixed << std::setprecision(2) << gpu.getGraphicUtilization() << " %";
    goto(8, 28);
    std::cout << std::setw(5) << std::setfill(' ') << std::left << gpu.getMemoryUsage() << " %";
    goto(8, 44);
    std::cout << std::setw(6) << std::setfill(' ') << std::right << gpu.getGraphicClocks() << " MHz";
    goto(8, 64);
    std::cout << std::setw(6) << std::setfill(' ') << std::right << gpu.getMemoryClocks() << " MHz";
    goto(12, 0);
    isPrinted = true;
    return true;
}