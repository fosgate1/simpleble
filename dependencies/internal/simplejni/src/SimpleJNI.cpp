#include <jni.h>
#include <mutex>
#include <android/log.h>

// Global storage that can be accessed across shared library boundaries
static JavaVM* g_javavm = nullptr;
static std::mutex g_javavm_mutex;

extern "C" {
    // Function to set JavaVM from React Native module - make it exported
    __attribute__((visibility("default"))) 
    void set_global_javavm(JavaVM* vm) {
        std::lock_guard<std::mutex> lock(g_javavm_mutex);
        g_javavm = vm;
        __android_log_print(ANDROID_LOG_INFO, "SimpleBLE", "Global JavaVM set: %p", vm);
    }
    
    // Function to get JavaVM for SimpleBLE library - make it exported  
    __attribute__((visibility("default")))
    JavaVM* get_global_javavm() {
        std::lock_guard<std::mutex> lock(g_javavm_mutex);
        __android_log_print(ANDROID_LOG_INFO, "SimpleBLE", "Global JavaVM requested: %p", g_javavm);
        return g_javavm;
    }
} 