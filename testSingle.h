#pragma once
#include <mutex>
#include <atomic>
#include <memory>
#include <iostream>


template<typename T>
class SingleIns
{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag flag;
        std::call_once(flag, [&](){
            ins_ = std::shared_ptr<T>(new T());
        });
        return ins_;
    }

protected:
    SingleIns() = default;
    SingleIns(const SingleIns&) = delete;
    SingleIns(SingleIns&&) = delete;
    SingleIns& operator=(const SingleIns&) = delete;
    SingleIns& operator=(SingleIns&&) = delete;

private:
    static std::shared_ptr<T>ins_;
};

template<typename T>
std::shared_ptr<T> SingleIns<T>::ins_ = nullptr;