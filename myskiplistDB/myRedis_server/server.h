/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 14:13:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-21 20:16:12
 * @FilePath: /cpptest/skiplistpro/myRedis_server/server.h
 */
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include "skiplist.h"
#include "./my_thread.h"
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <unistd.h>
#include "./epoller.h"



class Server;
class Client;

using mydb = Skiplist<std::string, std::string>; 
using CommanFun = void(*)(Server*, Client*, std::string, string&, bool&);


class Client
{
friend class Server;
public:


private:
    mydb db;
};

class Server
{
    friend class Client;
    
    Server();
    ~Server();
    void initCommand(Server*& server);
    void InitSocket();
    void AddClient_(int fd, sockaddr_in addr);
    void DealListen_();
    void CloseConn_();
    void DealProcess();
    
private:

    int  SetNoBlock(int fd);
    //set命令
    void setCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        flag=client->db.insert_element(key, value);
    }

    //get命令
    void getCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        flag = client->db.search_element(key, value);

    }

    //del命令
    void delCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        flag=client->db.delete_element(key);
        
    }

    //load命令
    void loadCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db.load_file();
    }
    //dump命令
    void dumpCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db.dump_file();
    }
        //load命令
    void loadpathCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db.load_file_path(key);
    }
    //dump命令
    void dumppathCommand(Server* server, Client* client, string key, string& value, bool& flag) {
        client->db.dump_file_path(key);
    }

private:
    std::unique_ptr<Epoller>epoller_;
    int listenFD_;
    int port_;
};





#endif