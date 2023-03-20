/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 15:10:04
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-17 17:03:53
 * @FilePath: /cpptest/skiplistpro/myRedis_server/skiplist.h
 */
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "node.h"
#include <fstream>
#include <mutex>
#include <string>
#include <mutex>

using std::string;
using std::ifstream;
using std::ofstream;
using std::mutex;
template<typename K, typename V>
class Skiplist
{
public:

    Skiplist();
    Skiplist(int level);
    ~Skiplist();
    Node<K,V>* create_node(K k, V v, int level);
    void display_list();

    int insert_element(K k, V v);
    bool search_element(K k,V &v);
    bool delete_element(K k);

    int size()const;

    void dump_file();
    void load_file();

private:

    void find(V target, vector<Node<K,V>*>& pre); 

    void get_key_val_from_string(const string&str, string* key, string *val);
    void is_valid_string(const string &str);
    
private:

    int max_level;


    int cur_onlevel;

    int element_count_;

    ofstream fileWriter_;
    ifstream fileReader_;

    Node<K,V>* header_; //跳表头节点
    mutex mtx_;
    
};

//找到每一层的小于目标值的最大节点，开一个pre数组用于保存这个节点
template<typename K, typename V>
void Skiplist<K,V>::find(V target,vector<Node<K,V>*>& pre)
{
    auto p = header_;
    for(int i = max_level - 1; i>=0; i--){
        while(p->next[i] && p->next[i]->val < target)p = p->next[i];
        pre[i] = p;
    }
}

template<typename K, typename V>
Node<K,V>* Skiplist<K,V>::create_node(K k, V v, int level)
{
    Node<K,V>* node = new Node<K,V>(k, v, level);
    return node;
}

template<typename K, typename V>
Skiplist<K,V>::Skiplist()
{
    max_level = 32;
    cur_onlevel = 0;
    element_count_ = 0;
    /*创造虚拟头节点，初始化为null null max_level*/
    K k;
    V v;
    header_ = new Node<K,V>(k, v, max_level);
}

template<typename K, typename V>
Skiplist<K,V>::Skiplist(int level)
{
    max_level = level;
    cur_onlevel = 0;
    element_count_ = 0;
    /*创造虚拟头节点，初始化为null null max_level*/
    K k;
    V v;
    header_ = new Node<K,V>(k, v, max_level);
}

template<typename K, typename V>
Skiplist<K,V>::~Skiplist()
{
    delete header_;
    if(fileWriter_.is_open()){
        fileWriter_.close();
    }
    if(fileReader_.is_open()){
        fileReader_.close();
    }

}

template<typename K, typename V>
int Skiplist<K,V>::insert_element(K key, V val)
{

}

template<typename K, typename V>
bool Skiplist<K,V>::search_element(K key, V &val)
{
    /*通过val将查询到的值传出去*/
}


template<typename K, typename V>
bool Skiplist<K,V>::delete_element(K key)
{
    
}

template<typename K, typename V>
int Skiplist<K,V>:: size()const
{

}


template<typename K, typename V>
void Skiplist<K,V>::dump_file()
{

}

template<typename K, typename V>
void Skiplist<K,V>::load_file()
{
    
}

template<typename K, typename V>
void Skiplist<K,V>::get_key_val_from_string(const string& str, string* key, string* val)
{

}

template<typename K, typename V>
void Skiplist<K,V>::is_valid_string(const string&str)
{
    
}

#endif
