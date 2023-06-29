package cn.touchair.aaudioplayer;

public class AAudioPlayer {

    static {
        System.loadLibrary("aaudiosystem");
    }

    public AAudioPlayer(int deviceId, int sampleRate, int channelCount) {
        if (newPlayer(deviceId, sampleRate, channelCount) == -1) throw new RuntimeException("Failed to create native player!");
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
    public native void reset();
}
