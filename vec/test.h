/*
 * @Author: zzzzztw
 * @Date: 2023-05-25 10:32:51
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-25 14:00:31
 * @FilePath: /myLearning/test.h
 */
#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <functional>
#include <cassert>
#include <iostream>
using namespace std;
template<typename T>
class Vector{
public:
    Vector(){
        Realloc(2);
    }

    ~Vector(){
        clear();
        ::operator delete(m_data, m_cap * sizeof(T));
    }

    Vector(const Vector& other){
        m_size = other.m_size;
        m_cap = other.m_cap;
        m_data = (T*)::operator new(m_cap * sizeof(T));
        for(std::size_t i = 0; i< m_size; i++){
            new(&m_data[i])T(other.m_data[i]);
        }
    }

    void Push_back(const T& val){
        if(m_size >= m_cap){
            Realloc(2 * m_cap);
        }
        m_data[m_size++] = val;
    }

    void Push_back(T&& val){
        if(m_size >= m_cap){
            Realloc(m_cap * 2);
        }
        m_data[m_size++] = std::move(val);
    }
    template<typename ...Args>
    void Emplace_back(Args&& ...args){
        if(m_size >= m_cap){
           
            Realloc(2 * m_cap);
        }
        
        new(&m_data[m_size++])T(std::forward<Args>(args)...);
    }
    
    void POP_back(){
        if(m_size > 0){
            m_size--;
            m_data[m_size].~T();
        }
    }



    Vector& operator=(const Vector& other){
        m_size = other.m_size;
        m_cap = other.m_cap;
        m_data = (T*)::operator new(m_cap * sizeof(T));
        for(std::size_t i = 0; i< m_size; i++){
            new(&m_data[i])T(other.m_data[i]);
        }
        return *this;
    }

    T& operator[](std::size_t idx){
        assert(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    const T& operator[](std::size_t idx)const{
        assert(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    std::size_t size()const{
        return m_size;
    }
    void clear(){
        for(std::size_t i = 0; i< m_size; i++){
            m_data[i].~T();
        }
        m_size = 0;
    }

private:


    void Realloc(std::size_t newCapsize){

        //1. 创建一块新大小的内存块
        //2. 将原来的内存块move到新的内存块里
        //3. 将原来的内存块析构了,先析构每个值，最后把整个内存块析构
        //4. 指针和size和cap指向最新的内存
        
        //1. 
        T* newblock =  (T*)::operator new(newCapsize * sizeof(T));
        std::size_t tempsize = m_size;
        if(newCapsize < m_size){
            m_size = newCapsize;
        }
        //2. 
        for(std::size_t i = 0; i< m_size; i++){
            new(&newblock[i])T(std::move(m_data[i]));
        }
        //3.
        for(std::size_t i = 0; i < tempsize; i++){
            m_data[i].~T();
        }
        ::operator delete(m_data, m_cap * sizeof(T));
        //4. 
        m_data = newblock;
        m_cap = newCapsize;

        
    }

private:
    T* m_data = nullptr;
    std::size_t m_size = 0;
    std::size_t m_cap = 0;
};
 

#endif