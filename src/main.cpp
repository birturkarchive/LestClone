#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <windows.h>


class LestCore {
   public:
   
   std::string arg;

   void cls() {
        system("cls");
    }

void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }


    std::vector<std::string> getIgnoredFiles() {
    std::vector<std::string> ignored;
    std::ifstream ignoreFile(".LCignore");
    std::string line;
    if (ignoreFile.is_open()) {
        while (std::getline(ignoreFile, line)) {
            if (!line.empty()) {
                ignored.push_back(line);
            }
        }
        ignoreFile.close();
    } else {
        
        ignored.push_back("*.zip");
        ignored.push_back("lestclone.exe");
    }
    return ignored;
}

    void clone() {
    std::string major, minor, patch;
    std::cout << "write major here: ";
    std::getline(std::cin, major);
    std::cout << "write minor here: ";
    std::getline(std::cin, minor);
    std::cout << "write patch here: ";
    std::getline(std::cin, patch);

    std::string zipName = major + "." + minor + "." + patch + ".zip";

    std::vector<std::string> ignored = getIgnoredFiles();
    std::string excludeList = "";
    for (size_t i = 0; i < ignored.size(); ++i) {
        excludeList += ignored[i];
        if (i < ignored.size() - 1) excludeList += ",";
    }

    
    std::string command = "powershell \"Get-ChildItem -Path * -Exclude " + excludeList + " | Compress-Archive -DestinationPath " + zipName + " -Force\"";
    
    system(command.c_str());
    
    std::cout << "[LestClone] Successfully cloned to " << zipName << " (Ignored: " << excludeList << ")" << std::endl;
}

void commit() {
    std::string message;
    std::cout << "write commit here: ";
    std::getline(std::cin, message);

    std::time_t now = std::time(0);
    char dt[26];
    ctime_s(dt, sizeof(dt), &now);
    std::string timestamp(dt);
    timestamp.pop_back();

    std::ofstream logFile("lc.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << timestamp << "] " << message << std::endl;
        logFile.close();
        std::cout << "[LestClone] Commit saved to lc.log" << std::endl;
    } else {
        std::cerr << "[Error] Could not open lc.log" << std::endl;
    }
}

void showLog() {
    std::ifstream logFile("lc.log");
    std::string line;
    if (logFile.is_open()) {
        std::cout << "\n--- LESTCLONE HISTORY ---" << std::endl;
        while (std::getline(logFile, line)) {
            std::cout << line << std::endl;
        }
        std::cout << "-------------------------\n" << std::endl;
        logFile.close();
    } else {
        std::cout << "[LestClone] No log file found yet." << std::endl;
    }
}

void read(std::string fileName) {
        std::ifstream file(fileName);
        std::string line;
        
        if (file.is_open()) {
            setColor(6); // Yellow
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
            setColor(7);
            file.close();
        } else {
            setColor(12);
            std::cout << "Error: Could not open file '" + fileName + "'" << std::endl;
            setColor(7);
        }
    }
void unclone() {
    std::string version;
    std::cout << "write version to restore (e.g. 1.0.0): ";
    std::getline(std::cin, version);
    std::string zipName = version + ".zip";
    
    if (std::filesystem::exists(zipName)) {
        std::string command = "powershell Expand-Archive -Path " + zipName + " -DestinationPath . -Force";
        system(command.c_str());
        std::cout << "[LestClone] Successfully restored version " << version << std::endl;
    } else {
        std::cout << "[Error] Version not found." << std::endl;
    }
}    


};


int main() {
    LestCore lestCore;
    
    lestCore.cls();
    std::cout << "LESTCLONE - A BASIC VERSION CONTROL SYSTEM" << std::endl;
    std::cout << "ALL RIGHTS RESERVED - BIRTURK." << std::endl;
    std::cout << "You can type 'help' to see all commands." << std::endl;
    while(true) {
        std::cout << "LESTCLONE> ";
        std::getline(std::cin, lestCore.arg);
        if (lestCore.arg.empty()) continue;
        else if(lestCore.arg == "help") {
            std::cout << "Available commands: help, exit, cls, lc clone, lc commit, lc showlog, lc unclone" << std::endl;
        } else if(lestCore.arg == "exit") {
            break;
        } else if(lestCore.arg == "cls") {
            lestCore.cls();
        } else if(lestCore.arg == "lc clone") {
            lestCore.clone();
        } else if(lestCore.arg == "lc commit") {
            lestCore.commit();
        }  else if(lestCore.arg == "lc showlog") {
            lestCore.showLog();
        } else if(lestCore.arg == "lc unclone") {
            lestCore.unclone();
        }
        else if(lestCore.arg == "easter") {
            lestCore.read("pardus.txt");
        }
         else {
            std::cout << "Unknown command. Type 'help' to see all commands." << std::endl;
        }
    }
    return 0;
}
