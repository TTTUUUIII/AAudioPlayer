#include <jni.h>
#include <string>
#include <map>
#include "AAudioPlayer.h"

static int32_t get_obj_hashcode(JNIEnv *, jobject);
static AAudioPlayer *find_player_by_obj(JNIEnv *, jobject);

static std::map<int32_t, AAudioPlayer*> player_map;

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_start(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> start();
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_pause(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> pause();
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_stop(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> stop();
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_setDatasource(JNIEnv *env, jobject thiz, jstring jpath) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player == nullptr) return JNI_ERR;
    const char *path = env->GetStringUTFChars(jpath, JNI_FALSE);
    player -> set_data_source(path);
    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_prepare(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player == nullptr) return JNI_ERR;
    player -> prepare();
    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_newPlayer(JNIEnv *env, jobject thiz, jint device_id,
                                                     jint sample_rate, jint channel_count) {
    int32_t hashcode = get_obj_hashcode(env, thiz);
    if (hashcode == JNI_ERR) return hashcode;
    AAudioPlayer *player = new AAudioPlayer(device_id, sample_rate, channel_count);
    player_map.emplace(hashcode, player);
    return JNI_OK;
}

static AAudioPlayer* find_player_by_obj(JNIEnv *env, jobject obj) {
    int32_t hashcode = get_obj_hashcode(env, obj);
    if (hashcode == JNI_ERR) return nullptr;
    unsigned int has = player_map.count(hashcode);
    if (has) {
        return player_map.at(hashcode);
    }
    else {
        return nullptr;
    }
}

static int32_t get_obj_hashcode(JNIEnv *env, jobject obj) {
    jclass clazz = env->FindClass("java/lang/Object");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    jmethodID methodId = env->GetMethodID(clazz, "hashCode", "()I");
    if (methodId == nullptr) {
        return JNI_ERR;
    }
    return env->CallIntMethod(obj, methodId);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_release(JNIEnv *env, jobject thiz) {
    int32_t hashcode = get_obj_hashcode(env, thiz);
    if (hashcode == JNI_ERR) return;
    unsigned int has = player_map.count(hashcode);
    if (has) {
        AAudioPlayer *player = player_map.at(hashcode);
        player -> release();
        player_map.erase(hashcode);
    }
}extern "C"
JNIEXPORT jboolean JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_isPrepared(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player == nullptr) return false;
    return player -> is_prepared();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_isPlaying(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player == nullptr) return false;
    return player -> is_playing();
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_seekTo(JNIEnv *env, jobject thiz, jfloat rp) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> seek_to(rp);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_reset(JNIEnv *env, jobject thiz) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> reset();
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_touchair_aaudioplayer_AAudioPlayer_setLoop(JNIEnv *env, jobject thiz, jboolean is_loop) {
    AAudioPlayer *player = find_player_by_obj(env, thiz);
    if (player != nullptr) player -> set_loop(is_loop);
}