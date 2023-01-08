#pragma once
#include<bits/stdc++.h>


using namespace std;


//符号的类，表示终结符和非终结符的对象
class Symbol {
private:
	string label;//符号的字符串

	bool isTerminator;//是否终结符


public:
	const static bool TERMINATOR = true;//静态常量，表示是终结符

	const static bool NONTERMINATOR = false;//静态常量，表示是非终结符


	//构造函数
	//默认构造函数，在定义静态类成员时需要
	Symbol() {
		this->label = string();
		this->isTerminator = false;
	}
	//拷贝构造函数，函数返回值时需要
	Symbol(const Symbol& symbol) {
		this->label = symbol.label;
		this->isTerminator = symbol.isTerminator;
	}
	Symbol(string label, bool isTerminator) {
		this->label = label;
		this->isTerminator = isTerminator;
	}
	Symbol(char label, bool isTerminator) :Symbol(string(1, label), isTerminator) {
	}


	//get函数
	string getLabel() {
		return this->label;
	}
	string getLabel() const {
		return this->label;
	}
	bool getIsTerminator() {
		return this->isTerminator;
	}


	//由于在map集合中用到了Symbol类，所以需要重载关系运算符
	//不加const会报错
	//运算符重载函数
	bool operator < (const Symbol& symbol) const {
		return this->label < symbol.label;
	}
	bool operator > (const Symbol& symbol) const {
		return symbol.label < this->label;
	}
	//重载==，在提取公因子时，进行产生式的相加，判断左部是否相同时使用
	//在提取公因子的树中走路径时也用到了
	//在消除直接左递归时，判断是否左递归
	bool operator == (const Symbol& symbol) const {
		return symbol.label == this->label;
	}
	//重载!=，和==号相反
	bool operator != (const Symbol& symbol) const {
		return !(*this == symbol);
	}
	//重载输出运算符，方便输出
	friend ostream& operator<<(ostream& output, const Symbol& symbol) {
		output << symbol.label;
		return output;
	}
	//重载string
	operator string() {
		return label;
	}
	string toString() {
		return string(*this);
	}


	//判断是否终结符
	bool isNonTerminator() {
		return this->isTerminator == NONTERMINATOR;
	}


	//判断是否拷贝符号，A'，A''
	bool isCopySymbol() {
		if (label.find("'") == string::npos) {
			return false;
		}
		return true;
	}
};
