#include<iostream>
#include<vector>
#include<string>
#include<memory>

using namespace std;

class StrBlobPtr;
class strBlob{

friend class StrBlobPtr;
public:
    using  size_type = vector<string>::size_type;   // ������size_type Ϊ vector<string>::size_type
    strBlob();                                        // ���캯��
    strBlob(initializer_list<string> il);           // �յĹ��캯������ initializer_list ��ʼ��
    strBlob(vector<string>*p);                       //
    strBlob(strBlob &s);                              // �������캯��
    strBlob& operator=(strBlob &rhs);                 //���ظ�ֵ�����
    size_type size() const{return data->size();}
    bool empty() const{return data->empty();}
    void push_back(const string&t) { data->push_back(t);}
    void pop_back();

    // ����
    string& front();
    const string& front() const;
    string& back();
    const string& back() const;

    //�� strblobptr �Ľӿ�
    StrBlobPtr begin();
    StrBlobPtr end();

    //const �汾
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

// ���ظ�ֵ�����
inline strBlob& strBlob::operator=(strBlob& rhs)
{
    data = make_shared<vector<string>>(*rhs.data);
    return *this;
}

// ���λ��
inline void strBlob::check(size_type i,const string &msg) const
{
    if(i >= data->size())
        throw out_of_range(msg);
}

//��ȡǰ��Ԫ��
inline string& strBlob::front()
{
    check(0,"front on empty strBlop");
    return data->front();
}
//ǰ��Ԫ��
inline const string& strBlob::front() const
{
    check(0,"front on empty strBlop");
    return data->front();
}

//β��Ԫ��,�� const
inline string&strBlob::back()
{
    check(0,"back on empty strBlob");
    return data->back();
}

//β��Ԫ�أ�const �汾
inline const string&strBlob::back() const
{
    check(0,"back on empty strBlob");
    return data->back();
}

// pop_Back ����������ʯ��
inline void strBlob::pop_back()
{
    check(0,"pop back on empty strBlob");
    data->pop_back();
}


class StrBlobPtr{
    friend bool eq(const StrBlobPtr&,const StrBlobPtr&);
public:
    StrBlobPtr():curr(0){}; // ����ǰλ���� 0 ��ʼ
    StrBlobPtr(strBlob&a,size_t sz=0):wptr(a.data),curr(sz){}   //���ã��� const
    StrBlobPtr(const strBlob&a,size_t sz = 0):wptr(a.data),curr(sz){}  //���ã�const �汾

    string& deref() const;          //������
    string& deref(int off) const;   //��ƫ��λ�õĽ�����
    StrBlobPtr& incr();              // ++
    StrBlobPtr& decr();              // --


private:
    shared_ptr<vector<string>> check(size_t i,const string&msg)const;       //����Ƿ���ȷ
    weak_ptr<vector<string>> wptr;  // weak_ptr���ײ� vector ���ܱ����٣�ָ�� data ����
    size_t curr;        // currsor ����ĵ�ǰλ��
};

// ���λ��
inline shared_ptr<vector<string>> StrBlobPtr::check(size_t i,const string&msg) const
{
    auto ret = wptr.lock(); // vector �Ƿ񻹴���
    if(!ret)
        throw runtime_error("unbounded StrBlobPtr");
    if(i>= ret->size())
        throw out_of_range(msg);
    return ret;     //����ͷ��� `vector`�� shared_ptr

}

// ������
inline string&StrBlobPtr::deref() const
{
    auto p = check(curr,"dereference past end");
    return (*p)[curr];
}

//ǰ׺����
inline StrBlobPtr& StrBlobPtr::incr()
{
    check(curr,"increasement past end of StrBlobPtr");
    ++curr;
    return *this;
}

//ǰ׺�ݼ�
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


//���ص�һ��ָ��
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
    // ����ǿջ����������
    if(l==r)
            return (!r || lhs.curr == rhs.curr);
    else
        return false;
}

// �����Ƿ����
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
    cout << "b2 �Ĵ�СΪ: "<< b2.size() << endl;
    cout << "b2����Ԫ��Ϊ: "<< b2.front() << endl;

    }

    cout << "b1 �Ĵ�СΪ: "<< b1.size() << endl;
    cout << "b1 ����Ԫ��Ϊ: "<< b1.front() << endl;

    strBlob b3 = b1; //���ظ�ֵ�����
    b3.push_back("next");

    cout << "b3 �Ĵ�СΪ: " << b3.size() << endl;
    cout << "b3 ����βԪ��Ϊ: " << b3.front() << " " << b3.back() << endl;

    cout << "b1��ȫ��Ԫ��Ϊ: " << endl;
    for(auto it  = b1.begin();neq(it,b1.end());it.incr())
        cout << it.deref() << endl;


}
