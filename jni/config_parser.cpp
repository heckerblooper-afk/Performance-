#include "config_parser.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <android/log.h>

#define TAG "ConfigParser"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ============================================================================
// Utility Functions
// ============================================================================

static std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

static bool parseKeyValue(const std::string &line, std::string &key, std::string &value) {
    size_t delim = line.find('=');
    if (delim == std::string::npos) return false;
    
    key = trim(line.substr(0, delim));
    value = trim(line.substr(delim + 1));
    return !key.empty();
}

// ============================================================================
// Config Parser Implementation
// ============================================================================

bool ConfigParser::parseFile(const std::string &filepath, AppConfig &config) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOGE("Failed to open config file: %s", filepath.c_str());
        return false;
    }
    
    std::string line;
    int line_num = 0;
    
    while (std::getline(file, line)) {
        line_num++;
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        std::string key, value;
        if (!parseKeyValue(line, key, value)) {
            LOGE("Invalid line %d in %s", line_num, filepath.c_str());
            continue;
        }
        
        applyConfigValue(key, value, config);
    }
    
    file.close();
    LOGI("Parsed config from: %s", filepath.c_str());
    return true;
}

void ConfigParser::applyConfigValue(const std::string &key, 
                                     const std::string &value,
                                     AppConfig &config) {
    // CPU Configuration
    if (key == "cpu_min_freq" || key == "default_cpu_min_freq") {
        config.cpu.min_freq = std::stoul(value);
    }
    else if (key == "cpu_max_freq" || key == "default_cpu_max_freq") {
        config.cpu.max_freq = std::stoul(value);
    }
    else if (key == "cpu_governor" || key == "default_cpu_governor") {
        config.cpu.governor = value;
    }
    else if (key == "schedtune_boost" || key == "default_schedtune_boost") {
        config.cpu.schedtune_boost = std::stoi(value);
    }
    else if (key == "uclamp_min" || key == "default_uclamp_min") {
        config.cpu.uclamp_min = std::stoi(value);
    }
    else if (key == "uclamp_max" || key == "default_uclamp_max") {
        config.cpu.uclamp_max = std::stoi(value);
    }
    // GPU Configuration
    else if (key == "gpu_min_freq" || key == "default_gpu_min_freq") {
        config.gpu.min_freq = std::stoul(value);
    }
    else if (key == "gpu_max_freq" || key == "default_gpu_max_freq") {
        config.gpu.max_freq = std::stoul(value);
    }
    // Spoof Configuration
    else if (key == "spoof_model") {
        config.spoof.model = value;
    }
    else if (key == "spoof_manufacturer") {
        config.spoof.manufacturer = value;
    }
    else if (key == "spoof_device") {
        config.spoof.device = value;
    }
    else if (key == "spoof_sdk") {
        config.spoof.sdk_level = std::stoi(value);
    }
    else if (key == "game_props") {
        parseGameProps(value, config.spoof.game_props);
    }
    // FPS Configuration
    else if (key == "unlock_fps") {
        config.fps.enabled = (std::stoi(value) == 1);
    }
    else if (key == "target_fps") {
        config.fps.target_fps = std::stoi(value);
    }
    else if (key == "hook_priority") {
        config.fps.hook_priority = std::stoi(value);
    }
    else if (key == "game_engine") {
        config.fps.game_engine = value;
    }
    else if (key == "library_hooks") {
        config.fps.library_hooks = value;
    }
    // Memory Configuration
    else if (key == "min_available_memory") {
        config.memory.min_available_memory = std::stoi(value);
    }
    else if (key == "enable_mem_tuning") {
        config.memory.enable_mem_tuning = (std::stoi(value) == 1);
    }
    // Power Configuration
    else if (key == "power_mode") {
        config.power.power_mode = value;
    }
}

void ConfigParser::parseGameProps(const std::string &props_str,
                                   std::map<std::string, std::string> &props) {
    std::istringstream stream(props_str);
    std::string prop;
    
    while (std::getline(stream, prop, ',')) {
        prop = trim(prop);
        size_t eq_pos = prop.find('=');
        
        if (eq_pos != std::string::npos) {
            std::string key = trim(prop.substr(0, eq_pos));
            std::string value = trim(prop.substr(eq_pos + 1));
            props[key] = value;
        }
    }
}
