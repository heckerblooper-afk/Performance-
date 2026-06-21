#pragma once

#include <string>
#include <cstdint>

class FpsUnlock {
public:
    void initialize(const std::string &engine);
    void setTargetFps(int32_t fps);
    void hookGameLibraries(const std::string &libraries_str);
    void enableFpsUnlock(bool enable);
    
private:
    std::string game_engine_;
    
    void hookLibrary(const std::string &library_name);
};
