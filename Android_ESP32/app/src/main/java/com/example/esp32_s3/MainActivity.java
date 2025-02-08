package com.example.esp32_s3;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import java.io.IOException;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import java.net.Socket;
import java.io.OutputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import android.os.CountDownTimer;

//import android.annotation.SuppressLint;
//import android.os.Bundle;
//import android.os.Handler;
//import android.os.Message;
//import android.text.TextUtils;
//import android.util.Log;
import android.view.MotionEvent;
//import android.view.View;
//import android.widget.Button;
//import android.widget.EditText;
//import androidx.appcompat.app.AppCompatActivity;
//import java.io.IOException;
//import java.io.OutputStream;
//import java.net.Socket;


public class MainActivity extends AppCompatActivity {
    private EditText editTextUrl;
    private WebView webView;
    private static String Web_inside = "192.168.141.84";;
    // 定义默认网址
    private static final String DEFAULT_URL = "http://192.168.141.84/";
    private Socket socket;
    int V_data=5;//基准速度
    int V_mode=0;//是否改变速度标志位 0-》不改变基准速度模式   1-》改变基准速度模式
    // 用于在子线程和主线程之间通信，更新UI
    private Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                    Toast.makeText(MainActivity.this, "连接成功", Toast.LENGTH_SHORT).show();
                    break;
                case 1:
                    Toast.makeText(MainActivity.this, "发送消息成功", Toast.LENGTH_SHORT).show();
                    break;
                case 2:
                    Toast.makeText(MainActivity.this, "发送消息失败，请检查网络连接", Toast.LENGTH_SHORT).show();
                    break;
                default:
                    break;
            }
            return false;
        }
    });
    // 添加新的成员变量来存储接收到的URL
    private String receivedUrl = "";
    private BufferedReader socketReader = null;

    @SuppressLint({"SetJavaScriptEnabled", "ClickableViewAccessibility"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 修改初始连接
        connectToServer(Web_inside);

        // 初始化界面组件
        editTextUrl = findViewById(R.id.editTextUrl);
        Button buttonLoad = findViewById(R.id.buttonLoad);
        webView = findViewById(R.id.webView);
        Button buttonConnect = findViewById(R.id.connect);

        // 设置WebView的相关属性
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setDomStorageEnabled(true);

        // 设置WebViewClient，用于拦截网页加载过程中的各种请求
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return false;
            }
        });

        // 为加载按钮设置点击事件监听器
        buttonLoad.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String inputUrl = editTextUrl.getText().toString().trim();
                if (TextUtils.isEmpty(inputUrl)) {
                    inputUrl = DEFAULT_URL;

                } else {
                    if (!inputUrl.startsWith("http://") &&!inputUrl.startsWith("https://")) {
                        inputUrl = "https://" + inputUrl + "/";
                        Web_inside=inputUrl;
                    }
                }
                webView.loadUrl(inputUrl);
            }
        });
        buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 如果收到了新的URL，使用新URL中的IP地址进行连接
                if (!TextUtils.isEmpty(receivedUrl)) {
                    try {
                        URL url = new URL(receivedUrl);
                        String host = url.getHost();
                        Web_inside = host; // 更新连接IP
                        connectToServer(host);
                    } catch (MalformedURLException e) {
                        e.printStackTrace();
                        // 如果URL解析失败，使用默认IP
                        connectToServer(Web_inside);
                    }
                } else {
                    // 如果没有收到新URL，使用默认IP
                    connectToServer(Web_inside);
                }
            }
        });

        Button buttonLeft = findViewById(R.id.btn_left);
        Button buttonRight = findViewById(R.id.btn_right);
        Button buttonDown = findViewById(R.id.btn_down);
        Button buttonUp = findViewById(R.id.btn_up);
        Button button_v_Updata = findViewById(R.id.btn_V_Updat);
        TextView textview_Vdata = findViewById(R.id.textView_v_data);
        Button button_yj_rest_esp = findViewById(R.id.btn_rest_yj_esp);
        Button button_yj_rest_stm = findViewById(R.id.btn_rest_yj_stm);
        // 为向左按钮添加触摸事件监听器，按下发送 "1"，松开发送 "0"
        buttonLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int customColorA = getResources().getColor(R.color.A, getTheme());
                int customColorB = getResources().getColor(R.color.B, getTheme());
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                buttonLeft.setBackgroundColor(customColorB);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("1".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                buttonLeft.setBackgroundColor(customColorA);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });
        // 为向右按钮添加触摸事件监听器，按下发送 "2"，松开发送 "0"
        buttonRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int customColorA = getResources().getColor(R.color.A, getTheme());
                int customColorB = getResources().getColor(R.color.B, getTheme());
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                buttonRight.setBackgroundColor(customColorB);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("2".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                buttonRight.setBackgroundColor(customColorA);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });

        // 为向下按钮添加触摸事件监听器，按下发送 "3"，松开发送 "0"
        buttonDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int customColorA = getResources().getColor(R.color.A, getTheme());
                int customColorB = getResources().getColor(R.color.B, getTheme());
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                buttonDown.setBackgroundColor(customColorB);
                                if(V_mode==1)
                                {
                                    V_data--;
                                    textview_Vdata.setText(String.valueOf(V_data));
                                }
                                new Thread(() -> {
                                    try {
                                        outputStream.write("3".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                buttonDown.setBackgroundColor(customColorA);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });

        // 为向上按钮添加触摸事件监听器，按下发送 "4"，松开发送 "0"
        buttonUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int customColorA = getResources().getColor(R.color.A, getTheme());
                int customColorB = getResources().getColor(R.color.B, getTheme());
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                buttonUp.setBackgroundColor(customColorB);
                                if(V_mode==1)
                                {
                                    V_data++;
                                    textview_Vdata.setText(String.valueOf(V_data));
                                }
                                new Thread(() -> {
                                    try {
                                        outputStream.write("4".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                buttonUp.setBackgroundColor(customColorA);
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });

        // 为向改变速度按钮添加触摸事件监听器，按下发送 "5"，松开发送 "0"
        button_v_Updata.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        textview_Vdata.setText(String.valueOf(V_data));
                        int customColorA = getResources().getColor(R.color.A, getTheme());
                        int customColorB = getResources().getColor(R.color.B, getTheme());
                        if(V_mode==0)//普通模式时
                        {
                            button_v_Updata.setText("更改速度");
                            button_v_Updata.setBackgroundColor(customColorA);
                        } else if (V_mode==1) {
                            button_v_Updata.setText("运动模式");
                            button_v_Updata.setBackgroundColor(customColorB);

                        }
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                if(V_mode==0)
                                {
                                    V_mode=1;
                                    new Thread(() -> {
                                        try {
                                            outputStream.write("5".getBytes());
                                            outputStream.flush();
                                            handler.sendEmptyMessage(1);

                                        } catch (IOException e) {
                                            e.printStackTrace();
                                            handler.sendEmptyMessage(2);
                                        }
                                    }).start();

                                } else if (V_mode==1) {
                                    V_mode=0;
                                    new Thread(() -> {
                                        try {
                                            outputStream.write("6".getBytes());
                                            outputStream.flush();
                                            handler.sendEmptyMessage(1);

                                        } catch (IOException e) {
                                            e.printStackTrace();
                                            handler.sendEmptyMessage(2);
                                        }
                                    }).start();

                                }
                                break;
                            case MotionEvent.ACTION_UP:
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });
//esp硬件复位按键，按下发送7 松手发送0
        button_yj_rest_esp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                new Thread(() -> {
                                    try {
                                        outputStream.write("7".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });
//stm硬件复位按键，按下发送8 松手发送0
        button_yj_rest_stm.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (socket!= null && socket.isConnected()) {
                    try {
                        final OutputStream outputStream = socket.getOutputStream();
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                new Thread(() -> {
                                    try {
                                        outputStream.write("8".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                            case MotionEvent.ACTION_UP:
                                new Thread(() -> {
                                    try {
                                        outputStream.write("0".getBytes());
                                        outputStream.flush();
                                        handler.sendEmptyMessage(1);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        handler.sendEmptyMessage(2);
                                    }
                                }).start();
                                break;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });

    }

    @Override
    protected void onPause() {
        super.onPause();
        webView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        webView.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        webView.destroy();
        try {
            if (socketReader != null) {
                socketReader.close();
            }
            if (socket != null) {
                socket.close();
            }
        } catch (IOException e) {
            Log.e("SocketDisconnect", "关闭连接出现异常", e);
        }
    }

    // 修改socket连接方法，添加数据接收功能
    private void connectToServer(String serverIp) {
        new Thread(() -> {
            try {
                socket = new Socket(serverIp, 6666);
                if (socket != null && socket.isConnected()) {
                    handler.sendEmptyMessage(0); // 发送连接成功消息

                    // 创建读取器来接收数据
                    socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    String line;
                    // 读取所有行直到找到URL
                    while ((line = socketReader.readLine()) != null) {
                        if (line.startsWith("http://")) {
                            receivedUrl = line.trim();
                            // 在UI线程更新WebView
                            runOnUiThread(() -> {
                                webView.loadUrl(receivedUrl);
                            });
                            break;
                        }
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
                handler.sendEmptyMessage(2); // 发送错误消息
            }
        }).start();
    }
}
