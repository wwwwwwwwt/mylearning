#include <atomic>
#include <iostream>

class shared_count{
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
class shared_ptr{
    template<typename U>
    friend class shared_ptr;//为了使子类指针可以转换成基类指针，需要声明为友元类来访问私有成员
public:
    explicit shared_ptr(T *ptr = nullptr):ptr_ (ptr){//默认构造函数
        if(ptr_){
            shared_count_ = new shared_count();
        }
    }

    ~shared_ptr(){
        if(ptr_&& ! shared_count_->reduce_count()){
            delete ptr_;
            delete shared_count_;
        }
    }

    shared_ptr(const shared_ptr& other){//复制构造函数
        ptr_ = other.ptr_;
        if(ptr_){
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    

    shared_ptr(shared_ptr && rhs){//移动构造函数，右值移动过来，然后指针为空
        ptr_ = rhs.ptr_;
        if(ptr_){
            shared_count_ = rhs.shared_count_;
            rhs.ptr_ = nullptr;
        }
    }
    
    template<typename U>
    shared_ptr(const shared_ptr<U>& rhs) noexcept
    {//支持子类动态转换,各个模板没有天然的friend关系，所以为了访问rhs.ptr_我们需要把模板声明成友元类
        ptr_ = rhs.ptr_;
        if(ptr_){
            rhs.shared_count_->add_count();
            shared_count_ = rhs.shared_count_;
        }
    }
    
    template<typename U>
    shared_ptr(const shared_ptr<U>&other, T* ptr)noexcept
    {
        ptr_ = ptr;
        if(ptr_){
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }

    template<typename U>
    shared_ptr(shared_ptr<U>&& rhs) noexcept
    {
        ptr_ = rhs.ptr_;
        if(ptr_){
            shared_count_ = rhs.shared_count_;
            rhs.ptr_ = nullptr;
        }
    }   


    shared_ptr& operator=(shared_ptr other) noexcept
    {//赋值构造函数，按值传递生成临时变量，在参数构造，调用swap，完成传递对象计数+1，函数完成后销毁，调用析构函数，原来对象计数-1
        other.swap(*this);
        return *this;
    }


    void swap(shared_ptr &rhs) noexcept
    {
        using std::swap;
        swap(rhs.ptr_, ptr_);
        swap(rhs.shared_count_, shared_count_);
    }

    T* get()const noexcept
    {
        return ptr_;
    }

    T* operator->() const noexcept
    {
        return ptr_;
    }

    T& operator*()const noexcept
    {
        return *ptr_;
    }

    operator bool()const noexcept
    {
        return ptr_;
    }

private:
    T* ptr_;
    shared_count* shared_count_;
};

template<typename T, typename U>
shared_ptr<T> reinterpret_cast_ptr(const shared_ptr<int>&other)noexcept
{
   T *ptr = reinterpret_cast<T*>(other.get());
    return shared_ptr<bool>(other, ptr);
}

int main(){
    shared_ptr<int>a(new int(10));
    bool st = a;
    std::cout<<st<<" "<<a.get()<<std::endl;
    return 0;
}