/*
 * @Author: zzzzztw
 * @Date: 2023-02-24 16:30:39
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-23 20:59:54
 * @FilePath: /cpptest/smartptr/myshareptr.h
 */
#ifndef MYSHAREPTR_H
#define MYSHAREPTR_H

#include <iostream>
#include <cassert>
template<typename T>
class Myshared_ptr{

public:
    Myshared_ptr():ptr_(nullptr),count_(new int(0)){ std::cout << "create" << std::endl;}
    Myshared_ptr(T *ptr):ptr_(ptr){
        if(ptr_){
            count_ = new int (1);
        }
        else{
            count_ = new int (0);
        }

        std::cout << "copy" <<*(count_)<< std::endl;
    }

    Myshared_ptr(const Myshared_ptr& s){
        if(this->ptr_ != s.ptr_){
            ptr_ = s.ptr_;
            count_ = s.count_;
            (*count_)++;
        }
    }

    Myshared_ptr(Myshared_ptr&& s){
        if(this->ptr_ != s.ptr_){
            ptr_ = s.ptr_;
            count_ = s.count_;
            (*count_)++;
        }
    }

    Myshared_ptr& operator=(const Myshared_ptr& s){
        if(this->ptr_ == s.ptr_){
            return *this;
        }
        if(--(*count_) == 0){
            delete this->ptr_;
            delete this->count_;
        }
        ptr_ = s.ptr_;
        count_ = s.count_;
        (*count_)++;
        return *this;
    }

    T* operator->(){
        assert(ptr_);
        return this->ptr_;
    }

    T& operator*(){
        assert(ptr_);
        return *ptr_;

    }

    int user_count()const {
        return *this->count_;
    }

    ~Myshared_ptr(){
        (*count_)--;
        if(*count_ == 0){
            delete ptr_;
            delete count_;
            std::cout << "delete" << std::endl;
        }

    }




private:
    T* ptr_;
    int* count_;

};

#endif