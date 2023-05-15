/*
 * @Author: zzzzztw
 * @Date: 2023-05-15 09:39:38
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-15 10:18:30
 * @FilePath: /myLearning/Srting/String.h
 */
#include <cstring>
#include <utility>
#include <iostream>

class String{
public:

    String():m_data(nullptr), length(0){};
    String(const char* str){
        length = std::strlen(str);
        m_data = new char[length + 1];
        strcpy(m_data, str);
    }

    String(const String& str){
        length = str.length;
        m_data = new char[length + 1];
        strcpy(m_data, str.m_data);
    }

    String(String && str)noexcept{
        m_data = str.m_data;
        length = str.length;
        str.m_data = nullptr;
        str.length = 0;
    }

    ~String(){
        delete[] m_data;
    }

    size_t size()const{
        return length;
    }

    const char* c_str()const{
        return m_data;
    }

    String& operator=(String str){
        str.swap(*this);
        return *this;
    }

    String& operator=(String && str){
        m_data = str.m_data;
        length = str.length;
        str.m_data = nullptr;
        str.length = 0;
        return *this;
    }

    void swap(String& str){
        using std::swap;
        swap(m_data, str.m_data);
        swap(length, str.length);
    }

    char operator[](size_t idx)const{
        return m_data[idx];
    }

    char& operator[](size_t idx){
        return m_data[idx];
    }

    String operator+(const String& str)const{
        String res;
        res.length = length + str.length;
        res.m_data = new char[res.length + 1];
        strcpy(res.m_data, m_data);
        strcat(res.m_data, str.m_data);
        return res;
    }
    friend std::ostream& operator<<(std::ostream& os, const String& str){
        os << str.m_data;
        return os;
    }

private:
    char* m_data;
    size_t length;
};