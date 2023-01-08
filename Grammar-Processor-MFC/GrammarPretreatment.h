#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Grammar.h"
#include"Production.h"


#include"Alphabet.h"


using namespace std;


//�ķ�Ԥ����ľ�̬��
static class GrammarPretreatment {
private:
	static vector<string> inputProductions;//���ļ��ж���Ĳ���ʽ

	static vector<string> tooShortProductions;//����ʽ̫�̣�����ʽ����Ҫ�����3

	static vector<string> beginningErrorProductions;//����ʽ��ͷ��ʽ���󣬲���ʽ��ͷ��ʽӦ���ơ�A->��

	static vector<string> selfProductions;//�Բ���ʽ�ļ��ϣ�����A->A���������ַ���

	static vector<string> pretreatedProductions;//Ԥ�����Ĳ���ʽ���뼯��

	static string grammarFileName;//�ķ��ļ����ļ���

	static Grammar grammar;//Ԥ�������ķ�

	const static string LACK_START_ERROR;//ȱ�ٿ�ʼ���ź�ĳ�����ʾ


	//������ʽ����|�����зָ�
	static vector<string> splitProduction(string productionToSplit) {
		string left = productionToSplit.substr(0, 3);//�������ʽ���󲿣���A->��
		productionToSplit = productionToSplit.substr(3);//�����Ҳ�����b|c|d��
		vector<string> rightsSplit = Util::splitString(productionToSplit, Grammar::productionSplitSymbol);//�ָ��������Ҳ�
		vector<string> productionsSplit;
		//���������Ҳ���������Ӻ󣬷��ؼ���
		for (auto right : rightsSplit) {
			productionsSplit.push_back(left + right);
		}
		return productionsSplit;
	}


	//�ж��Ƿ��Բ���ʽ
	static bool isSelfProduction(string production) {
		//�Բ���ʽ��ʽ��A->A��
		if (production.size() == 4 && production[0] == production[3]) {
			return true;
		}
		return false;
	}


	//���ļ��ж����ķ�����
	static void readGrammarFile() {
		ifstream ifs;
		ifs.open(grammarFileName, ios::in);
		string fileLine;//���ļ��ж�ȡ����һ��
		while (!ifs.eof()) {
			getline(ifs, fileLine);
			//�����һ��Ϊ��
			if (fileLine.empty()) {
				continue;
			}
			inputProductions.push_back(fileLine);
		}
	}


	//Ԥ������ļ��ж�����ķ����򣬽���ȱ�Ĳ���ʽ�����Ϸ��ķ���ɾȥ����
	static void pretreatInputProductions() {
		//��ȡÿһ������ʽ�ַ���
		for (string production : inputProductions) {
			//ɾ������ʽ�еĿո�
			production = Util::filterCharacter(production, { ' ' }, Util::NOT_RETAIN);

			//����ʽ̫�̣�����ʽҪ�󳤶ȴ���3
			if (production.size() <= 3) {
				tooShortProductions.push_back(production);
				continue;
			}

			//ע��
			if (production[0] == '/' && production[1] == '/') {
				continue;
			}

			//����ʽ�Ŀ�ͷ��������ʽ�Ŀ�ͷӦ�����ơ�A->�������ҵ�һλ�涨Ϊ��д��ĸ
			if (production[0] < 'A' || production[0] > 'Z' || production[1] != '-' || production[2] != '>') {
				beginningErrorProductions.push_back(production);
				continue;
			}

			//���˲��Ǵ�д��ĸСд��ĸ��|����@�����ַ�
			vector<char> characters({ '|','@' });
			for (char c = 'A'; c <= 'Z'; c++) {
				characters.push_back(c);
			}
			for (char c = 'a'; c <= 'z'; c++) {
				characters.push_back(c);
			}
			//����ʽ�Ŀ�ͷ��Ҫ��ȡ�����ٽ��й���
			production = production.substr(0, 3) + Util::filterCharacter(production.substr(3), characters, Util::RETAIN);

			//������ʽ����|�����зָ�
			//���ж��Ƿ��Բ���ʽ��ֻ��Ҫ�ж��Ƿ�A->A�������ĸ�ʽ����
			//Ȼ��������ʽ�ַ���������
			vector<string> productionsSplit = splitProduction(production);
			for (string productionSplit : productionsSplit) {
				//������Բ���ʽ��������Բ���ʽ����
				if (isSelfProduction(productionSplit)) {
					selfProductions.push_back(productionSplit);
					continue;
				}
				pretreatedProductions.push_back(productionSplit);
			}
		}
	}


	//���ݲ���ʽ�ַ����������������
	static void createObjects() {
		for (string production : pretreatedProductions) {
			//��ȡ��
			string leftString = production.substr(0, 1);
			//����󲿷��ţ����ս��
			//NONTERMINATOR��ʾ���ս��
			Symbol left = Alphabet::addSymbol(leftString, Symbol::NONTERMINATOR);
			//��ȡ�Ҳ�
			production = production.substr(3);
			if (isEpsilon(production)) {
				grammar.addProduction(left, Production(left, { Alphabet::getEpsilonSymbol() }));
				continue;
			}
			
			vector<Symbol> right;//����ʽ���Ҳ��������������
			//�����Ҳ��ַ������ӷ���
			for (char c : production) {
				//���ݷ��ŵĲ�ͬ�����ս���ͷ��ս��
				bool isTerminator = false;//�����ս���ͷ��ս���ı�־
				if (c >= 'A' && c <= 'Z') {
					isTerminator = Symbol::NONTERMINATOR;
				}
				if (c >= 'a' && c <= 'z') {
					isTerminator = Symbol::TERMINATOR;
				}
				Symbol symbol = Alphabet::addSymbol(c, isTerminator);

				//�Ҳ�����һ���������Ҳ����ŵļ�����
				right.push_back(symbol);
			}

			Production p(left, right);
			grammar.addProduction(left, p);
		}
	}


	static void printAll() {
		cout << "\inputProductions\n";
		Util::printVectorInLines(inputProductions);
		cout << "\ntooShortProductions\n";
		Util::printVectorInLines(tooShortProductions);
		cout << "\nbeginningErrorProductions\n";
		Util::printVectorInLines(beginningErrorProductions);
		cout << "\nselfProductions\n";
		Util::printVectorInLines(selfProductions);
		cout << "\npretreatedProductions\n";
		Util::printVectorInLines(pretreatedProductions);
		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}


	//����Ƿ��п�ʼ���š�S��
	static void checkStartSymbol() {
		for (string s : pretreatedProductions) {
			//�п�ʼ���š�S�����Ϸ�
			if (s[0] == 'S') {
				return;
			}
		}
		throw exception();
	}


	//�Բ���ʽ�ַ�������ȥ��
	static void deleteDuplicate() {
		//��תΪset����ת�ص�vector
		set<string> productionsSet(pretreatedProductions.begin(), pretreatedProductions.end());
		vector<string> productionsVector(productionsSet.begin(), productionsSet.end());
		pretreatedProductions.clear();
		pretreatedProductions.insert(pretreatedProductions.end(), productionsVector.begin(), productionsVector.end());
	}


	//�ж��ַ�������ʽ���Ҳ��Ƿ����
	static bool isEpsilon(string right) {
		return Alphabet::isEpsilon(right);
	}


	//��ձ��������
	static void clearCache() {
		Alphabet::clear();

		inputProductions.clear();
		tooShortProductions.clear();
		beginningErrorProductions.clear();
		selfProductions.clear();
		pretreatedProductions.clear();
		grammar = Grammar();
	}


public:
	string productionSplitSymbol;//����ʽ�ķָ�������|��

	//get����
	static vector<string> getInputProductions() {
		return inputProductions;
	}
	static vector<string> getTooShortProductions() {
		return tooShortProductions;
	}
	static vector<string> getBeginningErrorProductions() {
		return beginningErrorProductions;
	}
	static vector<string> getSelfProductions() {
		return selfProductions;
	}
	static vector<string> getPretreatedProductions() {
		return pretreatedProductions;
	}
	static Grammar& getGrammar() {
		return grammar;
	}


	//set����
	//�����ķ��ļ���
	static void setGrammarFileName(string fileName) {
		grammarFileName = fileName;
	}


	//�ܵĺ�������
	static void pretreatGrammar() {
		clearCache();

		//�����ķ���txt�ļ�
		readGrammarFile();

		//�ķ���Ԥ����
		pretreatInputProductions();

		//����Ƿ��п�ʼ����S
		try {
			checkStartSymbol();
		}
		catch (const std::exception&) {
			cout << LACK_START_ERROR << endl;
			exit(1);
		}

		//����ʽȥ��
		deleteDuplicate();

		//�������������
		createObjects();

		//���
		printAll();
	}
};


//��̬��Ա������������ж��壬�����ڴ�
