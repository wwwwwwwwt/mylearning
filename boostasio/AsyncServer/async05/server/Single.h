/*
 * @Author: zzzzztw
 * @Date: 2023-06-12 13:05:55
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-12 15:45:29
 * @FilePath: /myLearning/boostasio/AsyncServer/async05/server/Single.h
 */
#pragma once

#include <iostream>
#include <memory>
#include <mutex>

template<typename T>
class SingleInstance{
public:
    static std::shared_ptr<T> GetInstance(){

        static std::once_flag flag;
        std::call_once(flag, [&](){
            _ins = std::shared_ptr<T>(new T);
        });
        return _ins;
    }

    void PrintAddress(){
        std::cout<<_ins->get()<<std::endl;
    }
    ~SingleInstance(){
        std::cout<<"singleins is destructed"<<std::endl;
    }
private:
    
protected:
    SingleInstance() = default;
    SingleInstance(const SingleInstance<T>&) = delete;
    SingleInstance(SingleInstance<T>&&) = delete;
    SingleInstance& operator=(const SingleInstance<T>&) = delete;
    SingleInstance& operator=(SingleInstance<T>&&) = delete;
    static std::shared_ptr<T>_ins;


};

template<typename T>
std::shared_ptr<T> SingleInstance<T>::_ins = nullptr;