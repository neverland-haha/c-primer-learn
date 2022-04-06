#include<iostream>
#include<memory>
#include<vector>
#include<utility>
using namespace std;


class StrVec{

public:
    //���캯��
    StrVec():elements(nullptr),first_free(nullptr),cap(nullptr){}
    StrVec(initializer_list<string> str_vec);
    StrVec(const StrVec&);
    StrVec& operator=(const StrVec&);
    ~StrVec();
    void push_back(const string&);
    size_t size() const {return first_free - elements;}
    size_t capacity() const {return cap - elements;}
    string *begin() const {return elements;}
    string *end() const {return first_free;}
private:
    static allocator<string> alloc;  //����Ԫ��
    void chk_n_alloc(){if (size() == capacity()) reallocate();}   // ��� size �� capcity ��ȵĻ�������Ҫ�ٷ����ڴ�
    pair<string*,string*> alloc_n_copy(const string*,const  string*);
    void free();
    void reallocate();
    string *elements;
    string *first_free;
    string *cap;
};


allocator<string> StrVec::alloc;

void StrVec::push_back(const string&s)
{
    chk_n_alloc();  // ȷ�����㹻�Ŀռ�
    alloc.construct(first_free++,s);        // ���ݵ���ָ��.
}

pair<string*,string*>
StrVec::alloc_n_copy(const string*b,const string*e)
{
    auto data = alloc.allocate(e-b);        //���䷶Χ�еĿռ�
    return {data,uninitialized_copy(b,e,data)}; //
}

void StrVec::free()
{
    if(elements){
        //
        for(auto p = first_free;p!=elements;)
            alloc.destroy(--p);     //��������
            alloc.deallocate(elements,cap-elements);//ɾ���ڴ�
    }
}

StrVec::StrVec(std::initializer_list<std::string> str_vec)  //ʵ���ϣ�����StrVec�ڲ�ʵ�־��Ƕ�̬������� Ӧ��Ҫ����
{
	auto newdata = alloc_n_copy(str_vec.begin(), str_vec.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}
StrVec::StrVec(const StrVec &v)
{
	auto newdata = alloc_n_copy(v.begin(), v.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec::~StrVec(){free();}

StrVec& StrVec::operator=(const StrVec&rhs)
{
    auto data = alloc_n_copy(rhs.begin(),rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::reallocate()
{
    auto newcapcity = size()? 2 * size():1; //���䵱ǰ 2 �����ڴ�
    //
    auto newdata = alloc.allocate(newcapcity);
    auto dest = newdata;
    auto elem = elements;
    for(size_t i = 0;i!=size();++i)
        alloc.construct(dest++,std::move(*elem++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapcity;

}

int main()
{

    StrVec vec{"hello","world"};
    cout << vec.size() << endl;
    for(auto i:vec)
        cout << i << endl;



}
