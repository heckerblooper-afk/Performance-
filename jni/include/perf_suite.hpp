#pragma once

#include <string>
#include <map>
#include <memory>
#include <jni.h>
#include "zygisk.hpp"

// ============================================================================
// DATA STRUCTURES
// ============================================================================

struct CpuConfig {
    uint32_t min_freq = 300000;      // kHz
    uint32_t max_freq = 2880000;     // kHz
    std::string governor = "schedutil";
    int32_t schedtune_boost = 0;     // 0-100
    int32_t uclamp_min = 0;          // 0-1024
    int32_t uclamp_max = 1024;       // 0-1024
};

struct GpuConfig {
    uint32_t min_freq = 100;         // MHz
    uint32_t max_freq = 800;         // MHz
};

struct SpoofConfig {
    std::string model = "";
    std::string manufacturer = "";
    std::string device = "";
    int32_t sdk_level = -1;
    std::map<std::string, std::string> game_props;
};

struct FpsConfig {
    bool enabled = false;
    int32_t target_fps = 120;
    int32_t hook_priority = 10;
    std::string game_engine = "unity";
    std::string library_hooks = "libUnity.so,libil2cpp.so";
};

struct MemoryConfig {
    int32_t min_available_memory = 512;  // MB
    bool enable_mem_tuning = true;
};

struct PowerConfig {
    std::string power_mode = "balanced";  // balanced, performance, power_saving
};

struct AppConfig {
    std::string package_name = "";
    CpuConfig cpu;
    GpuConfig gpu;
    SpoofConfig spoof;
    FpsConfig fps;
    MemoryConfig memory;
    PowerConfig power;
};

// ============================================================================
// MAIN MODULE CLASS
// ============================================================================

class PerfSuiteModule : public zygisk::ModuleBase {
public:
    void onLoad(zygisk::api::Api *api, JNIEnv *env) override;
    void preAppSpecialize(zygisk::api::Api *api, const char *appDir) override;
    void postAppSpecialize(zygisk::api::Api *api) override;

private:
    zygisk::api::Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
    std::string current_package_name_;
    std::shared_ptr<AppConfig> app_config_;

    void loadAppConfig(const std::string &package_name);
    void applyPerformanceTweaks();
    void applySpoofing();
    void setupFpsUnlock();
};
