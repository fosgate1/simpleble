#pragma once

#include <jni.h>
#include <mutex>
#include <android/log.h>

// External functions provided by simplejni library
extern "C" {
    void set_global_javavm(JavaVM* vm);
    JavaVM* get_global_javavm();
}

namespace SimpleJNI {

// Generic class to handle the Java Virtual Machine (JVM)
class VM {
  public:
    // Set the JavaVM (to be called from React Native context)
    static void set_jvm(JavaVM* vm) {
        set_global_javavm(vm);
        __android_log_print(ANDROID_LOG_INFO, "SimpleBLE", "JavaVM set via Global VM: %p", vm);
    }
    
    // Get the stored JavaVM
    static JavaVM* jvm() {
        JavaVM* vm = get_global_javavm();
        __android_log_print(ANDROID_LOG_INFO, "SimpleBLE", "JavaVM retrieved via global VM: %p", vm);
        if (vm == nullptr) {
            throw std::runtime_error("JavaVM not initialized via Global VM - call set_jvm first");
        }
        return vm;
    }

    static bool is_jvm_available() {
        try {
            VM::jvm();
            return true;
        } catch (...) {
            return false;
        }
    }

    static bool is_jvm_alive() {
        JavaVM* jvm = VM::jvm();
        if (!jvm) return false;
        JNIEnv* env = nullptr;
        return jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK;
    }

    static JNIEnv* env() {
        JNIEnv* env = nullptr;
        JavaVM* jvm = VM::jvm();

        auto getEnvResult = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        switch (getEnvResult) {
            case JNI_OK:
                break;
            case JNI_EDETACHED: {
#ifdef __ANDROID__
                auto result = jvm->AttachCurrentThread(&env, nullptr);
#else
                auto result = jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
#endif
                if (result != JNI_OK) {
                    throw std::runtime_error("Failed to attach the current thread to the JVM");
                }
                break;
            }
            case JNI_EVERSION:
                throw std::runtime_error("Failed to get the JNIEnv, JNI version not supported");
            default:
                throw std::runtime_error("Failed to get the JNIEnv, unknown error");
        }
        return env;
    }

    static void attach() {
        JNIEnv* env = nullptr;
        JavaVM* jvm = VM::jvm();
#ifdef __ANDROID__
        auto result = jvm->AttachCurrentThread(&env, nullptr);
#else
        auto result = jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
#endif
        if (result != JNI_OK) {
            throw std::runtime_error("Failed to attach the current thread to the JVM");
        }
    }

    static void detach() {
        JavaVM* jvm = VM::jvm();
        jvm->DetachCurrentThread();
    }

  private:
    VM() = default;
    ~VM() = default;
};

}  // namespace SimpleJNI