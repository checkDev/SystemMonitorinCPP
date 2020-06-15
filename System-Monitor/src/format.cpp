#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds) {
    
//     int hour  = seconds/3600;
//     int min  = (seconds - hour*3600)/60;
//     int sec = (seconds - (hour*3600 + min*60 ));
    
//     return std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec);

    
    
// }


std::string Format::FillWithNLeadingSigns(const std::string& s, int minSize,
                                  char sign) {
    auto countOfFills = minSize;
    countOfFills -= static_cast<int>(s.size());
    if (countOfFills > 0) {
        return std::string(static_cast<std::size_t>(countOfFills), sign) + s;
    }
    return s;
}

std::string Format::ElapsedTime(long seconds) {
    auto minutes = seconds / 60;
    seconds -= minutes * 60;
    
    auto hours = minutes / 60;
    minutes -= hours * 60;
    
    if (hours > 99) {
        hours = 99;
    }
    
    return FillWithNLeadingSigns(std::to_string(hours), 2, '0') + ":" +
    FillWithNLeadingSigns(std::to_string(minutes), 2, '0') + ":" +
    FillWithNLeadingSigns(std::to_string(seconds), 2, '0');
}
