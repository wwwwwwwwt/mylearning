/*
 * @Author: zzzzztw
 * @Date: 2023-08-23 12:19:42
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-23 13:46:50
 * @FilePath: /myLearning/testshared_ptr.cpp
 */
#include <atomic>
#include <iostream>
#include <mutex>
class shared_count
{
public:
    shared_count():count_(1){}
    void add_count(){
        ++count_;
    }
    long reduce_count(){
        return --count_;
    }
    long get_count(){
        return count_;
    }

private:
    std::atomic_long count_;
};

template<typename T>
class shared_ptr
{
    template<typename U>
    friend class shared_ptr; 
public:
    explicit shared_ptr(T* ptr = nullptr):ptr_(ptr){
        if(ptr){
            count_ = new shared_count();
        }
    }
    ~shared_ptr(){
        if(ptr_ && !count_->reduce_count()){
            delete ptr_;
            delete count_;
        }
    }
    shared_ptr(const shared_ptr& other){
        ptr_ = other.ptr_;
        if(ptr_){
            other.count_->add_count();
            count_ = other.count_;
        }
    }
    shared_ptr(shared_ptr&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }
    
    template<typename U>
    shared_ptr(const shared_ptr<U>& other){
        ptr_ = other.ptr_;
        if(ptr_){
            other.count_->add_count();
            count_ = other.count_;
        }
    }
    template<typename U>
    shared_ptr(shared_ptr<U>&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }

    shared_ptr& operator=(shared_ptr other){
        other.swap(*this);
        return *this;
    }

    void swap(shared_ptr& other){
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(count_, other.count_);
    }

    shared_ptr& operator=(shared_ptr&& other){
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            ptr_ = nullptr;
        }
    }
    
    T* get()const noexcept{
        return ptr_;
    }

    T* operator->()const noexcept{
        return ptr_;
    }

    T& operator*(){
        return *ptr_;
    }

    operator bool()const noexcept{
        return ptr_;
    }
    
private:
    T* ptr_;
    shared_count* count_;
};





class Base{
public:
    Base() = default;
    virtual void print(){
        std::cout<<"base"<<std::endl;
    }
    virtual ~Base() = default;
};

class Dervive: public Base
{
public:
    Dervive():Base(){}
    void print()override{
        std::cout<<"dervive"<<std::endl;
    }
    ~Dervive() = default;
};

int main(){
    shared_ptr<Base>a(new Base());
    a->print();
    
    shared_ptr<Dervive>b(new Dervive());
    b->print();
    shared_ptr<Base>c(b);
    c->print();


    return 0;
}