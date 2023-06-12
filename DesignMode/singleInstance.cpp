#include <iostream>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <memory>
using namespace std;
// 饿汉模式，线程安全，共用一个静态对象。
/*
class SingleInstance{
public:
    static SingleInstance* GetInstance(){
        if(ins == nullptr){
            ins = new SingleInstance(5);
        }
        return ins;
    }
    ~SingleInstance(){}
    int Getcnt()const{
        return count;
    }
    void Addcnt(){
        count++;
    }
private:
    SingleInstance(int cnt){
        count = cnt;
        std::cout<<"饿汉"<<std::endl;
    };
    SingleInstance(SingleInstance&&) = default;
    SingleInstance(const SingleInstance&) = default;
    SingleInstance& operator=(const SingleInstance& other){return *this;}
    static SingleInstance *ins;
    int count;

};

SingleInstance* SingleInstance::ins = SingleInstance::GetInstance();

int main(){
    SingleInstance* ins = SingleInstance::GetInstance();

    SingleInstance* ins2 = SingleInstance::GetInstance();

    std::cout<<ins->Getcnt()<<std::endl;
    std::cout<<ins2->Getcnt()<<std::endl;
    ins->Addcnt();
    std::cout<<ins->Getcnt()<<std::endl;
    std::cout<<ins2->Getcnt()<<std::endl;

    return 0;
}
*/
/*
输出:
饿汉
5
5
6
6
*/

//懒汉模式，线程不安全需要上锁，懒初始化，直到第一次使用时才进行构造静态对象。

/*class SingleInstance{
public:
    static SingleInstance* GetInstance(){ // 实际生产中，最好使用shared_ptr; std::shared_ptr<SingleInstance>GetInst()
        if(ins == nullptr){                 // static std::shared_ptr<SingleInstance> ins;
            pthread_mutex_lock(&mutex);        // 声明 std::shared_ptr<SingleInstance> SingleInstance ::ins = nullptr;
            if(ins == nullptr){
                ins = new SingleInstance(0);
                count++;
            }
            pthread_mutex_unlock(&mutex);
        }
        return ins;
    }

    int Getcnt()const{
        return count;
    }
    void Addcnt(){
        count++;
    }

private:
    SingleInstance(int cnt){
        count = cnt;
        std::cout<<"懒汉"<<std::endl;
    };
    SingleInstance(SingleInstance&&) = delete;
    SingleInstance(const SingleInstance&) = delete;
    SingleInstance& operator=(const SingleInstance& other) = delete;
    static SingleInstance* ins;
    static pthread_mutex_t mutex;
    static int count;
};

SingleInstance* SingleInstance::ins = nullptr;
pthread_mutex_t SingleInstance::mutex;
int SingleInstance::count = 0;
void work(){
   SingleInstance* ins = SingleInstance::GetInstance();
   this_thread::sleep_for(chrono::seconds(1));
    //  ins->Addcnt();
   // this_thread::sleep_for(chrono::seconds(1));
    cout<<ins->Getcnt()<<endl; 

    //此处为了演示没有delete ins 实际生产环境需要delete防止内存泄漏
}
*/



class SingleInstance{
public:
    static shared_ptr<SingleInstance> GetInstance(){
        if(ins == nullptr){
            pthread_mutex_lock(&mtx);
            if(ins == nullptr){
                ins = std::shared_ptr<SingleInstance>(new SingleInstance(0));
                count++;
            }
            pthread_mutex_unlock(&mtx);
        }
        return ins;
    }
    ~SingleInstance(){
        std::cout<<"success delete"<<std::endl;
    }
    void Add_count(){
        count++;
    }
    int Get_count(){
       return count;
    }
private:
    SingleInstance(int cnt){
        count = cnt;
        std::cout<<"懒汉"<<std::endl;
    }
    SingleInstance(SingleInstance&&) = delete;
    SingleInstance(const SingleInstance&) = delete;
    SingleInstance& operator=(const SingleInstance&) = delete;
    SingleInstance& operator=(SingleInstance&&) = delete;
    static int count;
    static std::shared_ptr<SingleInstance>ins;
    static pthread_mutex_t mtx;
};
int SingleInstance::count = 1;
std::shared_ptr<SingleInstance> SingleInstance::ins = nullptr;
pthread_mutex_t SingleInstance::mtx;
void work(){
    auto ins = SingleInstance::GetInstance();
    
    this_thread::sleep_for(chrono::seconds(1));
    
    ins->Add_count();

    this_thread::sleep_for(chrono::seconds(1));

    std::cout<<ins->Get_count()<<std::endl;


}

int main(){

    std::thread t1(work);
    std::thread t2(work);


    t1.join();
    t2.join();
 
    return 0;
}