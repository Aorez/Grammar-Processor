#pragma once
#include<bits/stdc++.h>


using namespace std;


//���ŵ��࣬��ʾ�ս���ͷ��ս���Ķ���
class Symbol {
private:
	string label;//���ŵ��ַ���

	bool isTerminator;//�Ƿ��ս��


public:
	const static bool TERMINATOR = true;//��̬��������ʾ���ս��

	const static bool NONTERMINATOR = false;//��̬��������ʾ�Ƿ��ս��


	//���캯��
	//Ĭ�Ϲ��캯�����ڶ��徲̬���Աʱ��Ҫ
	Symbol() {
		this->label = string();
		this->isTerminator = false;
	}
	//�������캯������������ֵʱ��Ҫ
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


	//get����
	string getLabel() {
		return this->label;
	}
	string getLabel() const {
		return this->label;
	}
	bool getIsTerminator() {
		return this->isTerminator;
	}


	//������map�������õ���Symbol�࣬������Ҫ���ع�ϵ�����
	//����const�ᱨ��
	//��������غ���
	bool operator < (const Symbol& symbol) const {
		return this->label < symbol.label;
	}
	bool operator > (const Symbol& symbol) const {
		return symbol.label < this->label;
	}
	//����==������ȡ������ʱ�����в���ʽ����ӣ��ж����Ƿ���ͬʱʹ��
	//����ȡ�����ӵ�������·��ʱҲ�õ���
	//������ֱ����ݹ�ʱ���ж��Ƿ���ݹ�
	bool operator == (const Symbol& symbol) const {
		return symbol.label == this->label;
	}
	//����!=����==���෴
	bool operator != (const Symbol& symbol) const {
		return !(*this == symbol);
	}
	//���������������������
	friend ostream& operator<<(ostream& output, const Symbol& symbol) {
		output << symbol.label;
		return output;
	}
	//����string
	operator string() {
		return label;
	}
	string toString() {
		return string(*this);
	}


	//�ж��Ƿ��ս��
	bool isNonTerminator() {
		return this->isTerminator == NONTERMINATOR;
	}


	//�ж��Ƿ񿽱����ţ�A'��A''
	bool isCopySymbol() {
		if (label.find("'") == string::npos) {
			return false;
		}
		return true;
	}
};
