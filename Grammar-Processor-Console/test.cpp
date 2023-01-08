#include<bits/stdc++.h>


#include"Symbol.h"


using namespace std;


class A {
public:
	A(int b):a(b){}
	int a;
	operator string() {
		return to_string(a);
	}
};


enum class Type {
	set,
	pointer,
};


template<class T>
 Type type(T* p) {
	return Type::pointer;
}
template<class T>
Type type(set<T>& se) {
	return Type::set;
}
string typestring(Type t) {
	switch (t)
	{
	case Type::set:
		return "set";
		break;
	case Type::pointer:
		return "pointer";
		break;
	default:
		break;
	}
	return "";
}


int main() {
	int n = 1;
	int* p = &n;
	set<int> se;
	se.insert(n);
	cout << typestring(type(se));
	cout << typestring(type(p));
	return 0;
}