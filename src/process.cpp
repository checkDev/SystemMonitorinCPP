#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"
#include <iostream>
#include <cassert>
using namespace LinuxParser;

using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid)
{
    this->processId = pid;
}
// TODO: Return this process's ID
int Process::Pid() { return processId; }

// TODO: Return this process's CPU utilization

float Process::CpuUtilization() const {
    return cpuPERCENTAGE;
}
float Process::CpuUtilization(float val){
    cpuPERCENTAGE = val;
    return cpuPERCENTAGE;
    
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(processId);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(processId);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return   LinuxParser::User(processId);
  

}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(processId);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    
    return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process const& a) const {
    
    return CpuUtilization() > a.CpuUtilization();
}
