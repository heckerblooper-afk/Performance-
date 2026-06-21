#pragma once

#include <jni.h>
#include <string_view>

namespace zygisk {

namespace api {

class Api {
public:
    virtual ~Api() = default;
    virtual int getApiVersion() const = 0;
    virtual void log(const char *fmt, ...) = 0;
    virtual void hookJniNativeMethods(const char *className,
                                      const JNINativeMethod *methods,
                                      int methodCount) = 0;
    virtual void pltHookRegister(const char *libraryName,
                                 const char *symbolName,
                                 void *newFunc,
                                 void **oldFunc) = 0;
    virtual void pltHookCommit() = 0;
};

}  // namespace api

class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual void onLoad(api::Api *api, JNIEnv *env) = 0;
    virtual void preAppSpecialize(api::Api *api, const char *appDir) = 0;
    virtual void postAppSpecialize(api::Api *api) = 0;
};

}  // namespace zygisk
