<!--
 * @Author: zzzzztw
 * @Date: 2023-03-06 08:59:15
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-03 14:07:20
 * @FilePath: /cpptest/Effectivecpp/clause21.md
-->
# clause21. 不要错误的返回对象的引用

这个条款的核心观点在于，不要把返回值写成引用类型，作者在条款内部详细分析了各种可能发生的错误，无论是返回一个stack对象还是heap对象，在这里不再赘述。作者最后的结论是，如果必须按值返回，那就让他去吧，多一次拷贝也是没办法的事，最多就是指望着编译器来优化。
但是对于C++11以上的编译器，我们可以采用给类型编写“转移构造函数”以及使用std::move()函数更加优雅地消除由于拷贝造成的时间和空间的浪费。

# clause22. 将成员变量声明为private

好处：  
1. 语法的一致性，都是通过public的接口来进行成员变量的访问
2. 更精细的控制，可以控制某些成员变量只能访问， 有些能被赋值
3. 封装

# clause23. 非成员非友元函数替换成员函数

```cpp
class WebBrowser{

....

public:
    void clearCache();
    void clearHistory();
    void removeCookies();

    void clearBrowser();//调用上述三个方法，不推荐
};

// 推荐
void clearBrowser(Webbrowser& wb){
    wb.clearCache();
    wb.clearHistory();
    wb.removeCookies();
    ...
}

```
* 这样的好处是更加灵活，方便扩展

# clause24. 当所有参数都需要进行类型转换时，将函数定义为非成员函数


```cpp

class Rational{
public:

    Ration(int num = 0, int d = -1) //由于没声明explicit所以会隐式转换
    {
        num_ = num;
        d_ = d;
    }

    int getnum()const{
        return num_;
    }

    int getd()const{
        return d_;
    }


    const Rational operator*(const Rational& rhs){
        return {num_ * rhs.num_ , d_ * rhs.d_};
    } // 这种方法在int res = rhs * 2时，有效，2会隐式转换一个Rational的对象，但 2* Rational这种形式就会报错，除非友元
private:
    int num_;
    int d_;
};


//推荐做法

const Rational operator* (const Rational& rhs, const Rational& lhs){
    return {rhs.getnum() * lhs.getnum() , rhs.getd() * lhs.getd()};
}
```

# clasue25. 自定义swap函数
* 常见于是智能指针中，拷贝赋值和移动赋值函数，例子如下：
  
1. unique_ptr
```cpp

template<typename T>
class unique_ptr{

public:
    unique_ptr(unique_ptr&& other){
        ptr_ = other.release();
    }
    
    template<typename U>
    unique_ptr(unique_ptr<U>&& other){
        ptr_ = other.release();
    }


    /*
    unique_ptr& operator=(const unqiue_ptr& rhs){
        unique_ptr temp(rhs);
        temp.swap(*this);
        return *this;
    }
    对比和一下赋值运算函数中以值传参相比，按引用传参会更加繁琐
    */
    unique_ptr& operator=(unique_ptr rhs){
        rhs.swap(*this);
        return *this;
    }

    T* release(){
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(unique_ptr& rhs){
        using std::swap;
        swap(ptr_,rhs.ptr_);
    }



    T* operator->()const noexcept{
        return ptr_;
    }

    T& operator*()const noexcept{
        return *ptr_;
    }

    operator bool()const noexcept{
        return ptr;
    }
private:
    T* ptr_;
}


```