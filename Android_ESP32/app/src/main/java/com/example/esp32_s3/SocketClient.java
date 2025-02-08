//package com.example.esp32_s3;
//
//import java.io.BufferedReader;
//import java.io.BufferedWriter;
//import java.io.CharArrayWriter;
//import java.io.IOException;
//import java.io.InputStreamReader;
//import java.io.OutputStreamWriter;
//import java.net.InetSocketAddress;
//import java.net.Socket;
//import java.net.SocketAddress;
//import java.nio.charset.StandardCharsets;
//import java.util.Arrays;
//
//public class SocketClient {
//    Socket socket;
//    private BufferedReader reader;
//    private BufferedWriter writer;
//
//    // 连接方法，传入要连接的IP地址和端口号
//    public void connect(String ipAddress, int port) {
////        try {
////            socket = new Socket(ipAddress, port);
////            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
////            writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
////        } catch (IOException e) {
////            e.printStackTrace();
////
////        }
//        try {
//            socket = new Socket();
//            SocketAddress socketAddress = new InetSocketAddress(ipAddress, port);
//            socket.connect(socketAddress, 5000);  // 设置5秒超时时间，可根据实际情况调整
//            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//            writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
//
//    // 发送消息方法，发送消息并尝试读取返回的响应信息
////    public void sendMessage(String message) {
////        if (socket == null ||!socket.isConnected()) {
////            System.out.println("未连接到服务器，无法发送消息");
////            return;
////        }
////        try {
////            writer.write(message + "\n");
////            writer.flush();
////            reader.readLine();
////        } catch (IOException e) {
////
////            System.out.println("发送消息时出现错误：" + e.getMessage());
////        }
////    }
//    public void sendMessage(String message) {
//
//        try {
//
//            byte[] buffer = message.getBytes(StandardCharsets.UTF_8);
//
//            CharArrayWriter mOutputStream = null;
//            mOutputStream.write(Arrays.toString(buffer));
//
//
//
//            // 清空消息输入框
//
//        } catch (IOException e) {
//
//            e.printStackTrace();
//           // showToastOnUiThread(R.string.toast_failed_to_send_message);
//
//        }
//    }
//
//    // 断开连接方法，关闭相关的输入输出流和Socket连接
//    public void disconnect() {
//        try {
//            if (reader!= null) {
//                reader.close();
//            }
//            if (writer!= null) {
//                writer.close();
//            }
//            if (socket!= null) {
//                socket.close();
//            }
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
//}