#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Alphabet.h"


using namespace std;


//����ʽ����
class Production {
private:
	Symbol left;//����ʽ����

	vector<Symbol> right;//����ʽ���Ҳ�

public:
	const static int END_OF_RIGHT = -1;//��ʾ��ȡ�Ҳ�ʱ��ֱ�ӽ�ȡ���Ҳ�ĩβ

	const static int SYMBOL_NOT_FOUND = -1;//���Ҳ����Ҳ�����Ӧ����ʱ�ķ���ֵ

	//���캯��
	//Ĭ�Ϲ��캯��������ȡ�����ӵݹ麯���У����ķ���ֵ��ʹ��
	Production() {
		this->left = Symbol();
		this->right = vector<Symbol>();
	}
	//�������캯��
	Production(const Production& p) {
		//�ڿ������캯���п��Է���˽�г�Ա
		this->left = p.left;
		this->right = p.right;
	}
	//�����󲿼��Ҳ����й���
	Production(Symbol left, vector<Symbol> right) {
		this->left = left;
		this->right = right;
	}
	//��һ�����ż��ϵļ������������ʽ�����ڲ���ʽ�����
	Production(Symbol left, vector<vector<Symbol>> rightParts) {
		this->left = left;
		for (auto& rightPart : rightParts) {
			this->right.insert(this->right.end(), rightPart.begin(), rightPart.end());
		}
	}


	//get����
	Symbol getLeft() {
		return this->left;
	}
	vector<Symbol> getRight() {
		return this->right;
	}
	//����Ҳ���������
	int getRightSize() {
		return this->right.size();
	}


	//set����
	void setLeft(Symbol symbol) {
		this->left = symbol;
	}


	//���������
	//���ط�����[]�������ȡ�Ҳ�
	Symbol operator [] (int index) const {
		//����±겻�Ϸ����׳��쳣
		if (index < 0 || index >= right.size()) {
			throw exception();
		}

		return right[index];
	}
	//����==�����жϲ���ʽ�Ƿ��Ѵ���ʱʹ��
	bool operator == (const Production& p) const {
		return this->rightToString() == p.rightToString();
	}
	//���������������������
	//һ����һ���Ҳ����������ʽ
	friend ostream& operator<<(ostream& output, const Production& production) {
		Symbol left = production.left;
		vector<Symbol> right = production.right;
		output << left << "->";
		for (const auto& it : right) {
			output << it;
		}
		return output;
	}
	//����A->a|b�������ĸ�ʽ���в���ʽ�����
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
	//����string
	operator string() {
		return string(left) + "->" + Util::vectorToString(right, "");
	}
	string toString() {
		return string(*this);
	}


	//�ж��Ƿ������ݹ�
	bool isLeftRecursion() {
		return left == right[0];
	}


	//�ж��Ҳ��Ƿ�ȫ���ս�����
	//�ڲ�����ֹ�У���ǰ��ȫ���ս����ɵĲ���ʽ��������Ϊ����ֹ
	bool isAllMadeOfTerminators() {
		for (auto& rightSymbol : right) {
			//����з��ս����ֱ�ӷ���
			if (rightSymbol.isNonTerminator()) {
				return false;
			}
		}
		return true;
	}


	//�ж��Ƿ����
	//�������ʽ���Ҳ����ϳ��ȴ���1������false
	//����Ϊ1ʱ���жϿ���
	bool isEpsilon() {
		return Alphabet::isEpsilon(rightToString());
	}


	//���Ҳ����string���ͣ�������ŵĲ鿴���ж�
	string rightToString() const {
		string rightString = "";
		for (auto& it : this->right) {
			rightString += it.getLabel();
		}
		return rightString;
	}


	//�ж��Ҳ����Ƿ��ж�Ӧ���ų���
	bool rightHasLabel(string label) {
		string rightString = this->rightToString();//�Ҳ�תΪ�ַ���

		//����Ҳ����ַ������Ҳ�����Ӧ���ţ�����false
		if (rightString.find(label) == string::npos) {
			return false;
		}
		return true;
	}


	//���Ҳ������������
	//����ȡ�����ӽ��д����Ƶ�ʱʹ��
	void addRightSymbol(Symbol symbolToAdd) {
		right.push_back(symbolToAdd);
	}


	//���Ҳ����н�ȡ���ڼ����ݹ���滻��ʹ�ã���ȡ�ڶ������ŵ����
	//������ҿ���
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


	//���Ҳ��в���ĳ�����ţ����Ҳ������ز��Ҳ����ı�־ֵ
	int findSymbol(Symbol symbolToFind) {
		for (int i = 0; i < right.size(); i++) {
			if (symbolToFind == right[i]) {
				return i;
			}
		}
		return SYMBOL_NOT_FOUND;
	}


	//�жϲ���ʽ�Ƿ�������
	bool isLeftLinear() {
		switch (right.size())
		{
		case 0:return true;//�մ�
		case 1:return right[0].isNonTerminator() == false;//һ���ս���������մ�
		case 2:return right[0].isNonTerminator() && right[1].isNonTerminator() == false;//��߷��ս���ұ��ս��
		default:
			return false;
		}
	}
	//�жϲ���ʽ�Ƿ�������
	bool isRightLinear() {
		switch (right.size())
		{
		case 0:return true;//�մ�
		case 1:return right[0].isNonTerminator() == false;//һ���ս���������մ�
		case 2:return right[1].isNonTerminator() && right[0].isNonTerminator() == false;//��߷��ս���ұ��ս��
		default:
			return false;
		}
	}


	//�жϲ���ʽ���Ҳ����Ƿ���A'��A''���ֿ�������
	bool hasCopySymbol() {
		for (auto symbol : right) {
			//��������ֻ�з��ս������
			if (symbol.isNonTerminator()) {
				//����������е�����
				if (symbol.getLabel().find("'") != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
};
