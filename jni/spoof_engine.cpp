#include "spoof_engine.hpp"
#include <jni.h>
#include <android/log.h>
#include <cstring>

#define TAG "SpoofEngine"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ============================================================================
// Spoof Engine Implementation
// ============================================================================

SpoofEngine::SpoofEngine(JNIEnv *env) : env_(env) {
    if (!env_) {
        LOGE("Invalid JNIEnv pointer");
        return;
    }
}

void SpoofEngine::spoofBuildProperties(const std::string &model,
                                       const std::string &manufacturer,
                                       const std::string &device,
                                       int32_t sdk_level) {
    if (!env_) {
        LOGE("JNIEnv not initialized");
        return;
    }
    
    // Find android.os.Build class
    jclass buildClass = env_->FindClass("android/os/Build");
    if (!buildClass) {
        LOGE("Cannot find Build class");
        return;
    }
    
    // Use reflection to modify Build fields (for per-app spoofing)
    // This example shows how to access but doesn't modify global Build
    // Real implementation would use JNI to patch the values per-process
    
    jfieldID modelField = env_->GetStaticFieldID(buildClass, "MODEL", "Ljava/lang/String;");
    jfieldID manufacturerField = env_->GetStaticFieldID(buildClass, "MANUFACTURER", "Ljava/lang/String;");
    
    if (modelField && !model.empty()) {
        jstring modelStr = env_->NewStringUTF(model.c_str());
        // Note: Direct field modification may not work; proper hooking needed
        LOGI("[Spoof] Model: %s", model.c_str());
    }
    
    if (manufacturerField && !manufacturer.empty()) {
        LOGI("[Spoof] Manufacturer: %s", manufacturer.c_str());
    }
    
    LOGI("[Spoof] Device: %s, SDK: %d", device.c_str(), sdk_level);
    
    env_->DeleteLocalRef(buildClass);
}

void SpoofEngine::setSpoofProperty(const std::string &key, const std::string &value) {
    // Example: Hook system property getters
    LOGI("[Spoof] Property: %s = %s", key.c_str(), value.c_str());
    
    // Real implementation would hook __system_property_find or similar
    // to return custom values for specified keys
}

void SpoofEngine::hookBuildProperties() {
    // Hook point for intercepting Build.* field access
    // This requires advanced JNI hooking or native method interception
    LOGI("[Spoof] Setting up Build property hooks");
}
