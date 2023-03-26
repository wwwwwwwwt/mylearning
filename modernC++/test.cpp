#include <bits/stdc++.h>

using namespace std;

class Myclass
{
public:
    Myclass(){
        x = new int;
    }
    ~Myclass(){
        delete x;
    };
    void set(int a){
        *x = a;
    }
    void show()const{
        cout<<*x<<endl;
    }
private:
    int *x;

};

int main(){

    void* memory = malloc(2 * sizeof(Myclass));

    Myclass* a = new(memory)Myclass();
    a->set(15);
    a->~Myclass();
 //   Myclass* b = new(memory)Myclass();

 //   b->show();
  //  b->~Myclass();
  //  b->show();
    ::operator delete(memory,sizeof(Myclass));
        a->show();
    a->set(5);
        a->show();

    return 0;
}