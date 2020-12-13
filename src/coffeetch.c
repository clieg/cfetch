// coffeetch: a minimal CLI system information tool written in C.





#include <stdio.h>

// Text colours and weight
#define NEWLINE = '\n'
#define RESET "\x1b[0m"     // Reset
#define TBOLD "\x1b[1m"     // Bold
#define CBLACK "\x1b[30m"   // Black
#define CRED "\x1b[31m"     // Red
#define CGREEN "\x1b[32m"   // Green
#define CYELLOW "\x1b[33m"  // Yellow
#define CBLUE "\x1b[34m"    // Blue
#define CMAGENTA "\x1b[35m" // Magenta
#define CCYAN "\x1b[36m"    // Cyan
#define CWHITE "\x1b[37m"   // White

int day, hour, min, sec, ramused, ramtotal, dpkg, flatpak;
char  user[50], host[50], os[50], model[50], modelversion[25], kernel[25], shell[25], cpu[50];

// Detects the current user information
// This detect get the username and the hostname
void detectUser() {
    FILE *detecuser = popen("echo $USER", "r");
    FILE *detecthost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(detecuser, "%s", user);
    fscanf(detecthost, "%s", host);
    fclose(detecuser);
    fclose(detecthost);
}

// Detects the OS name
// This will detect the short description of the Linux Standard Base (LBS) release information
void detectOS() {
    FILE *detectos = popen("lsb_release -ds 2>/dev/null", "r");

    fscanf(detectos, "%[^\n]s", os);
    fclose(detectos);
}


// Detects the kernel
// This will detect the kernel's name, kernel-release, and machine hardware name
void detectKernel() {
    FILE *detectkernel = popen("uname -rsm", "r");

    fscanf(detectkernel, "%[^\n]s", kernel);
    fclose(detectkernel);
}


// Detects the model information
// This detect get the machine's name and model information
void detectModel() {
    FILE *detectmodel = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *detectmodelversion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(detectmodel, "%[^\n]s", model);
    fscanf(detectmodelversion, "%s", modelversion);
    fclose(detectmodel);
    fclose(detectmodelversion);
}


// Detects the shell information
// This will detect the shell that is being used
void detectShell() {
    FILE *detectshell = popen("echo $SHELL", "r");

    fscanf(detectshell, "%s", shell);
    fclose(detectshell);
}


// Detects the packages
// Currently this only displays dpkg and flatpaks
void detectPackages() {
    FILE *detectdpkgs = popen("dpkg-query -l | wc -l 2>/dev/null", "r");
    FILE *detectflatpaks = popen("flatpak list | wc -l 2>/dev/null", "r");

    fscanf(detectdpkgs, "%d", &dpkg);
    fscanf(detectflatpaks, "%d", &flatpak);
    fclose(detectdpkgs);
    fclose(detectflatpaks);
}


// Detects the CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void detectCPU() {
    FILE *cpuinfo = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", cpu);
    fclose(cpuinfo);
}


// Detects the RAM information
// This will detect the used RAM / the total RAM
void detectRAM() {
    FILE *used = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *total = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(used, "%d", &ramused);
    fscanf(total, "%d", &ramtotal);
    fclose(used);
    fclose(total);
}


// Detects the uptime information
// This will detect the uptime of the machin
// DDd, HHh, MMm
void detectUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sec);
    fclose(pathUptime);

    day = (sec/60/60/24);
    hour = (sec/60/60%24);
    min = (sec/60%60);
}


// Call all functions that detects the system information
void init() {
    detectUser();
    detectOS();
    detectModel();
    detectKernel();
    detectShell();
    detectPackages();
    detectCPU();
    detectRAM();
    detectUptime();
}



int main() {
    init();

    // Do not change these
    // Changing these will only disportionate the printed system information
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, pallate colour
    printf("%s%s coffeetch%s: a minimal CLI system information tool written in C.\n\n", TBOLD, CCYAN, RESET);
    printf("%s         ___          %suser%s:      %s@%s\n", TBOLD, CCYAN, RESET, user, host);
    printf("%s        (.· |         %sos%s:        %s\n", TBOLD, CCYAN, RESET, os);
    printf("%s        (<> |         %skernel%s:    %s\n", TBOLD, CCYAN, RESET, kernel);
    printf("%s       / __  \\        %smodel%s:     %s %s\n", TBOLD, CCYAN, RESET, model, modelversion);
    printf("%s      ( /  \\ /|       %scpu%s:       %s\n", TBOLD, CCYAN, RESET, cpu);
    printf("%s     _/\\ __)/_)       %sram %s:      %dM / %dM\n", TBOLD, CCYAN, RESET, ramused, ramtotal);
    printf("%s     \\|/-___\\|/       %sshell%s:     %s\n", TBOLD, CCYAN, RESET, shell);
    printf("%s                      %spkgs%s:      %d(dpkg), %d(flatpak)\n", TBOLD, CCYAN, RESET, dpkg, flatpak);
    printf("%s %s ██%s██%s██%s██%s██%s██%s██%s██    %suptime%s:    %dd %dh %dm", TBOLD, CBLACK, CRED, CGREEN, CYELLOW, CBLUE, CMAGENTA, CCYAN, CWHITE, CCYAN, RESET, day, hour, min);

    printf("\n\n\n");

    return 0;
}
