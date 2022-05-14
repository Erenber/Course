#include"Controller.h"
#include"thread.h"

void Controller::cpuRefresh()
{
    cpu.refreshInformation();
}
void Controller::gpuRefresh()
{
    gpu.refreshInformation();
}

void Controller::start()
{   //-h -c -C -g -s(0 - 5) -error
    if (flags[5]){
        std::cout << "Invalid command line options or combinations. Use -h for help\n";
        return;
    }
    if (flags[0]){
        std::cout << "CPU/GPU Monitoring \nTerminal size must be at least 80x24\n"
                  << "[OPTIONS]:\n"
                  << "    -h :    Help menu\n\n"
                  << "    -c :    Display CPU information and exit\n\n"
                  << "    -C*:    Display CPU information with workload and exit\n"
                  << "            Workload mode [0-8], where 0 - min level, 8 - max\n"
                  << "            Example: ./monitor -C  | ./monitor -C7\n\n"
                  << "    -g :    Display GPU information and exit\n\n"
                  << "    -s*:    Additional flag for periodic information update\n"
                  << "            Can be combined with one of \"-c\" \"-C*\" \"-g\"\n"
                  << "            Speed modes [0(one-time output) - 5]\n"
                  << "            Example: ./monitor -C2 -s3 \t ./monitor -s0 -g\n\n";
                  return;
    }

    struct timespec tw = { 6 - flags[4], 0};
    struct timespec tr;
    if (flags[1]){

        cpu.collectInformation();
        console.PrintCpuTable(cpu.getLogicalCoreNumber());
        if (flags[4] > 0){
            printf("%c[%dm", 0x1B, 47);
            printf("%c[%dmPress Ctrl+C to exit program", 0x1B, 30);
            printf("%c[%dm ", 0x1B, 0);
            printf("%c[u", 0x1B);
            }
        console.PrintCpuData(cpu);
        if (flags[4] == 0){
            return;
        }
        while (1){
            nanosleep(&tw, &tr);
            this->cpuRefresh();
            //cpu.refreshInformation();
            console.PrintCpuData(cpu);
            printf("%c[u", 0x1B);
        }
    }

    if (flags[2]){
        pthread_t threads[THREADS_MAX_COUNT];
        int status_addr;
        int status;
        Arg arguments[THREADS_MAX_COUNT] = {
             "./Pictures/lake.bmp", "./GrayT/lakeGrayT.bmp",
            "./Pictures/mountain.bmp", "./GrayT/mountainGrayT.bmp",
            "./Pictures/lightning.bmp", "./GrayT/lightningGrayT.bmp",
            "./Pictures/city.bmp", "./GrayT/cityGrayT.bmp",
            "./Pictures/autumn.bmp", "./GrayT/autumnGrayT.bmp",
            "./Pictures/winter.bmp", "./GrayT/winterGrayT.bmp",
            "./Pictures/spring.bmp", "./GrayT/springGrayT.bmp",
            "./Pictures/summer.bmp", "./GrayT/summerGrayT.bmp"
        };

        for (int i = 0; i < flags[2]; i++){
            status = pthread_create(&threads[i], NULL, drawTBMP, (void*) &arguments[i]);
        }
    
    
        cpu.collectInformation();
        console.PrintCpuTable(cpu.getLogicalCoreNumber());
        if (flags[4] > 0){
            printf("%c[%dm", 0x1B, 47);
            printf("%c[%dmPress Ctrl+C to exit program", 0x1B, 30);
            printf("%c[%dm ", 0x1B, 0);
            printf("%c[u", 0x1B);
        }
        console.PrintCpuData(cpu);
        if (flags[4] == 0){
            return;
        }
        while (1){
            nanosleep(&tw, &tr);
            this->cpuRefresh();
            //cpu.refreshInformation();
            console.PrintCpuData(cpu);
            printf("%c[u", 0x1B);
        }
        for (int i = 0; i < THREADS_MAX_COUNT; i++) {
            status = pthread_join(threads[i], (void**)&status_addr);
        }
    }

    if (flags[3])
    {
        gpu.collectInformation(); 
        console.PrintGpuTable();
        
        if (flags[4] > 0)
        {
            printf("%c[%dm", 0x1B, 47);
            printf("%c[%dmPress Ctrl+C to exit program", 0x1B, 30);
            printf("%c[%dm ", 0x1B, 0);
            printf("%c[s", 0x1B);
        }

        if (flags[4] == 0)
        {
            console.PrintGpuData(gpu);
            goto(10, 0);
            std::cout << " ";
            return;
        }

        while (true){
            console.PrintGpuData(gpu);
            this->gpuRefresh();
            //gpu.refreshInformation();
        }
    }
}

void* drawTBMP(void* args){
    
    Arg *arg = (Arg*)args;
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    FILE* sourceFile, * destFile;

    unsigned char gray, r, g, b;
    sourceFile = fopen(arg->name.c_str() , "rb");
    destFile = fopen(arg->destination.c_str(), "wb");

    fread(&file_header, sizeof(file_header), 1, sourceFile);
    fread(&info_header, sizeof(info_header), 1, sourceFile);

    fwrite(&file_header, sizeof(file_header), 1, destFile);
    fwrite(&info_header, sizeof(info_header), 1, destFile);

    fseek(sourceFile, 56, SEEK_SET);
    fseek(destFile, 56, SEEK_SET);

    //pthread_detach(pthread_self());
     for (int repeat = 0; repeat < 10000 && flag; repeat++){
        fseek(sourceFile, 56, SEEK_SET);//56 = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
        fseek(destFile, 56, SEEK_SET);
        for (int i = 0; !feof(sourceFile); i += 3) {
            fread(&r, sizeof(char), 1, sourceFile);
            fread(&g, sizeof(char), 1, sourceFile);
            fread(&b, sizeof(char), 1, sourceFile);

            gray = ((0.3 * r) + (0.59 * g) + (0.11 * b));
            r = gray;
            g = gray;
            b = gray;

            fwrite(&r, sizeof(char), 1, destFile);
            fwrite(&g, sizeof(char), 1, destFile);
            fwrite(&b, sizeof(char), 1, destFile);
        }
    }
    
    fclose(sourceFile);
    fclose(destFile);
    return args;
}