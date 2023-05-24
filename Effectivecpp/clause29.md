<!--
 * @Author: zzzzztw
 * @Date: 2023-04-07 09:08:24
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-05-24 12:51:01
 * @FilePath: /myLearning/Effectivecpp/clause29.md
-->
# clause29. 努力写出异常安全的代码

```cpp

class prettyMenu{

public:
    ...
    void changeBackground(std::istream& imgSrc);

    ...

private:
    mutex mtx_;
    Image* bgImage;
    int imageChanges;
};

void prettyMenu::changeBackground(std::istream& imgSrc){
    lock(&mutex);
    delete bgImage;
    ++imageChanges;
    bgImage = new Image(imgSrc);
    unlock(&mutex);
}

```

* 这段代码在实现changeBackground函数时就违背了异常安全原则，因为如果再new的这行爆出异常，而bgImage已经被delete掉了，已经破坏了原对象，但imagechanges计数又被增加，并且最后的锁将永远不会被解锁。

## 正确的做法应该是采用copy and swap方式


* 注意智能指针进行深拷贝时，不要使用原始指针进行初始化， 也不要将两个智能指针指向同一个裸指针

```cpp
int a = new int(0);
shared_ptr<int>p = make_shared<int>(*a);
shared_ptr<int>q = make_shared<int>(*a); // 正确的智能指针深拷贝方式
shared_ptr<int>r = a; // 错误的智能指针深拷贝方式，会使得两个指针同时管理同一个对象

```

* 在上面的代码中，使用 new 运算符创建的int对象 a 和使用 make_shared 函数创建的 shared_ptr 对象 q 都是指向相同的 int 对象的智能指针。而在第三行代码中，将 p 赋值给了 r，这样就会产生两个不同的智能指针管理同一个 int 对象的情况，这种情况会导致内存泄漏。因此，在使用 shared_ptr 进行深拷贝时，应该避免使用原始的指针进行赋值或者初始化。



* copy and swap方式核心就是先用一个替身进行构造，最后将本对象和替身swap，代码大概可能长得如下：

```cpp

struct base{
    int a;
    base(int v):a(v){};
};

struct pImpl{
    shared_ptr<base>bgimage;
    int imageChanges;
    pImpl(int a){
        bgimage = make_shared<base>(a);
        imageChanges = 0;
    }

};

class test{
public:
    test(){
        p = make_shared<pImpl>(5);
    }
    ~test() = default;

    void work(int b){
        using std::swap;

        lock_guard<mutex>locker(mtx_);
        shared_ptr<pImpl>newp = make_shared<pImpl>(*p);//注意！！！深拷贝不能用原始指针
        newp->bgimage.reset(new base(b));
        ++newp->imageChanges;
        return;// 使用return 模拟异常行为， 异常退出不会改变p的本身
        swap(p, newp);
    }

    void print(){
        cout<<p->bgimage->a<<endl;
    }

private:

    mutex mtx_;
    shared_ptr<pImpl>p;

};

int main(){

    shared_ptr<test>c = make_shared<test>();
    c->work(10);
    c->print();
    return 0;
}


```
  