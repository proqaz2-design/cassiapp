// Copyright © 2023 Cassia Developers, all rights reserved.

#include "cassia/wine_ctx.h"
#include <filesystem>
#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>

std::mutex stateMutex;
std::optional<cassia::WineContext> wineCtx;
ANativeWindow *nativeWindow{nullptr};

extern "C" JNIEXPORT void JNICALL
Java_cassia_app_CassiaManager_startServer(
        JNIEnv *env,
        jobject /* this */,
        jstring jRuntimePath, jstring jPrefixPath, jstring jCassiaExtPath) {
    const char *runtimePathStr{env->GetStringUTFChars(jRuntimePath, nullptr)};
    const char *prefixPathStr{env->GetStringUTFChars(jPrefixPath, nullptr)};
    const char *cassiaExtPathStr{env->GetStringUTFChars(jCassiaExtPath, nullptr)};
    std::filesystem::path runtimePath{runtimePathStr};
    std::filesystem::path prefixPath{prefixPathStr};
    std::filesystem::path cassiaExtPath{cassiaExtPathStr};
    env->ReleaseStringUTFChars(jRuntimePath, runtimePathStr);
    env->ReleaseStringUTFChars(jPrefixPath, prefixPathStr);
    env->ReleaseStringUTFChars(jCassiaExtPath, cassiaExtPathStr);

    {
        std::scoped_lock lock{stateMutex};
        wineCtx.emplace(runtimePath, prefixPath, cassiaExtPath);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_cassia_app_CassiaManager_stopServer(
        JNIEnv *env,
        jobject /* this */) {
    std::scoped_lock lock{stateMutex};
    wineCtx.reset();
}

extern "C" JNIEXPORT void JNICALL
Java_cassia_app_input_InputBridge_pointerEvent(JNIEnv *env, jclass /* cls */, jint pointerId, jdouble x, jdouble y, jint action) {
    __android_log_print(ANDROID_LOG_DEBUG, "cassia.native", "Pointer event id=%d action=%d x=%f y=%f", pointerId, action, x, y);
    std::scoped_lock lock{stateMutex};
    if (wineCtx.has_value()) {
        wineCtx->HandlePointerEvent(static_cast<int>(pointerId), static_cast<double>(x), static_cast<double>(y), static_cast<int>(action));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_cassia_app_input_InputBridge_keyEvent(JNIEnv *env, jclass /* cls */, jint scanCode, jboolean down) {
    __android_log_print(ANDROID_LOG_DEBUG, "cassia.native", "Key event scan=%d down=%d", scanCode, down);
    std::scoped_lock lock{stateMutex};
    if (wineCtx.has_value()) {
        wineCtx->HandleKeyEvent(static_cast<int>(scanCode), static_cast<bool>(down));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_cassia_app_CassiaManager_setSurface(
        JNIEnv *env,
        jobject /* this */,
        jobject surface) {
    std::scoped_lock lock{stateMutex};
    nativeWindow = surface == nullptr ? nullptr : ANativeWindow_fromSurface(env, surface);

    // TODO: Hook up to compositor.
}
