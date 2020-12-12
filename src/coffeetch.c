// coffeetch: a minimal CLI system information tool written in C.


#include <stdio.h>

int day, hour, min, sec, ramused, ramtotal, dpkg, flatpak;
char  user[50], host[50], os[50], model[50], modelversion[25], kernel[50], shell[25], cpu[50];


// Get current user
void getUser() {
    FILE *userName = popen("echo $USER", "r");
    FILE *userHost = popen("cat /proc/sys/kernel/hostname", "r");

    fscanf(userName, "%s", user);
    fscanf(userHost, "%s", host);
    fclose(userName);
    fclose(userHost);
}

// Get distro name
void getDistro() {
    FILE *distroName = popen("lsb_release -ds 2>/dev/null", "r");

    fscanf(distroName, "%[^\n]s", os);
    fclose(distroName);
}


// Get kernel
void getKernel() {
    FILE *pathKernel = popen("uname -rsm", "r");

    fscanf(pathKernel, "%[^\n]s", kernel);
    fclose(pathKernel);
}


// Get model information
void getModel() {
    FILE *productName = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *productVersion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");

    fscanf(productName, "%[^\n]s", model);
    fscanf(productVersion, "%s", modelversion);
    fclose(productName);
    fclose(productVersion);
}


// Get shell information
void getShell() {
    FILE *shellpath = popen("echo $SHELL", "r");

    fscanf(shellpath, "%s", shell);
    fclose(shellpath);
}


// Get packages
// Currently this only displays dpkg and flatpaks
void getPackages() {
    FILE *dpkgs = popen("dpkg-query -l | wc -l 2>/dev/null", "r");
    FILE *flatpaks = popen("flatpak list | wc -l 2>/dev/null", "r");

    fscanf(dpkgs, "%d", &dpkg);
    fscanf(flatpaks, "%d", &flatpak);
    fclose(dpkgs);
    fclose(flatpaks);
}


// Get CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void getCPU() {
    FILE *cpuinfo = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//'", "r");

    fscanf(cpuinfo, "%[^\n]s", cpu);
    fclose(cpuinfo);
}


// Get RAM information
void getRAM() {
    FILE *used = popen("vmstat -s -S M | grep ' used memory'", "r");
    FILE *total = popen("vmstat -s -S M | grep ' total memory'", "r");

    fscanf(used, "%d", &ramused);
    fscanf(total, "%d", &ramtotal);
    fclose(used);
    fclose(total);
}


// Get uptime information
void getUptime() {
    FILE *pathUptime = fopen("/proc/uptime", "r");

    fscanf(pathUptime, "%d", &sec);
    fclose(pathUptime);

    day = (sec/60/60/24);
    hour = (sec/60/60%24);
    min = (sec/60%60);
}


// Call all functions
void init() {
    getUser();
    getDistro();
    getModel();
    getKernel();
    getShell();
    getPackages();
    getCPU();
    getRAM();
    getUptime();
}

/*
 *   Colour legend
 *   \x1b[30m - Black
 *   \x1b[31m - Red
 *   \x1b[32m - Green
 *   \x1b[33m - YelloW os:\x
 *   \x1b[34m - Blue
 *   \x1b[35m - Magenta
 *   \x1b[36m - Cyan
 *   \x1b[37m - White
 */
int main() {
    init();

    // Do not change these
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, pallate colour
    printf("\n\x1b[1m \x1b[36mcoffeetch\x1b[0m: a minimal CLI system information tool written in C.\n\n");
    printf("\x1b[1m        ___          \x1b[36muser\x1b[0m:       %s@%s\n", user, host);
    printf("\x1b[1m       (.· |         \x1b[36mos\x1b[0m:         %s\n", os);
    printf("\x1b[1m       (<> |         \x1b[36mkernel\x1b[0m:     %s\n", kernel);
    printf("\x1b[1m      / __  \\        \x1b[36mmodel\x1b[0m:      %s %s\n", model, modelversion);
    printf("\x1b[1m     ( /  \\ /|       \x1b[36mcpu\x1b[0m:        %s\n", cpu);
    printf("\x1b[1m    _/\\ __)/_)       \x1b[36mram\x1b[0m:        %dM / %dM\n", ramused, ramtotal);
    printf("\x1b[1m    \\|/-___\\|/       \x1b[36mshell\x1b[0m:      %s\n", shell);
    printf("\x1b[1m                     \x1b[36mpkgs\x1b[0m:       %d(dpkg), %d(flatpak)\n", dpkg, flatpak);
    printf("\x1b[1m\x1b[30m ██\x1b[31m██\x1b[32m██\x1b[33m██\x1b[34m██\x1b[35m██\x1b[36m██\x1b[37m██\x1b[0m    \x1b[1;36muptime\x1b[0m:     %dd, %dh, %dm\n", day, hour, min);

    printf("\n\n\n");

    return 0;
}
