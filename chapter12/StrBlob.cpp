#include<iostream>
#include<vector>
#include<string>
#include<memory>

using namespace std;

class StrBlobPtr;
class strBlob{

friend class StrBlobPtr;
public:
    using  size_type = vector<string>::size_type;   // 别名，size_type 为 vector<string>::size_type
    strBlob();                                        // 构造函数
    strBlob(initializer_list<string> il);           // 空的构造函数，用 initializer_list 初始化
    strBlob(vector<string>*p);                       //
    strBlob(strBlob &s);                              // 拷贝构造函数
    strBlob& operator=(strBlob &rhs);                 //重载赋值运算符
    size_type size() const{return data->size();}
    bool empty() const{return data->empty();}
    void push_back(const string&t) { data->push_back(t);}
    void pop_back();

    // 访问
    string& front();
    const string& front() const;
    string& back();
    const string& back() const;

    //给 strblobptr 的接口
    StrBlobPtr begin();
    StrBlobPtr end();

    //const 版本
    StrBlobPtr begin() const;
    StrBlobPtr end() const;

private:
    shared_ptr<vector<string>> data;
    void check(size_type i,const string&msg) const;

};

inline strBlob::strBlob():data(make_shared<vector<string>>()){ }
inline strBlob::strBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){ }
inline strBlob::strBlob(vector<string> *p): data(p){}
inline strBlob::strBlob(strBlob&s):data(make_shared<vector<string>>(*s.data)){ }

// 重载赋值运算符
inline strBlob& strBlob::operator=(strBlob& rhs)
{
    data = make_shared<vector<string>>(*rhs.data);
    return *this;
}

// 检查位置
inline void strBlob::check(size_type i,const string &msg) const
{
    if(i >= data->size())
        throw out_of_range(msg);
}

//获取前部元素
inline string& strBlob::front()
{
    check(0,"front on empty strBlop");
    return data->front();
}
//前部元素
inline const string& strBlob::front() const
{
    check(0,"front on empty strBlop");
    return data->front();
}

//尾部元素,非 const
inline string&strBlob::back()
{
    check(0,"back on empty strBlob");
    return data->back();
}

//尾部元素，const 版本
inline const string&strBlob::back() const
{
    check(0,"back on empty strBlob");
    return data->back();
}

// pop_Back 工艺区他宝石有
inline void strBlob::pop_back()
{
    check(0,"pop back on empty strBlob");
    data->pop_back();
}


class StrBlobPtr{
    friend bool eq(const StrBlobPtr&,const StrBlobPtr&);
public:
    StrBlobPtr():curr(0){}; // 将当前位置由 0 开始
    StrBlobPtr(strBlob&a,size_t sz=0):wptr(a.data),curr(sz){}   //引用，非 const
    StrBlobPtr(const strBlob&a,size_t sz = 0):wptr(a.data),curr(sz){}  //引用，const 版本

    string& deref() const;          //解引用
    string& deref(int off) const;   //有偏移位置的解引用
    StrBlobPtr& incr();              // ++
    StrBlobPtr& decr();              // --


private:
    shared_ptr<vector<string>> check(size_t i,const string&msg)const;       //检查是否正确
    weak_ptr<vector<string>> wptr;  // weak_ptr，底层 vector 可能被销毁，指向 data 对象
    size_t curr;        // currsor 数组的当前位置
};

// 检查位置
inline shared_ptr<vector<string>> StrBlobPtr::check(size_t i,const string&msg) const
{
    auto ret = wptr.lock(); // vector 是否还存在
    if(!ret)
        throw runtime_error("unbounded StrBlobPtr");
    if(i>= ret->size())
        throw out_of_range(msg);
    return ret;     //否则就返回 `vector`的 shared_ptr

}

// 解引用
inline string&StrBlobPtr::deref() const
{
    auto p = check(curr,"dereference past end");
    return (*p)[curr];
}

//前缀递增
inline StrBlobPtr& StrBlobPtr::incr()
{
    check(curr,"increasement past end of StrBlobPtr");
    ++curr;
    return *this;
}

//前缀递减
inline StrBlobPtr& StrBlobPtr::decr()
{
    --curr;
    check(-1,"decreasement past begin of StrBlobPtr");
    return *this;
}

inline string&StrBlobPtr::deref(int off) const
{
    auto p = check(curr+off,"dereference past end");
    return (*p)[curr + off];
}


//返回第一个指针
inline StrBlobPtr strBlob::begin()
{
    return StrBlobPtr(*this);
}


inline StrBlobPtr strBlob::begin() const
{
    return StrBlobPtr(*this);
}

inline StrBlobPtr strBlob::end()
{
    auto ret = StrBlobPtr(*this,data->size());
    return ret;
}

inline StrBlobPtr strBlob::end() const
{
    auto ret = StrBlobPtr(*this,data->size());
    return ret;
}


inline bool eq(const StrBlobPtr&lhs,const StrBlobPtr&rhs)
{
    auto l = lhs.wptr.lock();
    auto r = rhs.wptr.lock();
    // 如果非空或者两个相等
    if(l==r)
            return (!r || lhs.curr == rhs.curr);
    else
        return false;
}

// 两个是否相等
inline neq(const StrBlobPtr&lhs,const StrBlobPtr& rhs)
{
    return !eq(lhs,rhs);
}


int main()
{
//    vector<int> *p;
    strBlob b1;
    {
    strBlob b2 = {"a","the","an"};
    b1 = b2;
    b2.push_back("about");
    cout << "b2 的大小为: "<< b2.size() << endl;
    cout << "b2的首元素为: "<< b2.front() << endl;

    }

    cout << "b1 的大小为: "<< b1.size() << endl;
    cout << "b1 的首元素为: "<< b1.front() << endl;

    strBlob b3 = b1; //重载赋值运算符
    b3.push_back("next");

    cout << "b3 的大小为: " << b3.size() << endl;
    cout << "b3 的首尾元素为: " << b3.front() << " " << b3.back() << endl;

    cout << "b1的全部元素为: " << endl;
    for(auto it  = b1.begin();neq(it,b1.end());it.incr())
        cout << it.deref() << endl;


}
