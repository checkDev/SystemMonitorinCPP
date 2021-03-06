#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long seconds);  // TODO: See src/format.cpp
std::string FillWithNLeadingSigns(const std::string& s, int minSize,
                                  char sign);
};                                    // namespace Format

#endif
