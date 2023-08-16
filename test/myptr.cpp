/*
 * @Author: zzzzztw
 * @Date: 2023-08-05 12:24:00
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-08-05 18:19:19
 * @FilePath: /myLearning/myptr.cpp
 */
#include <atomic>
#include <iostream>


class atomic_count{

public:
    atomic_count():count_(1){}
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
class share_ptr{
    template<typename U>
    friend class share_ptr;
public: 
    explicit share_ptr(T* ptr = nullptr):ptr_(ptr){
        if(ptr_){
            count_ = new atomic_count();
        }
    }
    ~share_ptr(){
        if(ptr_ && !count_->reduce_count()){
            delete ptr_;
            delete count_;
        }
    }
    share_ptr(const share_ptr& other){
        ptr_ = other.ptr_;
        if(ptr_){
            other.count_->add_count();
            count_ = other.count_;
        }
    }
    share_ptr(share_ptr&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }

    share_ptr& operator=(const share_ptr other){
        other.swap(*this);
        return *this;
    }
    share_ptr& operator=(share_ptr&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }
    template<typename U>
    share_ptr(const share_ptr<U>& other){
        ptr_ = other.ptr;
        if(ptr_){
            other.count_->add_count();
            count_ = other.count_;
        }
    }
    template<typename U>
    share_ptr(const share_ptr&& other){
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }
    template<typename U>
    share_ptr& operator=(const share_ptr<U>& other){
        ptr_ = other.ptr_;
        if(ptr_){
            other.count_->add_count();
            count_ = other.count_;
        }
    }
    template<typename U>
    share_ptr& operator=(share_ptr<U>&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            count_ = other.count_;
            other.ptr_ = nullptr;
        }
    }

    void swap(share_ptr& other) noexcept{
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(count_, other.count_);
    }
    T& operator*()const noexcept{
        return *ptr_;
    }

    T* operator->()const noexcept{
        return ptr_;
    }

    T* get()const noexcept{
        return ptr_;
    }
    operator bool()const noexcept{
        return ptr_;
    }

private:
    T* ptr_;
    atomic_count* count_;
};




class A {
public:
    A():a(1){
        
    };
    virtual ~A() = default;
    virtual void print(){
        std::cout<<" A: " <<a<<std::endl;
    }
private:
    int a;
};

class B : public A{
public:
    B():A(), b(10){
        print();
    }
    virtual ~B() = default;
    inline void print()override{
        std::cout<<" B: "<<b<<std::endl;
    }
private:
    int b;
};


int main(){
    share_ptr<A>a(new B());
    {
        share_ptr<A>b(a);
        share_ptr<A>c(a);
        share_ptr<A>d(a);
        a->print();
        b->print();
        c->print();
        d->print();
    }
    bool st = a;
    std:: cout<<"st:"<<st<<std::endl;
    auto b = std::move(a);
    st = a;
    std:: cout<<"st:"<<st<<std::endl;
    std::cout<<a.get()<<std::endl;



    return 0;
}