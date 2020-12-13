/*
 * coffeetch: a minimal CLI system information tool written in C.
 * Copyright (c) Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */





#include <stdio.h>

// Colours and text customisation
#define reset "\x1b[0m"
#define bold "\x1b[1m"
#define black "\x1b[30m"
#define red "\x1b[31m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define blue "\x1b[34m"
#define magenta "\x1b[35m"
#define cyan "\x1b[36m"
#define white "\x1b[37m"


int day, hour, min, sec, ramused, ramtotal, dpkg, flatpak;
char  user[50], host[50], os[50], model[50], modelversion[25], kernel[50], shell[25], cpu[50];


// Detects the current user
// This will detect the current user and the hostname
void detectUser() {
    FILE *userName = popen("echo $USER", "r");
    FILE *userHost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(userName, "%s", user);
    fscanf(userHost, "%s", host);
    fclose(userName);
    fclose(userHost);
}


// Detects the distro name
// This will detect the name of the distro
void detectDistro() {
    FILE *distroName = popen("lsb_release -ds 2>/dev/null", "r");

    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);
}


// Detects the kernel
// This will detect the kernel-release, kernel-name, and the hardware machin name
void detectKernel() {
    FILE *pathKernel = popen("uname -rsm", "r");

    fscanf(pathKernel, "%[^\n]s", kernel);
    fclose(pathKernel);
}


// Detects the model information
// This will detect the computer's product name and the product version
void detectModel() {
    FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(productName, "%[^\n]s", model);
    fscanf(productVersion, "%s", modelversion);
    fclose(productName);
    fclose(productVersion);
}


// Detects the shell information
void detectShell() {
    FILE *shellpath = popen("echo $SHELL", "r");

    fscanf(shellpath, "%s", shell);
    fclose(shellpath);
}


// Detects the packages
// Currently this only displays dpkg and flatpaks
void detectPackages() {
    FILE *dpkgs = popen("dpkg-query -l | wc -l 2>/dev/null", "r");
    FILE *flatpaks = popen("flatpak list | wc -l 2>/dev/null", "r");

    fscanf(dpkgs, "%d", &dpkg);
    fscanf(flatpaks, "%d", &flatpak);
    fclose(dpkgs);
    fclose(flatpaks);
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
// This will show the used RAM / total RAM
void detectRAM() {
    FILE *used = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *total = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(used, "%d", &ramused);
    fscanf(total, "%d", &ramtotal);
    fclose(used);
    fclose(total);
}


// Detects the uptime information
// This will show how long the computer is running: DDd, HHh, MMm
void detectUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sec);
    fclose(pathUptime);

    day = (sec/60/60/24);
    hour = (sec/60/60%24);
    min = (sec/60%60);
}


// Call all functions
// This will call of the functions that detects the system information
void getSysinfo() {
    detectUser();
    detectDistro();
    detectModel();
    detectKernel();
    detectShell();
    detectPackages();
    detectCPU();
    detectRAM();
    detectUptime();
}


int main() {
    getSysinfo();

    // Do not change these
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, pallate colours
    printf("%s %scoffeetch%s: a minimal CLI system information tool written in C.\n\n", bold, cyan, reset);
    printf("%s        ___          %suser%s:       %s@%s\n", bold, cyan, reset, user, host);
    printf("%s       (.· |         %sos%s:         %s\n", bold, cyan, reset, os);
    printf("%s       (<> |         %skernel%s:     %s\n", bold, cyan, reset, kernel);
    printf("%s      / __  \\        %smodel%s:      %s %s\n", bold, cyan, reset, model, modelversion);
    printf("%s     ( /  \\ /|       %scpu%s:        %s\n", bold, cyan, reset, cpu);
    printf("%s    _/\\ __)/_)       %sram%s:        %dM / %dM\n", bold, cyan, reset, ramused, ramtotal);
    printf("%s    \\|/-___\\|/       %sshell%s:      %s\n", bold, cyan, reset, shell);
    printf("%s                     %spkgs%s:       %d(dpkg), %d(flatpak)\n", bold, cyan, reset, dpkg, flatpak);
    printf("%s %s██%s██%s██%s██%s██%s██%s██%s██%s    %s%suptime%s:     %dd, %dh, %dm\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset, bold, cyan, reset, day, hour, min);

    return 0;
}
