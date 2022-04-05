#include<iostream>
#include<string>
using namespace std;

class HasPtr{
    friend void swap(HasPtr&,HasPtr&);
public:
    HasPtr(const string&s = string()):ps(new string(s)),i(0),use(new size_t(1)){ }
    HasPtr(HasPtr &p):ps(p.ps),i(p.i),use(p.use){++*use;}
    HasPtr& operator=(const HasPtr&);
    HasPtr& operator=(const string&);
    string& operator*();
    ~HasPtr();

private:
    string *ps;
    int i;
    size_t *use;
};

HasPtr:: ~HasPtr()
{
    if(--*use)
    {
        delete ps;
        delete use;
    }
}

HasPtr& HasPtr::operator=(const HasPtr&rhs)
{
    ++*rhs.use;
    if(--*rhs.use == 0)
    {
        delete ps;
        delete use;
    }
}

//���ظ�ֵ�����
HasPtr& HasPtr::operator=(const string&rhs)
{
    *ps = rhs;
    return *this;
}

// ����
string& HasPtr::operator*()
{
    return *ps;
}

void swap(HasPtr&lhs,HasPtr&rhs)
{   cout << "����" << *lhs.ps << "��" << *rhs.ps << endl;
    swap(lhs.ps,rhs.ps);
    swap(lhs.i,rhs.i);
}

int main(int argc,char **argv)
{

  HasPtr h("hi mom");
  HasPtr h2("hi dad");
//  h = " hi dad!";
  cout << "h: "<< *h << endl;
  cout << "h2: " << *h2 <<endl;
  swap(h,h2);
  cout << "h: "<< *h << endl;
  cout << "h2: " << *h2 <<endl;
  return 0;


}
