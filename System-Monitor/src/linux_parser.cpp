#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <cassert>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
    string os, kernel ,version;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    float memtotal = 0, memoryfree =0 ;
    string value ;
    string memtotalV = "", memoryfreekeyV ="" ;
    string line;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line))
        {
            //std::cout << " LinuxParser::MemoryUtilization entry!" << std::endl ;
            std::istringstream linestream(line);
            linestream >> memtotalV >> value ;
            if(memtotalV == "MemTotal:")
            {
               //std::cout << " MemTotal " << value << std::endl;
              
                memtotal = stod(value);
            }
            else if(memtotalV == "MemFree:")
            {
                //std::cout << " MemFree " << value << std::endl;
                memoryfree = stod(value);
                //std::cout << " LinuxParser::MemoryUtilization exit!" << std::endl ;
                  std::setprecision(4);
                return (static_cast<float>(memtotal - memoryfree)/memtotal);
            }
        }
    }
   // std::cout << " LinuxParser::MemoryUtilization exit!" << std::endl ;
    return memtotal;
    
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string upTime;
    string line;
    try {
        
        std::ifstream stream(kProcDirectory + kUptimeFilename);
        if (stream.is_open()) {
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> upTime;
            if(!upTime.empty())
                return stol(upTime);
        }
    }catch(const std::exception  &e)
    {
        std::cout << "LinuxParser::UpTime()" <<  std:: endl;
        std::cout << " LinuxParser::UpTime()" << e.what();
        return   0;
    }
    
    return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    
    return sysconf(_SC_CLK_TCK) * UpTime();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::totalseconds(int pid) {
   long seconds = (UpTime(pid)* sysconf(_SC_CLK_TCK)) - (UpTime() / sysconf(_SC_CLK_TCK)) ;
  return seconds;
}
long LinuxParser::ActiveJiffies(int pid) {
    long activieJiffyPid = 0;
    string line;
    //  std::ifstream stream(kProcDirectory + "/" + to_string(pid) +"/" + kStatusFilename);
    std::ifstream stream(kProcDirectory + "/" +  to_string(pid) +"/" + kStatFilename);
    int count = 1;
    try{
        if(stream.is_open())
        {
            getline(stream , line);
            string token ;
            std::istringstream newstream(line);
            while(getline(newstream ,token , ' ' ))
            {
                if(token.empty())continue;
                
                if(count >= 14 && count <=17)
                {
                    activieJiffyPid += stol(token);
                }
                count++;
            }
            
        }
    }catch(const std::exception  &e)
    {
        std::cout << "LinuxParser::ActiveJiffies()" <<  std:: endl;
        std::cout << "LinuxParser::ActiveJiffies()" << e.what();
        return   activieJiffyPid;
    }
 // return   activieJiffyPid;
    return   activieJiffyPid/sysconf(_SC_CLK_TCK) ;
    
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    
    long activeJiffies = 0;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    try {
        if(stream.is_open())
        {
            getline(stream , line );
            //std::cout << " line :  " << line << std::endl;
            std::istringstream linestream(line);
            string token;
            while(getline(linestream , token , ' '))
            {
                if(token.empty())continue;
                //std::cout  << std::endl << token << std::endl;
                if( token != "cpu")
                        activeJiffies += stol(token);
            }
        }
    }
    catch(const std::exception  &e)
    {
        std::cout << "LinuxParser::ActiveJiffies --" <<  std:: endl;
        std::cout << " LinuxParser::ActiveJiffies()" << e.what();
        return   activeJiffies;
    }
    
    return activeJiffies;
    
    
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    long idleJiffies = 0;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    try{
        if(stream.is_open())
        {
            getline(stream , line );
            std::istringstream linestream(line);
            string token;
            int count = 1;
            while(getline(linestream , token , ' '))
            {
                if(token.empty())continue;
                if(token == "cpu")
                    continue;
                count++;
                if(count == 4 || count == 5 )
                    idleJiffies += stol(token);
                if(count == 5 )
                    break;
            }
        }
    } catch(const std::exception  &e)
    {
         std::cout << "LinuxParser::IdleJiffies " <<  std:: endl;
        std::cout << " LinuxParser::IdleJiffies()" << e.what();
        return   idleJiffies;
    }
    return idleJiffies;
    
    
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    
    
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string processes, value ;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream >> processes >>  value;
            if(processes == "processes")
            {
                return stoi(value);
            }
        }
    }
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    
    string runningProcesses, value ;
    string line , processes;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream >> processes >>  value;
            if(processes == "procs_running")
            {
                return stoi(value);
            }
        }
    }
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string cmdLine;
    std::ifstream stream(kProcDirectory + "/" +  to_string(pid) + kCmdlineFilename);
    if(stream.is_open())
    {
        
        getline(stream , cmdLine);
        
        return cmdLine;
    }
    return   cmdLine;;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    
    std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
    if(stream.is_open())
    {
        string line;
        string key , value;
        while(getline(stream , line))
        {
            std::istringstream ifstream(line);
            ifstream >> key >> value;
            if(key == "VmSize:")
            {
                // kb to mb;
                return to_string(stoi(value)/1000);
            }
            
        }
    }
    return "";
}

string LinuxParser::Uid(int pid) {
    
    std::cout << "Uid start ==>" << pid << std::endl;
    string userId , value;
    string uname ;
    std::ifstream stream(kProcDirectory + "/" + to_string(pid) +"/" + kStatusFilename);
    string line;
    bool isFound = false;
    if(stream.is_open())
    {
        while(!isFound && getline(stream , line))
        {
            if(line.size() >=4 && line.substr(0, 4) == "Uid:")
            {
                line = line.substr(4);
                isFound = true;
                std::istringstream ifstream(line);
                std::string token ;
                while(getline(ifstream , token , ' ' ))
                {
                    if(token != "")
                    {
                        value = token;
                        break;
                    }
                }
                
            }
        }
    }
   // std::cout << "Uid ==>" << value << std::endl;
    return value;
}

string LinuxParser::User(int pid) {
    
    
    string userId ;
    int value;
    string uname ;
    std::ifstream stream(kProcDirectory + "/" + to_string(pid) +"/" +  kStatusFilename);
    string line;
    bool isFound = false;
  std::string  value1 , value2;
    if(stream.is_open())
    {
       //std::cout << std::endl << " FILE OPEN hehe " << std::endl;
        while(!isFound && getline(stream , line))
        {
            if(line.size() >=4 && line.substr(0, 4) == "Uid:")
            {
                line = line.substr(4);
                isFound = true;
                std::istringstream ifstream(line);
                //std::cout << "line =" << line << "****" << std::endl;
                std::string token ;
//                 while(getline(ifstream , token , ' ' ))
//                 {
//                   //std::cout << "get line **token=" << stoi(token) << "****" << std::endl;
//                     if(token != "")
//                     {
//                         value = stoi(token);
//                         break;
//                     }
//                 }
              
              ifstream >> value1 >> value2;
                
            }
        }
    }
   
 

  
   // string tofindUid = ":x:" + to_string(value1);
   string tofindUid = ":x:" + value1;
  
    std::ifstream stream2("/etc/passwd");
    assert(stream2.is_open());
    if(stream2.is_open())
    {
        while(getline(stream2 , line))
        {
            auto pos = line.find(tofindUid);
            if(pos != string::npos)
            {
              
                uname = line.substr(0,pos);
                break;
            }
        }
    }
    assert(uname != "");
    return   uname;
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    
    
    //string userId , value;
    string uname ;
    try {
        std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
        string line , token;
        if(stream.is_open())
        {
            int count = 0;
            getline(stream , line );
            std::istringstream ifstream(line);
            while(getline(ifstream , token , ' '))
            {
                if(token.empty())continue;
                if(++count == 22)
                {
                    long int val = stol(token);
                    return val/sysconf(_SC_CLK_TCK);
                    
                }
                
            }
            
            return 0;
            
        }
    }
    catch(const std::exception  &e)
    {
         std::cout << "LinuxParser::UpTime " <<  std:: endl;
        std::cout << " LinuxParser::UpTime()" << e.what();
        return   0;
    }
    return 0;
}

