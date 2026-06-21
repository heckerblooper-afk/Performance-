#pragma once

#include <string>
#include <map>
#include <jni.h>

class SpoofEngine {
public:
    explicit SpoofEngine(JNIEnv *env);
    
    void spoofBuildProperties(const std::string &model,
                              const std::string &manufacturer,
                              const std::string &device,
                              int32_t sdk_level);
    
    void setSpoofProperty(const std::string &key, const std::string &value);
    
private:
    JNIEnv *env_;
    
    void hookBuildProperties();
};
