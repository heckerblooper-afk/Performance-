#include "gpu_tuning.hpp"
#include <fstream>
#include <android/log.h>

#define TAG "GpuTuning"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)

// ============================================================================
// GPU Frequency Paths (Adreno - adjust for your device)
// ============================================================================

static const char *GPU_FREQ_MIN_PATHS[] = {
    "/sys/class/kgsl/kgsl-3d0/min_pwrlevel",
    "/sys/devices/platform/soc/5000000.gpu/devfreq/5000000.gpu/min_freq",
    nullptr
};

static const char *GPU_FREQ_MAX_PATHS[] = {
    "/sys/class/kgsl/kgsl-3d0/max_pwrlevel",
    "/sys/devices/platform/soc/5000000.gpu/devfreq/5000000.gpu/max_freq",
    nullptr
};

// ============================================================================
// Utility
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
    LOGI("GPU Sysfs: %s = %s", path, value);
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
// GPU Tuning Implementation
// ============================================================================

void GpuTuning::setGpuFrequency(uint32_t min_freq, uint32_t max_freq) {
    const char *min_path = findValidPath(GPU_FREQ_MIN_PATHS);
    const char *max_path = findValidPath(GPU_FREQ_MAX_PATHS);
    
    if (min_path) {
        writeSysfs(min_path, std::to_string(min_freq).c_str());
    }
    if (max_path) {
        writeSysfs(max_path, std::to_string(max_freq).c_str());
    }
}
