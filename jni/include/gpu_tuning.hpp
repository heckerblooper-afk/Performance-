#pragma once

#include <cstdint>

class GpuTuning {
public:
    void setGpuFrequency(uint32_t min_freq, uint32_t max_freq);
};
