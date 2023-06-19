/*
 * @Author: zzzzztw
 * @Date: 2023-06-18 21:28:47
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-18 21:40:04
 * @FilePath: /cpptest/boostasio/coroutineServer/msgnode.cpp
 */
#include "msgnode.h"

RecvNode::RecvNode(short max_len, short msg_id):MsgNode(max_len), _msg_id(msg_id){

}
SendNode::SendNode(const char* data, short max_len, short msg_id):MsgNode(max_len + HEAD_TOTAL_LEN),_msg_id(msg_id){
    //先转成网络字节序的id
    short msg_id_net = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
    memcpy(_data, &msg_id_net, HEAD_ID_LEN);
    //再发送数据的长度，转为网络字节序
    short msg_len_net = boost::asio::detail::socket_ops::host_to_network_short(max_len);
    memcpy(_data + HEAD_ID_LEN, &max_len, HEAD_DATA_LEN);

    memcpy(_data + HEAD_TOTAL_LEN, data, max_len);

}