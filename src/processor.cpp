#include "processor.h"
#include "linux_parser.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace LinuxParser;
using std::string;
using std::to_string;
using std::vector;



// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float utilization {0.0};
    long active_ticks = LinuxParser::ActiveJiffies();
    long idle_ticks = LinuxParser::IdleJiffies();
    long duration_active{active_ticks - cachedActiveTicks};
    long duration_idle{active_ticks - cachedIdleTicks};
 
    long duration{duration_active+ duration_idle};
    //std::cout << " activeduration " << duration_active << " duration " << duration << std::endl;
    std::setprecision(4);
    utilization = (static_cast<float>(duration_active) /duration);
    cachedActiveTicks = active_ticks;
    cachedIdleTicks = idle_ticks;
    //std::cout << "utilization "  <<   utilization << std::endl;
 
    return utilization;
}
    

