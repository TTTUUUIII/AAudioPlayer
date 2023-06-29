//
// Created by CHRISTNA on 2023/6/29.
//
#include <thread>
#include <fstream>
#include <aaudio/AAudio.h>
#include "AAudioPlayer.h"
#include "Log.h"
#define NO_ERROR 0

void completed_proc(AAudioPlayer *player);

void AAudioPlayer::set_data_source(string path) {
    source_path = path;
}

int8_t AAudioPlayer::prepare() {
    LOGD("AAudioPlayer::prepare");
    if (source_path.empty()) return ERROR;
    source_fin.open(source_path, ios::in | ios::binary);
    if (!AAudioPlayer::source_fin.is_open()) return ERROR;
    source_fin.seekg(0, ios::end);
    source_length = source_fin.tellg();
    source_fin.seekg(0, ios::beg);
    _is_prepared = true;
    return NO_ERROR;
}

void AAudioPlayer::pause() {
    LOGD("AAudioPlayer::pause");
    AAudioStream_requestPause(stream);
    waitFor();
    AAudioStream_requestFlush(stream);
    waitFor();
    _is_playing = false;
}

void AAudioPlayer::start() {
    LOGD("AAudioPlayer::start");
    AAudioStream_requestStart(stream);
    waitFor();
    _is_playing = true;
}

void AAudioPlayer::stop() {
    LOGD("AAudioPlayer::stop");
    AAudioStream_requestStop(stream);
    waitFor();
    _is_playing = false;
}

void AAudioPlayer::reset() {
    LOGD("AAudioPlayer::reset");
    if (_is_playing) stop();
    if (source_fin.is_open()) source_fin.close();
}

void AAudioPlayer::release() {
    LOGD("AAudioPlayer::release");
    free(this);
}

aaudio_stream_state_t AAudioPlayer::waitFor() {
    aaudio_stream_state_t current_state = AAudioStream_getState(stream);
    aaudio_stream_state_t next_state = AAUDIO_STREAM_STATE_UNINITIALIZED;
    AAudioStream_waitForStateChange(stream, current_state, &next_state, TIMEOUT_NANO);
    state = AAudioStream_getState(stream);
    return state;
}

void AAudioPlayer::seek_to(float rp) {
    LOGD("AAudioPlayer::seek_to %f", rp);
    if (source_fin.is_open()) {
        source_fin.seekg(source_length * rp, ios::beg);
    }
}

bool AAudioPlayer::is_prepared() {
    return _is_prepared;
}

bool AAudioPlayer::is_playing() {
    return _is_playing;
}

AAudioPlayer::AAudioPlayer(uint8_t device_id, uint16_t sample_rate, uint8_t channel_count) {
    AAudioStreamBuilder *builder;
    AAudio_createStreamBuilder(&builder);
    AAudioStreamBuilder_setDeviceId(builder, device_id);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setSampleRate(builder, sample_rate);
    AAudioStreamBuilder_setChannelCount(builder, channel_count);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setDataCallback(builder, AAudioPlayer::data_callback, this);
    AAudioStreamBuilder_setErrorCallback(builder, AAudioPlayer::error_callback, this);
    AAudioStreamBuilder_openStream(builder, &stream);
    if (builder) AAudioStreamBuilder_delete(builder);
}

AAudioPlayer::~AAudioPlayer() {
    _is_playing = false;
    if (source_fin.is_open()) source_fin.close();
    AAudioStream_close(stream);
}

aaudio_data_callback_result_t
AAudioPlayer::data_callback(AAudioStream *stream, void *userData, void *audioData,
                            int32_t numFrames) {
    AAudioPlayer *player = (AAudioPlayer *) userData;
    if (player->source_fin.is_open()) {
        player -> source_fin.read(reinterpret_cast<char *>(audioData), numFrames * 2 * 2);
        if (player -> source_fin.eof()) {
            LOGD("Play completed!");
            thread cop_handler(completed_proc, player);
            cop_handler.detach();
        }
    } else return AAUDIO_CALLBACK_RESULT_STOP;
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void completed_proc(AAudioPlayer *player) {
    LOGD("completed_proc called");
    player -> stop();
    player -> seek_to(0);
}

void error_proc(AAudioStream *stream, void *userData) {
    AAudioStream_close(stream);
}
void AAudioPlayer::error_callback(AAudioStream *stream, void *userData, aaudio_result_t error) {
    LOGE("AAudioPlayer::error_callback");
    thread err_handler(error_proc, stream, userData);
    err_handler.detach();
}
