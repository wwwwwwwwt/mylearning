#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;



class test{
public:
    test(){
        flag = A;
        cnt = 0;
    }

    ~test() = default;

    void PrintA(){
        unique_lock<mutex>locker(mtx);

        while(true){
            cv.wait(locker, [&](){
                return flag == A;
            });
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));

            cout<<"A"<<endl;
            cnt++;
            
            state(flag);
            cv.notify_all();
        }
    }

    void PrintB(){
        unique_lock<mutex>locker(mtx);

        while(true){
            cv.wait(locker, [&](){
                return flag == B;
            });

            this_thread::sleep_for(chrono::seconds(1));
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            cout<<"B"<<endl;
            cnt++;
            state(flag);
            cv.notify_all();
        }
    }

    void PrintC(){
        unique_lock<mutex>locker(mtx);

        while(true){
            cv.wait(locker, [&](){
                return flag == C;
            });
            if(cnt >= 6){
                state(flag);
                cv.notify_all();
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));

            cout<<"C"<<endl;
            cnt++;

            state(flag);
            cv.notify_all();
        }
    }

private:
    mutex mtx;
    condition_variable cv;
    enum Flag{A, B,C};
    volatile 
    Flag flag;
    volatile int cnt;
    void state(volatile enum Flag& flag){
        switch(flag){
            case A:
                flag = B;
                break;
            case B:
                flag = C;
                break;
            case C:
                flag = A;
                break;
        }
    }
};


int main(){
    test A;
    thread t1(&test::PrintA,&A);
    thread t2(&test::PrintB,&A);
    thread t3(&test::PrintC,&A);
    t1.join();
    t2.join();
    t3.join();

    cout<<"all done"<<endl;
    return 0;
}