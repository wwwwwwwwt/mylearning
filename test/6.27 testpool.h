/*
 * @Author: zzzzztw
 * @Date: 2023-06-27 09:20:39
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-06-27 10:49:11
 * @FilePath: /myLearning/testpool.h
 */
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <future>
#include <memory>
#include <functional>
using namespace std;

template<typename T>
class Safeque{

public:
    Safeque() = default;
    Safeque(const Safeque&) = delete;
    Safeque& operator=(const Safeque&) = delete;
    void enqueue(const T&t){
        lock_guard<mutex>locker(mtx_);
        que_.push(t);
    }
    void enqueue(T&& t){
        lock_guard<mutex>locker(mtx_);
        que_.push(t);
    }
    bool dequeue(T& t){
        lock_guard<mutex>locker(mtx_);

        if(que_.empty())return false;

        t = std::move(que_.front());
        que_.pop();
        return true;
    }

    void clean(){
        lock_guard<mutex>locker(mtx_);
        while(que_.size())que_.pop();
    }
    bool isempty(){
        lock_guard<mutex>locker(mtx_);
        return que_.empty();
    }
    int size(){
        lock_guard<mutex>locker(mtx_);
        return que_.size();
    }
    ~Safeque(){
        clean();
    }

private:
    mutex mtx_;
    queue<T>que_;
};


class ThreadPool{
public:
    explicit ThreadPool(int size = 4):threads_(size), isstop_(false){};
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool(){
        stop();
        cout<<"all done"<<endl;
        for(size_t i = 0; i < threads_.size(); i++){
            if(threads_[i].joinable()){
                threads_[i].join();
            }
        }
    }
    void init(){
        for(size_t i = 0; i < threads_.size(); i++){
            threads_.at(i) = thread(ThreadWorker(this, i));
        }
    }
    void stop(){
        isstop_ = true;
        cv_.notify_all();
    }

    template<typename F, typename ...Args>
    auto Addtask(F&&f, Args&& ...args)->future<decltype(f(args...))>{
        using ret = decltype(f(args...));
        function<ret()>func = bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = make_shared<packaged_task<ret()>>(func);
        auto wrapper_ptr = [task_ptr](){
            (*task_ptr)();
        };
        safeque_.enqueue(wrapper_ptr);
        cv_.notify_one();
        return task_ptr->get_future();
    }

private:
    class ThreadWorker{
    public:
        ThreadWorker(ThreadPool* pool, int id):pool_(pool), id_(id){};
        void operator()(){
            cout<<"thread "<<id_<<" : enter"<<endl;
            function<void()>func;
            bool hastask = false;
            while(!pool_->isstop_){
                {
                    unique_lock<mutex>locker(pool_->mutx_);
                    cout<<"thread "<<id_<<" : waiting task\n";
                    pool_->cv_.wait(locker, [&](){
                        if(pool_->isstop_)return true;
                        return !pool_->safeque_.isempty();
                    });
                    hastask = pool_->safeque_.dequeue(func);
                }
                if(hastask){
                    printf("threadpool [%d]: is working\n", id_);
                    func();
                    printf("threadpool [%d] work done\n", id_);
                }
            }
            cout<<"thread "<<id_<<" : exit"<<endl;
        }
    private:
        ThreadPool* pool_;
        int id_;    
    };
private:
    Safeque<function<void()>>safeque_;
    vector<thread>threads_;
    mutex mutx_;
    condition_variable cv_;
    atomic<bool>isstop_;
};