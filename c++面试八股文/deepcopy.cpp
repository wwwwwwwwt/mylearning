#include <bits/stdc++.h>

using namespace std;


class Person
{
public:
    Person(const char* name, int age):age(age)
    {
        m_name = new char[strlen(name) + 1];
        strcpy(m_name,name);
    }

    ~Person(){
        delete []m_name;
    }

    Person(const Person& p){
        age = p.age;
        m_name = new char[strlen(p.m_name) + 1];
        strcpy(m_name, p.m_name);
    }

    Person& operator=(Person p){
        swap(m_name,p.m_name);
        swap(age, p.age);
        return *this;
    }
    
    void get()const{
        cout<<m_name<<" "<<age<<endl;
    }

    void changename(string str){
        
        char* newname = new char[str.size() + 1];
        strcpy(newname, str.c_str());
        delete []m_name;
        m_name = newname;
    }

private:
    char* m_name;
    int age;
};

int main(){

    Person b("ztw",24);
    Person c = b;
    c.changename("wgl");
    Person d(c);
    d.changename("klh");
    b.get();
    c.get();
    d.get();
    return 0;

}