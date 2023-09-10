#include <atomic>

class shared_count{
public:
    shared_count():count(1){}
    ~shared_count() = default;
    void add_count(){
        ++count;
    }
    long reduce_count(){
        return --count;
    }
    long get_count(){
        return count;
    }
private:
    std::atomic_long count;
};

template<typename T>
class share_ptr
{
    template<typename U>
    friend class share_ptr;
public:
    explicit share_ptr(T* ptr = nullptr):ptr_(ptr){
        if(ptr_){
            count = new shared_count();
        }
    }
    ~share_ptr(){
        if(ptr_ && !count->reduce_count()){
            delete ptr_;
            delete count;
        }
    }
    share_ptr(const share_ptr& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            ptr_ = other.ptr_;
            other.count->add_count();
            count = other.count;
        }
    }

    share_ptr(share_ptr&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            ptr_ = other.ptr_;
            count = other.count;
            other.ptr_ = nullptr;
        }
    }
    template<typename U>
    share_ptr(const share_ptr<U>& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            ptr_ = other.ptr_;
            other.count->add_count();
            count = other.count;
        }
    }
    template<typename U>
    share_ptr(share_ptr<U>&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            ptr_ = other.ptr_;
            count = other.count;
            other.ptr_ = nullptr;
        }
    }
    share_ptr& operator=(share_ptr other)noexcept{
        other.swap(*this);
        return *this;
    }
    
    void swap(share_ptr& other)noexcept{
        using std::swap;
        swap(other.ptr_, ptr_);
        swap(other.count, count);
    }
    share_ptr& operator=(share_ptr&& other)noexcept{
        ptr_ = other.ptr_;
        if(ptr_){
            ptr_ = other.ptr_;
            count = other.count;
            other.ptr_ = nullptr;
        }        
    }

    T* get(){
        return ptr_;
    }

    T* operator->(){
        return ptr_;
    }

    T& operator*(){
        return *ptr_;
    }

    operator bool()const{
        return ptr_;
    }
private:
    T* ptr_;
    shared_count* count;
};

#include <memory>
#include <mutex>
template<typename T>
class SingleIns
{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag flag;
        std::call_once(flag, [&](){
            ins = std::shared_ptr<T>(new T());
        });
        return ins;
    }
protected:
    SingleIns() = default;
    SingleIns(const SingleIns&) = delete;
    SingleIns(SingleIns&&) = delete;
    SingleIns& operator=(const SingleIns&) = delete;
    SingleIns& operator=(SingleIns&&) = delete;
private:
    static std::shared_ptr<T>ins;
};

template<typename T>
std::shared_ptr<T> SingleIns<T>::ins = nullptr;