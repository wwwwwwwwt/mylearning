/*
 * @Author: zzzzztw
 * @Date: 2023-08-09 16:33:24
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-09 17:28:00
 * @FilePath: /myLearning/testdeign.cpp
 */
#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
using namespace std;

template<typename T>
class SingleInstance
{
public:
    static shared_ptr<T> GetInstance(){
        static once_flag flag;
        call_once(flag, [&](){
            count++;
            ins = shared_ptr<T>(new T);
        });
        return ins;
    }

    static int get(){
        return count;
    }

public:
    SingleInstance() = default;
    ~SingleInstance() = default;
    SingleInstance & operator=(const SingleInstance&) = delete;
    SingleInstance & operator=(SingleInstance&&) = delete;
    SingleInstance(const SingleInstance&) = delete;
    SingleInstance(SingleInstance&&) = delete;
private:
    static atomic_int count;
    static shared_ptr<T>ins;
};
template<typename T>
shared_ptr<T> SingleInstance<T>::ins = nullptr;
template<typename T>
atomic_int SingleInstance<T>::count = 0;

void test(){
    auto ins = SingleInstance<int>::GetInstance();
    cout<<SingleInstance<int>::get()<<endl;
}

int main(){
    thread t1(test);
    thread t2(test);
    t1.join();
    t2.join();
    return 0;
}

