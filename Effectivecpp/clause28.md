# 注意按引用的方式返回对象的成员
1. 注意返回时是否会会修改被const修饰的成员变量

```cpp

class Point{
public:
Point(int x,int y);
...
void setx(int val);
void sety(int val);
...

};

struct Recdata{
    Point ulnc;
    Point lrhc;
};

class Rectangle{


Point& upperleft()const{return pData->ulnc;}
Point& lowerRight()const{return pData->lrhc;}
private:
sta::shared_ptr<Recdata> pData_;
}

Point cord1(0,0);
Point cord2(100,100);

const Rectangle rec(cord1,cord2);
rec.upperleft().setx(50);
这句话和上面那个被const修饰成员变量相悖，
修改的方法为返回值为const Point&

```

2.注意按引用返回一个，如果绑定的是一个临时变量，那么再次访问是临时变量已经被释放的内存，修改方法声明个变量代替临时变量

```cpp
class GuiObject{...}
const Rectangle boundingBox(const GuiObject& obj);


GuiObject *pgo;
const Point* pUpperleft = &(boundingBox(*pgp).upperleft());
这种就是绑定临时变量
修改方式
auto t = boundingBox(*pgp).upperleft();
const Point* pUpperleft = &t;

```