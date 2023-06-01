/*
 * @Author: zzzzztw
 * @Date: 2023-05-31 14:55:05
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-01 22:30:46
 * @FilePath: /myLearning/boostasio/asioApi/session.cpp
 */
#include "session.h"
using namespace std;
using namespace boost;
Session::Session(std::shared_ptr<boost::asio::ip::tcp::socket>socket):_sock(socket), _send_pending(false), _recv_pending(false){};

void Session::Connect(const boost::asio::ip::tcp::endpoint& ep){
    _sock->connect(ep);
}


// 这两种方法有问题第一个tcp层不是空的情况下，一个消息发了一半，在调用回调函数继续发送时，另一个消息先进行发送，导致出现“hello |hello world| world”的情况
void Session::WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transferred, 
        std::shared_ptr<MsgNode> msg_node)
{
//当前发送的长度 + 已经发送的长度 小于总长度就说明需要继续发送
   if(bytes_transferred + msg_node->_cur_len < msg_node->_total_len){
        _send_node->_cur_len += bytes_transferred;
        _sock->async_write_some(asio::buffer(_send_node->_msg + _send_node->_cur_len, _send_node->_total_len - _send_node->_cur_len), 
            std::bind(&Session::WriteCallBackErr, this, std::placeholders::_1, std::placeholders::_2, _send_node));
   }
}
void Session::WriteToSocketErr(const string buf){
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.size());
    _sock->async_write_some(boost::asio::buffer(_send_node->_msg, _send_node->_total_len),
        std::bind(&Session::WriteCallBackErr,this,std::placeholders::_1, std::placeholders::_2, _send_node));
}


//正确姿势

void Session::WriteToSocket(const std::string buf){
    
    // 先把消息放进消息队列
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.size()));

    //_send_pending = true代表还有消息没法送完，直接返回接着发
    if(_send_pending){
        return;
    }

    //走到这说明是第一次发这个消息，异步发送消息，并绑定回调函数，将_send_pending置为true表示没发完
    _sock->async_write_some(asio::buffer(buf),std::bind(&Session::WriteCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}


void Session::WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){
    if(ec.value() != 0){
        std::cout<<"err !, error code is "<<ec.value() <<" message is "<<ec.message()<<endl;
        return;
    }


    // 取出队首元素
    auto & send_data = _send_queue.front();

    send_data->_cur_len += bytes_transferred;
    if(send_data->_cur_len < send_data->_total_len){
        //说明还有消息没发完，将消息偏移一个已经发完的长度， 总长度也需要减去已经发完的长度就是剩余消息的长度。
        _sock->async_write_some(asio::buffer(send_data->_msg+ send_data->_cur_len, send_data->_total_len - send_data->_cur_len), 
            std::bind(&Session::WriteCallBack, this, std::placeholders::_1, std::placeholders::_2));
            return;
    }

    _send_queue.pop();
    if(_send_queue.empty()){
        //说明消息都发完了,将标志位制空
        _send_pending = false;
    }

    if(!_send_queue.empty()){
        //继续取出对手元素，接着发出去。第一次发不需要偏移量。
        auto &send_data = _send_queue.front();
        _sock->async_write_some(asio::buffer(send_data->_msg, send_data->_total_len), std::bind(&Session::WriteCallBack, this, std::placeholders::_1,
            std::placeholders::_2));
    }
}


//常用
void Session::WriteAllToSocket(const std::string buf){
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.size()));

    if(_send_pending){
        return;
    }
    //async_send会重复调用async_write_some函数，直到把所有长度发完
    _sock->async_send(asio::buffer(buf), std::bind(&Session::WriteAllCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}

void Session::WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){

    if(ec.value() != 0){
        std::cout<<"err !, error code is "<<ec.value() <<" message is "<<ec.message()<<endl;
        return;
    }

    _send_queue.pop();

    if(_send_queue.empty()){
        _send_pending = false;
    }

    if(!_send_queue.empty()){
        auto &send_data = _send_queue.front();
        _sock->async_write_some(asio::buffer(send_data->_msg, send_data->_total_len), std::bind(&Session::WriteAllCallBack, this, std::placeholders::_1, 
            std::placeholders::_2));
    }

}



// 利用async_read_some异步读
void Session::ReadFromSocket(){
    if(_recv_pending){//正在接收，直接返回
        return;
    }

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);

    _sock->async_read_some(asio::buffer(_recv_node->_msg, _recv_node->_total_len), std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
void Session::ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){

    _recv_node ->_cur_len += bytes_transferred;
    if(_recv_node->_cur_len < _recv_node->_total_len){
        _sock->async_read_some(asio::buffer(_recv_node->_msg + _recv_node->_cur_len, _recv_node->_total_len - _recv_node->_cur_len),
            std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
            return;
    }
    _recv_pending = true;
}

// 利用async_receive一次读出
void Session::ReadAllFromSocket(){
    if(_recv_pending){
        return;
    }

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _sock->async_receive(asio::buffer(_recv_node->_msg, _recv_node->_total_len), std::bind(&Session::ReadaLLCallBack, this, std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}

void Session::ReadaLLCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred){
    _recv_node->_cur_len += bytes_transferred;
    _recv_node = nullptr;
    _recv_pending = false;
}