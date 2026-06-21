#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <cstring>
#include <cstdlib>
#include "perf_suite.hpp"
#include "config_parser.hpp"
#include "cpu_tuning.hpp"
#include "spoof_engine.hpp"
#include "fps_unlock.hpp"

#define TAG "ZygiskPerfSuite"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Module API instance
static PerfSuiteModule *g_module = nullptr;

// ============================================================================
// Module Lifecycle
// ============================================================================

void PerfSuiteModule::onLoad(zygisk::api::Api *api, JNIEnv *env) {
    LOGI("[PerfSuite] Module loaded (Zygisk API v%d)", api->getApiVersion());
    
    api_ = api;
    env_ = env;
    
    // Initialize any global hooks or JNI bindings here
    // Example: Hook system property getters for global spoofing
}

void PerfSuiteModule::preAppSpecialize(zygisk::api::Api *api, const char *appDir) {
    LOGI("[PerfSuite] Pre-app specialize: %s", appDir);
    
    // Placeholder: Extract package name from appDir
    // Real implementation would parse appDir path
    const char *pkg = "/data/app/com.example.game/";
    if (strstr(appDir, "com.example")) {
        current_package_name_ = "com.example.game1";
        loadAppConfig(current_package_name_);
        LOGI("[PerfSuite] Loaded config for: %s", current_package_name_.c_str());
    }
}

void PerfSuiteModule::postAppSpecialize(zygisk::api::Api *api) {
    if (!app_config_) return;
    
    LOGI("[PerfSuite] Post-app specialize");
    
    // Apply all tweaks after app is fully initialized
    applyPerformanceTweaks();
    applySpoofing();
    setupFpsUnlock();
    
    LOGI("[PerfSuite] All tweaks applied for: %s", current_package_name_.c_str());
}

// ============================================================================
// Config Loading
// ============================================================================

void PerfSuiteModule::loadAppConfig(const std::string &package_name) {
    ConfigParser parser;
    app_config_ = std::make_shared<AppConfig>();
    app_config_->package_name = package_name;
    
    // Load global config first
    std::string global_path = "/data/adb/modules/zygisk_perf_suite/config/global.conf";
    if (!parser.parseFile(global_path, *app_config_)) {
        LOGW("[PerfSuite] Failed to load global config");
    }
    
    // Load app-specific config
    std::string app_path = "/data/adb/modules/zygisk_perf_suite/config/apps/" + 
                          package_name + ".conf";
    if (!parser.parseFile(app_path, *app_config_)) {
        LOGI("[PerfSuite] No app-specific config found; using defaults");
    }
}

// ============================================================================
// Apply Tweaks
// ============================================================================

void PerfSuiteModule::applyPerformanceTweaks() {
    if (!app_config_) return;
    
    CpuTuning cpu_tuner;
    
    // Apply CPU tuning
    LOGI("[PerfSuite] Applying CPU tuning: governor=%s, min=%u, max=%u",
         app_config_->cpu.governor.c_str(),
         app_config_->cpu.min_freq,
         app_config_->cpu.max_freq);
    
    cpu_tuner.setCpuGovernor(app_config_->cpu.governor);
    cpu_tuner.setCpuFrequency(app_config_->cpu.min_freq, app_config_->cpu.max_freq);
    cpu_tuner.setSchedtune(app_config_->cpu.schedtune_boost);
    cpu_tuner.setUclamp(app_config_->cpu.uclamp_min, app_config_->cpu.uclamp_max);
}

void PerfSuiteModule::applySpoofing() {
    if (!app_config_ || app_config_->spoof.model.empty()) return;
    
    SpoofEngine spoof_engine(env_);
    
    LOGI("[PerfSuite] Applying device spoofing: model=%s, manufacturer=%s",
         app_config_->spoof.model.c_str(),
         app_config_->spoof.manufacturer.c_str());
    
    spoof_engine.spoofBuildProperties(
        app_config_->spoof.model,
        app_config_->spoof.manufacturer,
        app_config_->spoof.device,
        app_config_->spoof.sdk_level
    );
    
    // Apply game-specific properties
    for (const auto &prop : app_config_->spoof.game_props) {
        spoof_engine.setSpoofProperty(prop.first, prop.second);
        LOGI("[PerfSuite] Spoof property: %s=%s", prop.first.c_str(), prop.second.c_str());
    }
}

void PerfSuiteModule::setupFpsUnlock() {
    if (!app_config_ || !app_config_->fps.enabled) return;
    
    FpsUnlock fps_unlocker;
    
    LOGI("[PerfSuite] Setting up FPS unlock: target=%d fps", 
         app_config_->fps.target_fps);
    
    fps_unlocker.initialize(app_config_->fps.game_engine);
    fps_unlocker.setTargetFps(app_config_->fps.target_fps);
    fps_unlocker.hookGameLibraries(app_config_->fps.library_hooks);
}

// ============================================================================
// Module Entry Point
// ============================================================================

extern "C" {
    zygisk::ModuleBase *createModule() {
        if (!g_module) {
            g_module = new PerfSuiteModule();
        }
        return g_module;
    }
}
