//
// Created by CHRISTNA on 2023/6/29.
//

#ifndef AAUDIOPLAYER_LOG_H
#define AAUDIOPLAYER_LOG_H
#include <android/log.h>
#define TAG "AAudioPlayer"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);

#endif //AAUDIOPLAYER_LOG_H
