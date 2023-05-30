/*
 * @Author: zzzzztw
 * @Date: 2023-05-29 14:02:46
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-30 19:40:11
 * @FilePath: /myLearning/boostasio/endpoint.h
 */
extern int client_end_point(); // 客户端连接一个端点
extern int server_end_point(); // 服务器创建一个接口端点供客户端连接
extern int create_tcp_socket(); // 创建socket
extern int create_acceptor_socket(); // 建立accept
extern int bind_acceptor_socket(); // 老版本实现bind操作
extern int connect_to_end(); // 作为客户端可以连接服务器指定的端点进行连接
extern int accept_new_connection(); // 作为服务端去连接一个连接
extern void use_const_buffer();//asio网络库中使用buffer麻烦的方式
extern void use_const_str();// 简单的方式
extern void use_buffer_array();// 字符串数组
extern void use_stream_buffer();//使用streanbuf，将输入输出流和streambuf绑定，可以实现流式输入和输出
extern int send_data_by_write_some();//模拟同步客户端，先创建对端地址，创建上下文，创建socket，使用socket连接到对端