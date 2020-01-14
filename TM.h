#ifndef TURINGMACHINE_TM_H
#define TURINGMACHINE_TM_H

#include <string>
#include <iostream>
using namespace std;

typedef int Rank;

#ifndef DEFAULT_CAPACITY
#define DEFAULT_CAPACITY 100
#endif

template <class T>
class Vector{
private:
    T *_elem;
    int _size;
    int _capacity;
protected:
    void expand();
    void shrink();
    void sshrink();
public:
    Vector():_size(0),_capacity(DEFAULT_CAPACITY){_elem=new T[_capacity];}
    Vector(int n,const T &e):_size(n),_capacity(n){_elem=new T[n];for(int i=0;i<n;i++) _elem[i]=e;}
    Vector(T *elem,int size);
    Vector(const Vector<T> &v);
    ~Vector(){delete [] _elem;}
    bool empty() const {return _size==0;}
    T& operator[](Rank r){return _elem[r];}
    T& operator[](Rank r) const {return _elem[r];}
    int size() const {return _size;}
    void push_back(const T &e);
    void insert(const T &e,Rank i);//insert sth in front of _elem[i]
    T pop_back();
    void clear();
    T& last() {return _elem[_size-1];}
    Rank search(const T &e) const;//attention:if not found,return -1
    Rank binarysearch(const T &e,Rank lo,Rank hi) const;//attention:may return _size-1 or -1
    Rank binarysearch(const T &e) const {return binarysearch(e,0,_size);}
};

template <class T>
Vector<T>::Vector(T *elem,int size) {
    _elem=new T[_capacity=_size=size];
    for(Rank i=0;i<size;i++) _elem[i]=elem[i];
}

template <class T>
Vector<T>::Vector(const Vector<T> &v) {
    _elem=new T[_capacity=v._capacity];
    _size=v.size();
    for(Rank i=0;i<v.size();i++) _elem[i]=v[i];
}

template <class T>
void Vector<T>::expand() {
    T *n_elem=new T[_capacity<<=1];
    for(Rank i=0;i<_size;i++) n_elem[i]=_elem[i];
    delete [] _elem;
    _elem=n_elem;
}

template <class T>
void Vector<T>::shrink() {
    T *n_elem = new T[_capacity >>= 1];
    for(Rank i = 0; i < _size; i++) n_elem[i] = _elem[i];
    delete[] _elem;
    _elem = n_elem;
}

template <class T>
void Vector<T>::sshrink() {
    T *nelem=new T[_size=_capacity=DEFAULT_CAPACITY];
    for(int i=0;i<_capacity;i++) nelem[i]=_elem[i];
    delete [] _elem;
    _elem=nelem;
}

template<class T>
void Vector<T>::clear() {
    if(_capacity>DEFAULT_CAPACITY) sshrink();
    _size=0;
}

template <class T>
void Vector<T>::push_back(const T &e) {
    if(_size==_capacity) expand();
    _elem[_size++]=e;
}

template <class T>
T Vector<T>::pop_back() {
    T tmp=_elem[--_size];
    if(_size<_capacity>>1) shrink();
    return tmp;
}

template<class T>
Rank Vector<T>::search(const T &e) const {
    for(Rank i=_size-1;i>=0;i--)
        if(_elem[i]==e) return i;
    return -1;
}

template <class T>
Rank Vector<T>::binarysearch(const T &e, Rank lo, Rank hi) const {
    while(lo<hi){
        Rank mi=(lo+hi)>>1;
        (e<_elem[mi])?hi=mi:lo=mi+1;
    }
    return --lo;
}

template <class T>
void Vector<T>::insert(const T &e,Rank r) {
    if(_size==_capacity) expand();
    for(Rank i=_size;i>r;i--)
        _elem[i]=_elem[i-1];
    _size++;
    _elem[r]=e;
}


typedef  int Status_NO;

typedef enum{STABLE,LEFT,RIGHT} Direction;

const char dir[]={'S','L','R'};

struct trans_tuple{
    Status_NO new_status;
    char new_char;
    Direction new_direction;
    trans_tuple():new_status(-1),new_char('X'),new_direction(STABLE){}
};

struct trans_tuple_pair{
    trans_tuple trans[2];
    trans_tuple_pair(){}
};

class TM {
    private:
        string tape;
        string code;
        int char_n;
        int status_n;
        int currentStatus;
        Vector<trans_tuple_pair > trans_function;
        Rank cursor;
    protected:
        int read_some_0(Rank &start,int count);
        int read_some_1(Rank &start,int count);
        int read_all_0(Rank &start);
        int read_all_1(Rank &start);
    public:
        TM(){char_n=2;status_n=cursor=0;}
        ~TM(){}
        void acceptCode(){cin>>code;}
        void setCode(const string &s){code=s;}
        void analyseCode();
        void showCode(){cout<<code<<endl;}
        void acceptInput();
        void run();
        void show_graph();
};

int TM::read_all_0(Rank &start) {
    if(code[start]!='0') return 0;
    int cnt=0;
    while(code[start]=='0') {cnt++;start++;}
    return cnt;
}

int TM::read_all_1(Rank &start){
    if(code[start]!='1') return 0;
    int cnt=0;
    while(code[start]=='1') {cnt++;start++;}
    return cnt;
}

void TM::analyseCode() {
    for(Rank i=0;i<code.length();){
        int former_status=read_all_0(i);//read original status
        trans_function.push_back(trans_tuple_pair());
        if(read_all_1(i)!=1) exit(-1);

        int former_char_no=read_all_0(i);//read original char on the tape
        if(former_char_no!=1&&former_char_no!=2) exit(-1);

        if(read_all_1(i)!=1) exit(-1);
        int new_status_no=read_all_0(i);//read new status
        status_n=status_n>new_status_no?status_n:new_status_no;
        trans_function[former_status-1].trans[former_char_no-1].new_status=new_status_no-1;

        if(read_all_1(i)!=1) exit(-1);
        int new_char_no=read_all_0(i);//read new char
        if(new_char_no!=1&&new_char_no!=2) exit(-1);
        trans_function[former_status-1].trans[former_char_no-1].new_char='0'-1+new_char_no;

        if(read_all_1(i)!=1) exit(-1);
        int direction=read_all_0(i);//read direction
        if(direction==1) trans_function[former_status-1].trans[former_char_no-1].new_direction=LEFT;
        else {
            if (direction == 2) trans_function[former_status-1].trans[former_char_no-1].new_direction = RIGHT;
            else exit(-1);
        }
        if(i<=code.length()-2&&read_all_1(i)!=2) exit(-1);
    }
}

void TM::acceptInput() {std::cin>>tape;}

void TM::run() {
    currentStatus=0;
    cursor=0;
    while(cursor<tape.size()){
        int formerStatus=currentStatus;
        int formerChar=tape[cursor]-'0';
        if(trans_function[formerStatus].trans[formerChar].new_char=='X') break;
        tape[cursor]=trans_function[formerStatus].trans[formerChar].new_char;
        currentStatus=trans_function[formerStatus].trans[formerChar].new_status;
        if(trans_function[formerStatus].trans[formerChar].new_direction==RIGHT) cursor++;
        else {if(trans_function[formerStatus].trans[formerChar].new_direction==LEFT) cursor--;
              else break;
        }
    }
    if(currentStatus!=1) cout<<"Reject!"<<endl;
    else cout<<"Accept!"<<endl;
}

void TM::show_graph() {
    for(Rank i=0;i<status_n;i++) {
        cout<<i<<"\t"
            <<"0"<<"\t"
            <<trans_function[i].trans[0].new_status<<"\t"
            <<trans_function[i].trans[0].new_char<<"\t"
            <<dir[trans_function[i].trans[0].new_direction]<<endl;
        cout<<i<<"\t"
            <<"1"<<"\t"
            <<trans_function[i].trans[1].new_status<<"\t"
            <<trans_function[i].trans[1].new_char<<"\t"
            <<dir[trans_function[i].trans[1].new_direction]<<endl;
    }
}

#endif //TURINGMACHINE_TM_H
