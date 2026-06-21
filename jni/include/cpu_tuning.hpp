#pragma once

#include <cstdint>
#include <string>

class CpuTuning {
public:
    void setCpuFrequency(uint32_t min_freq, uint32_t max_freq);
    void setCpuGovernor(const std::string &governor);
    void setSchedtune(int32_t boost);
    void setUclamp(int32_t min, int32_t max);
};
