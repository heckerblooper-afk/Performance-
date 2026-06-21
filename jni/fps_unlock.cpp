#include "fps_unlock.hpp"
#include <dlfcn.h>
#include <android/log.h>
#include <cstring>
#include <sstream>

#define TAG "FpsUnlock"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ============================================================================
// FPS Unlock Constants & Function Signatures
// ============================================================================

// Example: Generic FPS frame limiter function signature
// This varies by engine, but typically looks like:
// void limitFrameRate(float targetFps, float deltaTime)
typedef void (*FrameLimiterFunc)(float, float);

// Example: Hook for UnityEngine frame timing
typedef float (*GetDeltaTimeFunc)(void);

// Global frame rate value (used by hooked functions)
static volatile float g_target_fps = 60.0f;
static volatile bool g_fps_unlock_active = false;

// ============================================================================
// Trampolines & Hooks
// ============================================================================

// Trampoline: Original function pointer (placeholder)
static FrameLimiterFunc g_original_frame_limiter = nullptr;

// Hooked version of frame limiter
void hooked_frame_limiter(float targetFps, float deltaTime) {
    if (g_fps_unlock_active) {
        // Override target FPS
        targetFps = g_target_fps;
    }
    
    // Call original if we have it
    if (g_original_frame_limiter) {
        g_original_frame_limiter(targetFps, deltaTime);
    }
    
    LOGI("[FPS Hook] Frame limiter called: target=%f, unlock=%d", targetFps, g_fps_unlock_active);
}

// ============================================================================
// FPS Unlock Implementation
// ============================================================================

void FpsUnlock::initialize(const std::string &engine) {
    game_engine_ = engine;
    LOGI("[FpsUnlock] Initializing for engine: %s", engine.c_str());
}

void FpsUnlock::setTargetFps(int32_t fps) {
    g_target_fps = static_cast<float>(fps);
    LOGI("[FpsUnlock] Target FPS set to: %d", fps);
}

void FpsUnlock::hookGameLibraries(const std::string &libraries_str) {
    // Parse library names (comma-separated)
    std::istringstream stream(libraries_str);
    std::string lib_name;
    
    while (std::getline(stream, lib_name, ',')) {
        // Trim whitespace
        size_t start = lib_name.find_first_not_of(" \t\r\n");
        size_t end = lib_name.find_last_not_of(" \t\r\n");
        if (start != std::string::npos) {
            lib_name = lib_name.substr(start, end - start + 1);
            hookLibrary(lib_name);
        }
    }
}

void FpsUnlock::hookLibrary(const std::string &library_name) {
    LOGI("[FpsUnlock] Attempting to hook library: %s", library_name.c_str());
    
    // Try to load library
    void *handle = dlopen(library_name.c_str(), RTLD_LAZY);
    if (!handle) {
        LOGW("[FpsUnlock] Could not load library: %s (may not be loaded yet)", library_name.c_str());
        return;
    }
    
    // Example: Hook frame limiter function (names vary by engine)
    // For Unity: "_ZN5Unity3D11FrameLimiterEv" or similar mangled name
    // For IL2CPP: Look for game-specific functions
    
    const char *func_names[] = {
        "Frame_Limiter",
        "FrameCapCallback",
        "limitFramerate",
        "setFramerate",
        nullptr
    };
    
    for (int i = 0; func_names[i] != nullptr; i++) {
        FrameLimiterFunc original = (FrameLimiterFunc)dlsym(handle, func_names[i]);
        if (original) {
            LOGI("[FpsUnlock] Found and hooked: %s", func_names[i]);
            g_original_frame_limiter = original;
            g_fps_unlock_active = true;
            dlclose(handle);
            return;
        }
    }
    
    LOGW("[FpsUnlock] No hooking points found in: %s", library_name.c_str());
    dlclose(handle);
}

void FpsUnlock::enableFpsUnlock(bool enable) {
    g_fps_unlock_active = enable;
    LOGI("[FpsUnlock] FPS unlock: %s", enable ? "ENABLED" : "DISABLED");
}
