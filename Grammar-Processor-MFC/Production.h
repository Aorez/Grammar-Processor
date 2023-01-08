#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Alphabet.h"


using namespace std;


//产生式的类
class Production {
private:
	Symbol left;//产生式的左部

	vector<Symbol> right;//产生式的右部

public:
	const static int END_OF_RIGHT = -1;//表示截取右部时，直接截取到右部末尾

	const static int SYMBOL_NOT_FOUND = -1;//在右部中找不到对应符号时的返回值

	//构造函数
	//默认构造函数，在提取左公因子递归函数中，最后的返回值中使用
	Production() {
		this->left = Symbol();
		this->right = vector<Symbol>();
	}
	//拷贝构造函数
	Production(const Production& p) {
		//在拷贝构造函数中可以访问私有成员
		this->left = p.left;
		this->right = p.right;
	}
	//传递左部及右部进行构造
	Production(Symbol left, vector<Symbol> right) {
		this->left = left;
		this->right = right;
	}
	//用一个符号集合的集合来构造产生式，用于产生式的相加
	Production(Symbol left, vector<vector<Symbol>> rightParts) {
		this->left = left;
		for (auto& rightPart : rightParts) {
			this->right.insert(this->right.end(), rightPart.begin(), rightPart.end());
		}
	}


	//get函数
	Symbol getLeft() {
		return this->left;
	}
	vector<Symbol> getRight() {
		return this->right;
	}
	//获得右部符号数量
	int getRightSize() {
		return this->right.size();
	}


	//set函数
	void setLeft(Symbol symbol) {
		this->left = symbol;
	}


	//重载运算符
	//重载方括号[]，方便获取右部
	Symbol operator [] (int index) const {
		//如果下标不合法，抛出异常
		if (index < 0 || index >= right.size()) {
			throw exception();
		}

		return right[index];
	}
	//重载==，在判断产生式是否已存在时使用
	bool operator == (const Production& p) const {
		return this->rightToString() == p.rightToString();
	}
	//重载输出运算符，方便输出
	//一个左部一个右部地输出产生式
	friend ostream& operator<<(ostream& output, const Production& production) {
		Symbol left = production.left;
		vector<Symbol> right = production.right;
		output << left << "->";
		for (const auto& it : right) {
			output << it;
		}
		return output;
	}
	//按“A->a|b”这样的格式进行产生式的输出
	friend ostream& operator<<(ostream& output, const vector<Production>& productions) {
		Symbol left = productions[0].left;
		output << left << "->";
		string splitCharacter = "";
		for (auto& it : productions) {
			vector<Symbol> right = it.right;
			cout << splitCharacter;
			for (const auto& it2 : right) {
				output << it2;
			}
			splitCharacter = "|";
		}
		return output;
	}
	//重载string
	operator string() {
		return string(left) + "->" + Util::vectorToString(right, "");
	}
	string toString() {
		return string(*this);
	}


	//判断是否存在左递归
	bool isLeftRecursion() {
		return left == right[0];
	}


	//判断右部是否全由终结符组成
	//在不可终止中，提前将全由终结符组成的产生式的左部设置为可终止
	bool isAllMadeOfTerminators() {
		for (auto& rightSymbol : right) {
			//如果有非终结符，直接返回
			if (rightSymbol.isNonTerminator()) {
				return false;
			}
		}
		return true;
	}


	//判断是否空字
	//如果产生式的右部集合长度大于1，返回false
	//长度为1时，判断空字
	bool isEpsilon() {
		return Alphabet::isEpsilon(rightToString());
	}


	//将右部变成string类型，方便符号的查看和判断
	string rightToString() const {
		string rightString = "";
		for (auto& it : this->right) {
			rightString += it.getLabel();
		}
		return rightString;
	}


	//判断右部中是否有对应符号出现
	bool rightHasLabel(string label) {
		string rightString = this->rightToString();//右部转为字符串

		//如果右部的字符串中找不到对应符号，返回false
		if (rightString.find(label) == string::npos) {
			return false;
		}
		return true;
	}


	//向右部的最后加入符号
	//在提取公因子进行代入推导时使用
	void addRightSymbol(Symbol symbolToAdd) {
		right.push_back(symbolToAdd);
	}


	//对右部进行截取，在间接左递归的替换中使用，截取第二个符号到最后
	//是左闭右开的
	vector<Symbol> cutRight(int leftClosed, int rightOpened = END_OF_RIGHT) {
		if (rightOpened < 0) {
			rightOpened = right.size();
		}
		vector<Symbol> cutResult;
		for (int i = leftClosed; i < rightOpened; i++) {
			cutResult.push_back(right[i]);
		}
		return cutResult;
	}


	//在右部中查找某个符号，查找不到返回查找不到的标志值
	int findSymbol(Symbol symbolToFind) {
		for (int i = 0; i < right.size(); i++) {
			if (symbolToFind == right[i]) {
				return i;
			}
		}
		return SYMBOL_NOT_FOUND;
	}


	//判断产生式是否左线性
	bool isLeftLinear() {
		switch (right.size())
		{
		case 0:return true;//空串
		case 1:return right[0].isNonTerminator() == false;//一个终结符，包括空串
		case 2:return right[0].isNonTerminator() && right[1].isNonTerminator() == false;//左边非终结符右边终结符
		default:
			return false;
		}
	}
	//判断产生式是否右线性
	bool isRightLinear() {
		switch (right.size())
		{
		case 0:return true;//空串
		case 1:return right[0].isNonTerminator() == false;//一个终结符，包括空串
		case 2:return right[1].isNonTerminator() && right[0].isNonTerminator() == false;//左边非终结符右边终结符
		default:
			return false;
		}
	}


	//判断产生式的右部中是否有A'，A''这种拷贝符号
	bool hasCopySymbol() {
		for (auto symbol : right) {
			//拷贝符号只有非终结符才有
			if (symbol.isNonTerminator()) {
				//如果符号中有单引号
				if (symbol.getLabel().find("'") != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
};
