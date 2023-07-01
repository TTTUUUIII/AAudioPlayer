//
// Created by CHRISTNA on 2023/6/29.
//

#ifndef AAUDIOPLAYER_AAUDIOPLAYER_H
#define AAUDIOPLAYER_AAUDIOPLAYER_H
#include <string>
#include <fstream>
#include <aaudio/AAudio.h>
#define TIMEOUT_NANO 8000L
#define ERROR -1;
using namespace std;

class AAudioPlayer {
public:
    void set_data_source(string path);
    int8_t prepare();
    void start();
    void pause();
    void stop();
    void reset();
    void release();
    void seek_to(float rp);
    void set_loop(bool is_loop);
    bool is_prepared();
    bool is_playing();
    bool is_loop();
    AAudioPlayer(uint8_t device_id, uint16_t sample_rate, uint8_t channel_count);
    ~AAudioPlayer();
private:
    bool _is_loop = false;
    bool _is_prepared = false;
    bool _is_playing = false;
    aaudio_stream_state_t state;
    AAudioStream *stream;
    string source_path;
    fstream source_fin;
    size_t source_length;
    static aaudio_data_callback_result_t data_callback(AAudioStream *stream,
                                                void *userData,
                                                void *audioData,
                                                int32_t numFrames);
    static void error_callback(AAudioStream *stream,
                        void *userData,
                        aaudio_result_t error);
    aaudio_stream_state_t wait_for();
};


#endif //AAUDIOPLAYER_AAUDIOPLAYER_H
