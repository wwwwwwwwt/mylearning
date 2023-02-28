<!--
 * @Author: zzzzztw
 * @Date: 2023-02-28 09:52:48
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-28 11:08:35
 * @FilePath: /Effectivecpp/clause13.md
-->
# clause13. 使用RAII来管理资源
## RAII：在构造函数中获得资源和在析构函数中释放资源
1. 在c++11之后```auto_ptr```已经过时，现在对标```unique_ptr```,过时的原因如下个例子所示：
```cpp
int main(){
    std::auto_ptr<int>a1(new int);
    std::auto_ptr<int>a2 = a1
    std::auto_ptr<int>a3(new int);
    assert(a1.get() == nullptr);
    /*拷贝赋值后所有权发生移动时，管理的资源对应指针变为nullptr，c++11之后支持  
    移动语义，unique_ptr并不支持拷贝，但支持移动*/
    a3 = a2;
    assert(a2.get() == nullptr);
    return 0;
}
```

<!--
 * @Author: zzzzztw
 * @Date: 2023-02-28 10:08:08
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-02-28 10:57:12
 * @FilePath: /Effectivecpp/clause14.md
-->
# clause14. RAII的拷贝行为
1. 首先举一个资源管理的例子```lock_guard```:
```cpp
template<typename _Mutex>
class lock_guard{
public:
    typename _Mutex mutex_type;
    explicit lock_guard(mutex_type& __m):_M_device(__m)
    {
        _M_device.lock();
    }
    lock_guard(mutex_type& __m, adopt_lock_t)noexcept:_M_device(__m){}//calling thread owns mutex
    ~lock_guard(){_M_device.unlock();}

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
    /* 删除了拷贝构造函数和拷贝复制函数，同时由于自定义析构函数，不会自动生成移动构造函数和移动赋值函数，
    也就是禁止了拷贝和移动*/
private:
    mutex_type & _M_devcice;
}
```
2. RAII可以对于拷贝或移动会产生多种情况：
```cpp
1. 禁止拷贝 (std::unique_ptr)
2. 管理资源的引用计数(std::shared_ptr)
3. 深拷贝底层资源(std::string)
4. 移动管理资源所有权 (std::unique_ptr)
```
3. 使用unique_ptr的三种方法：
```cpp
1. unique_ptr<T>a = make_unique<T>(...)使用make_unique进行包装(最推荐)
2. unique_ptr<T>a(new T());
3. unique_ptr<T>a(现有的裸指针) 最不推荐，可能会报错，不满足独享语义
```

# clause15. 资源管理类中应该提供对原始资源的访问接口

1. 先看一个例子
```cpp
FontHandle getFont();
void releaseFont(FontHandle fh);
class Font{
public:
    explicit  Font(FontHandle fh):f(fh){}
    ~Font(){releaseFont();}
    FontHandle get()const{
        return f;
    }
    operator Fondhandle()const{
        return f;
    }
private:
    FondHandle f;
}
```
使用时  
```cpp
//...
Fond f1(getFont());
//...
FondHandle f2 = f1; // 根据重载发生隐式转换为FondHandle(),当f1被销毁时，会发生未定义行为
//...
```
2. 改进之后我们加上```explicit```关键字，进行转换时必须使用```static_cast<FondHandle>```引起注意，减少错误