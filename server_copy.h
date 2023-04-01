/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 14:13:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-30 15:00:41
 * @FilePath: /cpptest/server_copy.h
 */
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <unistd.h>
#include <functional>
#include <utility>
#include <memory>
#include <map>
#include "./epoller.h"
#include "skiplist.h"
#include "./my_thread.h"

class Server;
class Client;

const int DBNUM = 16;

using mydb = Skiplist<std::string, std::string>; 
using CommandFun = void (Server::*) (Server*, Client*, string, string&,bool&);



class Client // 保存客户的用户名和对应数据库
{
friend class Server;
public:
    int GETfd()const{
        return fd_;
    }
    void init(int fd,sockaddr_in addr){
        fd_ = fd;
        addr_ = addr;
    }

private:
    int fd_;// 通信的文件描述符
    struct sockaddr_in addr_;
    std::string username_;
    mydb *db;
};

class Server
{
    friend class Client;
    
    Server(int port,bool openLinger);
    ~Server();
   
    bool InitSocket();
    void AddClient_(int fd, sockaddr_in addr);
    void DealListen_();
    void CloseConn_();
    void DealProcess();
    void worker();// 进来一个连接就开启一个线程为这个用户服务
    void initCommand();
    bool addUser(const std::string & username);
    bool loguser(const std::string& username);




    

private:

    int  SetNoBlock(int fd);
    //set命令
    

    void setCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        if(userToDB_.count(client->username_) > 0)
        //flag=client->db.insert_element(key, value);
        flag = userToDB_[client->username_]->insert_element(key,value);
    }

    //get命令
    void getCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        flag = client->db->search_element(key, value);

    }

    //del命令
    void delCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        flag=client->db->delete_element(key);
        
    }

    //load命令
    void loadCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db->load_file_path(client->username_ + ".txt");
    }
    //dump命令
    void dumpCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db->dump_file_path(client->username_ + ".txt");
    }

    void loginCommand(Server* server, Client* client, string key, string& value, bool& flag) { // 登录时从对应用户名的文件中读取数据
        if(userToDB_.count(client->username_) > 0)
        //client->db.load_file_path(client->username + ".txt");
        userToDB_[client->username_]->load_file_path(client->username_ + ".txt");
    }

    void quitCommand(Server* server, Client* client, string key, string& value, bool& flag) { // 登出时，将数据写入对应用户名文件
       // client->db.dump_file_path(client->username + ".txt");
        if(userToDB_.count(client->username_) > 0)
        //client->db.load_file_path(client->username + ".txt");
        userToDB_[client->username_]->dump_file_path(client->username_ + ".txt");
    }

private:

    mydb* DB[DBNUM];
    std::unordered_map<std::string, CommandFun>commands_;
    std::unordered_map<std::string, bool >userToClient_; // 用户名与客户的映射, 在集合中就证明一定有一个对应名字的txt
    std::unordered_map<std::string, mydb*>userToDB_; // 用户名和对应数据库的映射
    std::unique_ptr<Epoller>epoller_;
    int listenFD_;
    int port_;
    bool openLinger_;
    int listenEvent_;
    int connEvent_;
};

