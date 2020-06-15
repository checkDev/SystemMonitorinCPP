#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization(float val) ;                  // TODO: See src/process.cpp
  float CpuUtilization() const;
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
    inline  void setPid(int pid){ processId = pid;};
  bool operator>(Process const& a) const;

 

  // TODO: Declare any necessary private members
 private:
    int processId;
    float cpuPERCENTAGE;
};

#endif
