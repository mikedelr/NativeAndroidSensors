#ifndef NATIVELOGGER_H
#define NATIVELOGGER_H

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Native-Side", __VA_ARGS__)) //inline debug statements
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Native-Side", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Native-Side", __VA_ARGS__))

#endif
