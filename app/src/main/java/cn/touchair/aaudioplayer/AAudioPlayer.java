package cn.touchair.aaudioplayer;

import android.media.AudioFormat;

import androidx.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class AAudioPlayer {

    static {
        System.loadLibrary("aaudiosystem");
    }

    private static final int DEVICE_DEFAULT = 0;

    public static final int SAMPLE_RATE_44100K = 44100;
    public static final int SAMPLE_RATE_48000K = 48000;

    public static final int CHANNEL_OUT_MONO = 1;
    public static final int CHANNEL_OUT_STEREO = 2;

    public AAudioPlayer(int deviceId, @SampleRate int sampleRate, @ChannelOut int channelCount) {
        if (newPlayer(deviceId, sampleRate, channelCount) == -1) throw new RuntimeException("Failed to create native player!");
    }

    public AAudioPlayer(@SampleRate int sampleRate, @ChannelOut int channelOut) {
        this(DEVICE_DEFAULT, sampleRate, channelOut);
    }

    private native int newPlayer(int deviceId, int sampleRate, int channelCount);
    public native void start();
    public native void pause();
    public native void stop();
    public native int setDatasource(String path);
    public native int prepare();
    public native void release();
    public native void seekTo(float rp);
    public native boolean isPrepared();
    public native boolean isPlaying();
    public native void setLoop(boolean isLoop);
    public native void reset();

    @IntDef({
            SAMPLE_RATE_44100K,
            SAMPLE_RATE_48000K
    })
    @Retention(RetentionPolicy.SOURCE)
    public @interface SampleRate{}

    @IntDef({
            CHANNEL_OUT_MONO,
            CHANNEL_OUT_STEREO
    })
    @Retention(RetentionPolicy.SOURCE)
    public @interface ChannelOut{}
}
