#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include<iomanip>

using std::set;
using std::size_t;
using std::string;
using std::vector;
//using LinuxParser;


// TODO: Return the system's CPU
Processor& System::Cpu() {
    
    return cpu_;
   
    
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int>pids(LinuxParser::Pids());
    
    set<int> extent_pids;
    for(Process &process : processes_)
    {
        extent_pids.insert(process.Pid());
        
    }
    for(int pid : pids)
    {
        if(extent_pids.find(pid) == extent_pids.end())
        {
            processes_.emplace_back(pid);
        }
    }
    
    for(auto &process : processes_)
    {
        auto activeJiffies = LinuxParser::ActiveJiffies(process.Pid());
        auto totalJiffies = LinuxParser::totalseconds(process.Pid());
        float cpuUtilization = static_cast<float>(activeJiffies )/totalJiffies ;
        process.CpuUtilization(cpuUtilization);
    }
  std::sort(processes_.begin() , processes_.end() , std::greater<>());
    return processes_;
    
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
    
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {  return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return   LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

System::System()
{
  /*
    auto pids = LinuxParser::Pids();
    processes_.reserve(pids.size());
    int count = 0;
    for(auto pid : pids)
    {
        processes_[count++].setPid(pid);
    }
    */
}
