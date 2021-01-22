/*
 * cfetch: A minimal system information tool for Linux-based operating systems.
 * Copyright (c) 2020 - 2021 Clint
 * This programme is provided under the GPL-3.0 License. See LICENSE for more details.
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>

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


// System information variables
struct sysinfo {
    char username[25], hostname[65], os[50], distro[50], kernel[50], modelname[50], modelversion[50], cpu[50], shell[8];
    int ramused, ramtotal, core;
    int apk, apt, dnf, emerge, flatpak, kiss, nix, opkg, pacman, rpm, snap, xbps, yay, yum, zypper;
    int day, hour, min, sec;
} sysinfo;


// Gets the current user
// This will get the current user and the hostname
void getuser() {
    struct utsname hostname;
    uname(&hostname);
    snprintf(sysinfo.username, 25, "%s", getenv("USER"));
    snprintf(sysinfo.hostname, 65, "%s", hostname.nodename);
}

// Gets the distro name
// This will get the name of the distro
void getos() {
    FILE *os = popen("if [ $(command -v lsb_release) ];then lsb_release -ds;else grep -i 'PRETTY_NAME=' /etc/*-release | cut -d '\"' -f2;fi 2>/dev/null", "r");
    fscanf(os, "%[^\n]", sysinfo.os);
    fclose(os);
}

// Gets the kernel
// This will get the kernel-release, kernel-name, and the hardware machin name
void getkernel() {
	struct utsname kerninfo;
	uname(&kerninfo);
	snprintf(sysinfo.kernel, 195, "%s %s %s", kerninfo.sysname, kerninfo.release, kerninfo.machine);
}

// Gets the model information
// This will get the computer's product name and the product version
void getmodel() {
    FILE *modelname = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    FILE *modelversion = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
    if (0 != modelname) {
        fscanf(modelname, "%49[^\n]", sysinfo.modelname);
        fclose(modelname);
    } else
        strncpy(sysinfo.modelname,"unavailable\0",12);
    if (0 != modelversion) {
        fscanf(modelversion, "%49s", sysinfo.modelversion);
        fclose(modelversion);
    } else
        strncpy(sysinfo.modelversion,"unavailable\0",12);
}

// Gets the shell information
void Getshell() {
    FILE *shell = popen("basename $SHELL", "r");
    fscanf(shell, "%s", sysinfo.shell);
    fclose(shell);
}

// Gets the packages
// Supports the distro's package manager, flatpak, and snap
void getpackages() {
    FILE *apk = popen("apk list --installed 2>/dev/null | wc -l" , "r");
    FILE *apt = popen("dpkg-query -f '${binary:Package}\n' -W 2>/dev/null | wc -l", "r");
    FILE *dnf = popen("dnf list installed 2>/dev/null | wc -l", "r");
    FILE *emerge = popen("qlist -I 2>/dev/null | wc -l", "r");
    FILE *flatpak = popen("flatpak list 2>/dev/null | wc -l", "r");
    FILE *kiss = popen("kiss list 2>/dev/null | wc -l", "r");
    FILE *nix = popen("nix-store -q --requisites /run/current-system/sw 2>/dev/null | wc -l", "r");
    FILE *opkg = popen("opkg list-installed  2>/dev/null | wc -l", "r");
    FILE *pacman = popen("pacman -Q 2>/dev/null | wc -l", "r");
    FILE *rpm = popen("rpm -qa --last 2>/dev/null | wc -l", "r");
    FILE *snap = popen("snap list 2>/dev/null | wc -l", "r");
    FILE *xbps = popen("xbps-query -l 2>/dev/null | wc -l", "r");
    FILE *yay = popen("yay -Q 2>/dev/null | wc -l", "r");
    FILE *yum = popen("yum list installed 2>/dev/null | wc -l", "r");
    FILE *zypper = popen("zypper se 2>/dev/null | wc -l", "r");

    fscanf(apk, "%d", &sysinfo.apk);
    fscanf(apt, "%d", &sysinfo.apt);
    fscanf(dnf, "%d", &sysinfo.dnf);
    fscanf(emerge, "%d", &sysinfo.emerge);
    fscanf(flatpak, "%d", &sysinfo.flatpak);
    fscanf(kiss, "%d", &sysinfo.kiss);
    fscanf(nix, "%d", &sysinfo.nix);
    fscanf(opkg, "%d", &sysinfo.opkg);
    fscanf(pacman, "%d", &sysinfo.pacman);
    fscanf(rpm, "%d", &sysinfo.rpm);
    fscanf(snap, "%d", &sysinfo.snap);
    fscanf(xbps, "%d", &sysinfo.xbps);
    fscanf(yay, "%d", &sysinfo.yay);
    fscanf(yum, "%d", &sysinfo.yum);
    fscanf(zypper, "%d", &sysinfo.zypper);
    fclose(apk);
    fclose(apt);
    fclose(dnf);
    fclose(emerge);
    fclose(flatpak);
    fclose(kiss);
    fclose(nix);
    fclose(opkg);
    fclose(pacman);
    fclose(rpm);
    fclose(snap);
    fclose(xbps);
    fclose(yay);
    fclose(yum);
    fclose(zypper);
}

// Print the correct packages and names
void printpkgs(){
    int comma=0;
    if (sysinfo.apk != 0) {
        if (comma == 1) printf(", ");
        printf("%d (apk)", sysinfo.apk);
        comma = 1;
    }
    if (sysinfo.apt != 0) {
        if (comma == 1) printf(", ");
        printf("%d (apt)", sysinfo.apt);
        comma = 1;
    }
    if (sysinfo.dnf != 0) {
        if(comma == 1) printf(", ");
        printf("%d (dnf)", sysinfo.dnf);
        comma = 1;
    }
    if (sysinfo.emerge != 0) {
        if (comma == 1) printf(", ");
        printf("%d (emerge)", sysinfo.emerge);
        comma = 1;
    }
    if (sysinfo.flatpak != 0) {
        if (comma == 1) printf(", ");
        printf("%d (flatpak)", sysinfo.flatpak);
        comma = 1;
    }
    if (sysinfo.kiss != 0) {
        if (comma == 1) printf(", ");
        printf("%d (kiss)", sysinfo.kiss);
        comma = 1;
    }
    if (sysinfo.nix != 0) {
        if (comma == 1) printf(", ");
        printf("%d (nix)", sysinfo.nix);
        comma = 1;
    }
    if (sysinfo.opkg != 0) {
        if (comma == 1) printf(", ");
        printf("%d (opkg)", sysinfo.opkg);
    }
    if (sysinfo.pacman != 0) {
        if (comma == 1) printf(", ");
        printf("%d (pacman)", sysinfo.pacman);
        comma = 1;
    }
    if (sysinfo.rpm != 0) {
        if (comma == 1) printf(", ");
        printf("%d (rpm)", sysinfo.rpm);
        comma = 1;
    }
    if (sysinfo.snap != 0) {
        if (comma == 1) printf(", ");
        printf("%d (snap)", sysinfo.snap);
        comma = 1;
    }
    if (sysinfo.xbps != 0) {
        if (comma == 1) printf(", ");
        printf("%d (xbps)", sysinfo.xbps);
        comma = 1;
    }

    if (sysinfo.yay != 0) {
        if (comma == 1) printf(", ");
        printf("%d (yay)", sysinfo.yay);
        comma = 1;
    }
    if (sysinfo.yum != 0) {
        if (comma == 1) printf(", ");
        printf("%d (yum)", sysinfo.yum);
        comma=1;
    }
    if (sysinfo.zypper != 0) {
        if (comma == 1) printf(", ");
        printf("%d (zypper)", sysinfo.zypper);
        comma = 1;
    }

    printf("%s\n", reset);
}

// Gets the CPU information
// lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//
// sed -r 's/Model name:\\s{1,}// -> This will remove the 'Model name:', only the CPU will be printed.
void getcpu() {
    FILE *cpu = popen("grep -i 'model name' /proc/cpuinfo | head -1 | cut -f3- -d' '", "r");
    FILE *core = popen("nproc", "r");
    fscanf(cpu, "%[^\n]", sysinfo.cpu);
    fscanf(core, "%d", &sysinfo.core);
    fclose(cpu);
    fclose(core);
}

// Gets the RAM information
// This will show the used RAM / total RAM
void getram() {
    FILE *ramused = popen("grep -i MemAvailable /proc/meminfo | awk '{print $2}'", "r");
    FILE *ramtotal = popen("grep -i MemTotal /proc/meminfo | awk '{print $2}'", "r");
    fscanf(ramused, "%d", &sysinfo.ramused);
    fscanf(ramtotal, "%d", &sysinfo.ramtotal);
    sysinfo.ramused=sysinfo.ramtotal-sysinfo.ramused;
    sysinfo.ramtotal=sysinfo.ramtotal/1024;
    sysinfo.ramused=sysinfo.ramused/1024;
    fclose(ramused);
    fclose(ramtotal);
}

// Gets the uptime information
// This will show how long the computer is running: DDd, HHh, MMm
void getuptime() {

	// CLOCK_UPTIME or CLOCK_BOOTTIME should be defined, so only use /proc/uptime as a last resort
   #ifdef CLOCK_UPTIME
	struct timespec si;
	clock_gettime(CLOCK_UPTIME, &si);
	sysinfo.day = (si.tv_sec/60/60/24);
	sysinfo.hour = (si.tv_sec/60/60%24);
	sysinfo.min = (si.tv_sec/60%60);
   #elif CLOCK_BOOTTIME
	struct timespec si;
	clock_gettime(CLOCK_BOOTTIME, &si);
	sysinfo.day = (si.tv_sec/60/60/24);
	sysinfo.hour = (si.tv_sec/60/60%24);
	sysinfo.min = (si.tv_sec/60%60);
   #else
    	FILE *uptime = fopen("/proc/uptime", "r");
    	fscanf(uptime, "%d", &sysinfo.sec);
    	fclose(uptime);

    	sysinfo.day = (sysinfo.sec/60/60/24);
    	sysinfo.hour = (sysinfo.sec/60/60%24);
    	sysinfo.min = (sysinfo.sec/60%60);
   #endif
}

// Call all functions
// This will call of the functions that Gets the system information
void getsysinfo() {
    getuser();
    getos();
    getmodel();
    getkernel();
    Getshell();
    getpackages();
    getcpu();
    getram();
    getuptime();
}

int main() {
    getsysinfo();

    // Do not change these
    // user, os, kernel, model, cpu, ram, shell, pkgs, uptime, palette colours
    if (strstr(sysinfo.os, "Alpine") != NULL) {
        printf("%s                      %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\            OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\           KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      / /\\ \\  /\\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     / /  \\ \\/  \\     CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    / /    \\ \\/\\ \\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / / /|   \\ \\ \\ \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /_/ /_|    \\_\\ \\_\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                      UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                      %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Arch") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\        OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\       KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      /\\   \\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /  __  \\     CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    /  (  )  \\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / __|  |__ \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.\\`      \\`.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Bang") != NULL) {
        printf("%s                    %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\          OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       / _\\         KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      /  \\ /        MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /   // \\       CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    /   //   \\      RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / ___()___ \\     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.\\`        \\`.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                    %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "ArchLabs") != NULL) {
        printf("%s                    %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\          OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\         KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      / /\\ \\        MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     / //\\\\ \\       CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    / //  \\\\ \\      RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / _\\\\  //_ \\     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.\\    /   /.\\    PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                    %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Arco") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\        OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\       KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      / /\\ \\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     / /  \\ \\     CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    / /    \\ \\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / / -----\\ \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /_/  ````-.\\_\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Artix") != NULL) {
        printf("%s                    %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        /\\          OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /  \\         KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      /\\`'.,\\       MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /     ',       CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    /      ,\\`\\     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   /   ,.'\\`.  \\    SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.,'\\`     \\`'.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                    %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Cent") != NULL) {
        printf("%s               %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s   ____^____   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s   |\\  |  /|   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   | \\ | / |   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  <---- ---->  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   | / | \\ |   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   |/__|__\\|   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s       v       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s               UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s               %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Debian") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     ,---._     KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   /\\`  __ \\\\   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  |   /    |    CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  |   \\`.__.\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   \\            SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    \\`-,_       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Devuan") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  -.,              OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     \\`'-.,        KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s          \\`':.    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s             ::    CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s        __--\\`:\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   _,--\\` _.-\\`    SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  :_,--\\`\\`        PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Elementary") != NULL) {
        printf("%s               %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s               OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    _______    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   /  ___  \\   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  /  |  /  /\\  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  |__\\ /  / |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  \\   /__/  /  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s   \\_______/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s               UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s               %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Fedora") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s        _____     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       /   __)\\   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s       |  /  \\ \\  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    ___|  |__/ /  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   / (_    _)_/   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  / /  |  |       SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\ \\__/  |       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s   \\(_____/       UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Gentoo") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s    .-----.       OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  .\\`    _  \\`.   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  \\`.   ()   \\`.  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    \\`.        /  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   .\\`       .\\`  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  /       .\\`     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\____.-\\`       PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "GNew") != NULL) {
        printf("%s                      %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                      OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     ________         KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   .\\`__.---.__\\`.    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  / /  |  *|  \\ \\     CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  | |  \\___/  | |     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  \\ \\         / /     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s   \\`.\\`       \\`.\\`  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                      UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                      %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Guix") != NULL) {
        printf("%s                      %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  |.__          __.|  OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  |__ \\        / __|  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s     \\ \\      / /     MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s      \\ \\    / /      CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s       \\ \\  / /       RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s        \\ \\/ /        SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s         \\__/         PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                      UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                      %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Hyperbola") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      |\\`__.\\`/   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s      \\____/      KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s      .--.        MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /    \\       CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    /  ___ \\      RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   / .\\`   \\`.\\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /.\\`      \\`.\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Instant") != NULL) {
        printf("%s                 %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                 OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s   ,-''-,        KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  : .''. :       MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  : ',,' :       CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   '-____:__     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s         :  \\`.  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s         \\`._.'  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                 UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                 %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Lite") != NULL) {
        printf("%s          %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s     /\\   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    /  \\  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   / / /  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  > / /   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  \\ \\ \\   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   \\_\\_\\  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s      \\   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s          UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s          %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Mint") != NULL || (strstr(sysinfo.os, "LMDE") != NULL)) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s   _____________   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  |_            \\  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s    |  |  ___   |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    |  | | | |  |  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    |  | | | |  |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s    |   \\____/  |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    \\__________/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Mageia") != NULL) {
        printf("%s            %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s     *      OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s      *     KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s     **     MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   /\\__/\\   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  /      \\  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  \\      /  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s   \\____/   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s            UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s            %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Manjaro") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  ||||||||| ||||  OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  ||||||||| ||||  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  ||||      ||||  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  |||| |||| ||||  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  |||| |||| ||||  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  |||| |||| ||||  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |||| |||| ||||  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "MX") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      \\\\  /     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s       \\\\/      KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s        \\\\      MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s     /\\/ \\\\     CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    /  \\  /\\    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   /    \\/  \\   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  /__________\\  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Nix") != NULL) {
        printf("%s                  %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s    \\   \\ //      OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s   ==\\___\\/ //    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s     //    \\//    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  ==//      //==  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   //\\____//      RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  // /\\   \\==     SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    // \\   \\      PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Wrt") != NULL) {
        printf("%s                     %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     _____           KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  .-- ___ --.        MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   .-- _ --.         CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s    .-- --.          RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s       0             SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s                     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                     UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                     %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Parabola") != NULL) {
        printf("%s                     %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    __ __ __  _      KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  .\\`_//_//_/ / \\`.  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s            /  .\\`   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s           / .\\`     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s          /.\\`       SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s         /\\`         PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                     UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                     %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Pop") != NULL) {
        printf("%s                     %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  ______             OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  \\   _ \\        __  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   \\ \\ \\ \\      / /  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    \\ \\_\\ \\    / /   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s     \\  ___\\  /_/    RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s      \\ \\    _       SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s     __\\_\\__(_)_     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s    (___________)    UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                     %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Pure") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                   OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s   _____________   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  |  _________  |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  | |         | |  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  | |         | |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  | |_________| |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |_____________|  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UP TIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Raspbian") != NULL) {
        printf("%s              %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s              OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    __  __    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   (_\\)(/_)   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   (_(__)_)   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  (_(_)(_)_)  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   (_(__)_)   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s     (__)     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Slack") != NULL) {
        printf("%s                 %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s     ________    OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s    /  ______|   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s    | |______    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    \\______  \\   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s     ______| |   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  | |________/   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |____________  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                 UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                 %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "SUSE") != NULL) {
        printf("%s                 %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      _______    OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  -___|   __ \\   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s         / .\\ \\  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s         \\__/ |  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s       _______|  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s       \\_______  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  --__________/  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                 UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                 %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Ubuntu") != NULL) {
        printf("%s                %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s                OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s           _    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s       ---(_)   MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s   _/  ---  \\   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  (_) |   |     RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s    \\  --- _/   SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s       ---(_)   PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Void") != NULL) {
        printf("%s                   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      _______      OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s      \\_____ \\`-   KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s   /\\   ___ \\`- \\  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  | |  /   \\  | |  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  | |  \\___/  | |  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s   \\ \\`-_____  \\/  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s    \\`-______\\     PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                   UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                   %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else if (strstr(sysinfo.os, "Voyager") != NULL) {
        printf("%s   _____   ____   %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s  |     | |    |  OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s  |     | |    |  KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s  |     | |    |  MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s  |     | |____|  CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s  |     | _____   RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  |     ||     |  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  |_____||_____|  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s                  UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s                  %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    } else {
        printf("%s              %s%s@%s%s%s\n", bold, sysinfo.username, reset, bold, sysinfo.hostname, reset);
        printf("%s      ___     OS%s     %s%s\n", bold, reset, sysinfo.os, reset);
        printf("%s     (.. |    KERNEL%s %s%s\n", bold, reset, sysinfo.kernel, reset);
        printf("%s     (<> |    MODEL%s  %s %s%s\n", bold, reset, sysinfo.modelname, sysinfo.modelversion, reset);
        printf("%s    / __  \\   CPU%s    %s @ %d%s\n", bold, reset, sysinfo.cpu, sysinfo.core, reset);
        printf("%s   ( /  \\ /|  RAM%s    %dM / %dM%s\n", bold, reset, sysinfo.ramused, sysinfo.ramtotal, reset);
        printf("%s  _/\\ __)/_)  SHELL%s  %s%s\n", bold, reset, sysinfo.shell, reset);
        printf("%s  \\|/-___\\|/  PKGS%s   ", bold, reset);
        printpkgs();
        printf("%s              UPTIME%s %dd, %dh, %dm%s\n", bold, reset, sysinfo.day, sysinfo.hour, sysinfo.min, reset);
        printf("%s              %s██%s██%s██%s██%s██%s██%s██%s██%s\n\n", bold, black, red, green, yellow, blue, magenta, cyan, white, reset);
    }

    return 0;
}
