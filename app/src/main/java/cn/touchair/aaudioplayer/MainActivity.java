package cn.touchair.aaudioplayer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import java.nio.file.Path;
import java.nio.file.Paths;

import cn.touchair.aaudioplayer.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private static final String TAG = MainActivity.class.getSimpleName();

    private AAudioPlayer mPlayer;
    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        binding.pauseBtn.setOnClickListener(this);
        binding.startBtn.setOnClickListener(this);
        binding.restartBtn.setOnClickListener(this);
        binding.resetBtn.setOnClickListener(this);
        binding.setSourceBtn.setOnClickListener(this);
        mPlayer = new AAudioPlayer(AAudioPlayer.SAMPLE_RATE_44100K, AAudioPlayer.CHANNEL_OUT_STEREO);
        mPlayer.setLoop(true);
        mIsLoopMode = true;
        updateUI();
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (mPlayer.isPlaying()) {
            mPlayer.stop();
            binding.stateTextView.setText("STATE_STOPPED");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mPlayer.release();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.pause_btn:
                mPlayer.pause();
                mCurrentStatus = "STATE_PAUSED";
                updateUI();
                break;
            case R.id.start_btn:
                mPlayer.start();
                mCurrentStatus = "STATE_STARED";
                updateUI();
                break;
            case R.id.restart_btn:
                mPlayer.seekTo(0);
                break;
            case R.id.set_source_btn:
                final Path path = Paths.get((binding.editText.getText().toString()));
                if (path.toFile().exists()) {
                    mPlayer.setDatasource(path.toString());
                    mPlayer.prepare();
                    mIsShowControlPanel = true;
                    mIsShowSourcePanel = false;
                    mCurrentStatus = "STATE_PREPARED";
                    updateUI();
                } else {
                    Toast.makeText(getApplicationContext(), "文件不存在！", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.reset_btn:
                mPlayer.reset();
                mCurrentStatus = "STATE_UNINITIALIZED";
                mIsShowControlPanel = false;
                mIsShowSourcePanel = true;
                updateUI();
                break;
        }
    }

    private boolean mIsShowControlPanel = false;
    private boolean mIsShowSourcePanel = true;

    private boolean mIsLoopMode = false;

    private String mCurrentStatus = "STATE_UNINITIALIZED";

    public void updateUI() {
        binding.setSourceBtn.setEnabled(mIsShowSourcePanel);
        binding.startBtn.setEnabled(mIsShowControlPanel);
        binding.pauseBtn.setEnabled(mIsShowControlPanel);
        binding.restartBtn.setEnabled(mIsShowControlPanel);
        binding.resetBtn.setEnabled(mIsShowControlPanel);
        binding.stateTextView.setText(mCurrentStatus);
        binding.isLoopModeTextView.setText(mIsLoopMode ? "YES" : "NO");
    }
}