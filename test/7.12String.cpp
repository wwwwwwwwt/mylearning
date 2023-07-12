/*
 * @Author: zzzzztw
 * @Date: 2023-07-12 10:53:29
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-07-12 14:13:10
 * @FilePath: /myLearning/test2.cpp
 */

#include <iostream>
#include <cstring>
#include <utility>


class String{
public:
    String():m_data(nullptr), m_length(0){}
    ~String(){
        delete []m_data;
    }
    String(const char* ch){
        m_length = strlen(ch);
        m_data = new char[m_length + 1];
        strcpy(m_data, ch); 
    }
    String(const String& str){
        m_length = str.m_length;
        m_data = new char[m_length + 1];
        strcpy(m_data, str.m_data);
    }
    String(String&& str)noexcept{
        m_data = str.m_data;
        m_length = str.m_length;
        str.m_data = nullptr;
        str.m_length = 0;
    }
    String& operator=(String str){
        String(str).Swap(*this);
        return *this;
    }
    String& operator=(String&& str)noexcept{
        m_data = str.m_data;
        m_length = str.m_length;
        str.m_data = nullptr;
        str.m_length = 0;
        return *this;
    }
    char& operator[](int idx)const{
        return m_data[idx];
    }
    char& operator[](int idx){
        return m_data[idx];
    }
    void Swap(String& str){
        using std::swap;
        std::swap(m_data, str.m_data);
        std::swap(m_length, str.m_length);
    }
    friend std::ostream& operator<<(std::ostream& os, const String& str){
        os<<str.m_data;
        return os;
    }
private:
    char* m_data;
    int m_length;
};


int main(){
    String str = "123456";
    std::cout<<str<<std::endl;
    String str2 = str;
    std::cout<<str2<<std::endl;
    str2[3] = '5';
    std::cout<<str2<<std::endl;
    auto str3 = std::move(str2);
    str3[3] = '9';
    std::cout<<str3<<std::endl;
    std::cout<<str2<<std::endl;
    return 0;
}