#include "cpu_tuning.hpp"
#include <fstream>
#include <unistd.h>
#include <android/log.h>

#define TAG "CpuTuning"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)

// ============================================================================
// CPU Frequency Paths (device-specific, adjust for your hardware)
// ============================================================================

static const char *CPU_FREQ_MIN_PATHS[] = {
    "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq",
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_min_freq",
    nullptr
};

static const char *CPU_FREQ_MAX_PATHS[] = {
    "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq",
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_max_freq",
    nullptr
};

static const char *CPU_GOVERNOR_PATHS[] = {
    "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor",
    nullptr
};

static const char *SCHEDTUNE_BOOST_PATH = "/dev/stune/top-app/schedtune.boost";
static const char *UCLAMP_MAX_PATH = "/proc/sys/kernel/sched_util_clamp_max";

// ============================================================================
// Utility: Write to sysfs
// ============================================================================

static bool writeSysfs(const char *path, const char *value) {
    if (!path || !value) return false;
    
    std::ofstream file(path);
    if (!file.is_open()) {
        LOGW("Cannot write to: %s", path);
        return false;
    }
    
    file << value;
    file.close();
    LOGI("Wrote to %s: %s", path, value);
    return true;
}

static const char *findValidPath(const char **paths) {
    for (int i = 0; paths[i] != nullptr; i++) {
        if (access(paths[i], W_OK) == 0) {
            return paths[i];
        }
    }
    return nullptr;
}

// ============================================================================
// CPU Tuning Implementation
// ============================================================================

void CpuTuning::setCpuFrequency(uint32_t min_freq, uint32_t max_freq) {
    const char *min_path = findValidPath(CPU_FREQ_MIN_PATHS);
    const char *max_path = findValidPath(CPU_FREQ_MAX_PATHS);
    
    if (min_path) {
        writeSysfs(min_path, std::to_string(min_freq).c_str());
    }
    if (max_path) {
        writeSysfs(max_path, std::to_string(max_freq).c_str());
    }
}

void CpuTuning::setCpuGovernor(const std::string &governor) {
    const char *gov_path = findValidPath(CPU_GOVERNOR_PATHS);
    if (gov_path) {
        writeSysfs(gov_path, governor.c_str());
    }
}

void CpuTuning::setSchedtune(int32_t boost) {
    if (access(SCHEDTUNE_BOOST_PATH, W_OK) == 0) {
        writeSysfs(SCHEDTUNE_BOOST_PATH, std::to_string(boost).c_str());
    }
}

void CpuTuning::setUclamp(int32_t min, int32_t max) {
    if (access(UCLAMP_MAX_PATH, W_OK) == 0) {
        writeSysfs(UCLAMP_MAX_PATH, std::to_string(max).c_str());
    }
}
