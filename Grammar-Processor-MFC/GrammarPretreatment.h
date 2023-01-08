#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"
#include"Grammar.h"
#include"Production.h"


#include"Alphabet.h"


using namespace std;


//文法预处理的静态类
static class GrammarPretreatment {
private:
	static vector<string> inputProductions;//从文件中读入的产生式

	static vector<string> tooShortProductions;//产生式太短，产生式长度要求大于3

	static vector<string> beginningErrorProductions;//产生式开头格式错误，产生式开头格式应类似“A->”

	static vector<string> selfProductions;//自产生式的集合，即“A->A”这样的字符串

	static vector<string> pretreatedProductions;//预处理后的产生式输入集合

	static string grammarFileName;//文法文件的文件名

	static Grammar grammar;//预处理后的文法

	const static string LACK_START_ERROR;//缺少开始符号后的出错提示


	//将产生式按“|”进行分割
	static vector<string> splitProduction(string productionToSplit) {
		string left = productionToSplit.substr(0, 3);//保存产生式的左部，“A->”
		productionToSplit = productionToSplit.substr(3);//所有右部，“b|c|d”
		vector<string> rightsSplit = Util::splitString(productionToSplit, Grammar::productionSplitSymbol);//分割后的所有右部
		vector<string> productionsSplit;
		//遍历所有右部，与左部相加后，返回集合
		for (auto right : rightsSplit) {
			productionsSplit.push_back(left + right);
		}
		return productionsSplit;
	}


	//判断是否自产生式
	static bool isSelfProduction(string production) {
		//自产生式格式“A->A”
		if (production.size() == 4 && production[0] == production[3]) {
			return true;
		}
		return false;
	}


	//从文件中读入文法规则
	static void readGrammarFile() {
		ifstream ifs;
		ifs.open(grammarFileName, ios::in);
		string fileLine;//从文件中读取出的一行
		while (!ifs.eof()) {
			getline(ifs, fileLine);
			//如果这一行为空
			if (fileLine.empty()) {
				continue;
			}
			inputProductions.push_back(fileLine);
		}
	}


	//预处理从文件中读入的文法规则，将残缺的产生式、不合法的符号删去……
	static void pretreatInputProductions() {
		//读取每一个产生式字符串
		for (string production : inputProductions) {
			//删除产生式中的空格
			production = Util::filterCharacter(production, { ' ' }, Util::NOT_RETAIN);

			//产生式太短，产生式要求长度大于3
			if (production.size() <= 3) {
				tooShortProductions.push_back(production);
				continue;
			}

			//注释
			if (production[0] == '/' && production[1] == '/') {
				continue;
			}

			//产生式的开头出错，产生式的开头应该类似“A->”，并且第一位规定为大写字母
			if (production[0] < 'A' || production[0] > 'Z' || production[1] != '-' || production[2] != '>') {
				beginningErrorProductions.push_back(production);
				continue;
			}

			//过滤不是大写字母小写字母“|”“@”的字符
			vector<char> characters({ '|','@' });
			for (char c = 'A'; c <= 'Z'; c++) {
				characters.push_back(c);
			}
			for (char c = 'a'; c <= 'z'; c++) {
				characters.push_back(c);
			}
			//产生式的开头需要截取掉，再进行过滤
			production = production.substr(0, 3) + Util::filterCharacter(production.substr(3), characters, Util::RETAIN);

			//将产生式按“|”进行分割
			//并判断是否自产生式，只需要判断是否“A->A”这样的格式即可
			//然后放入产生式字符串集合中
			vector<string> productionsSplit = splitProduction(production);
			for (string productionSplit : productionsSplit) {
				//如果是自产生式，则放入自产生式集合
				if (isSelfProduction(productionSplit)) {
					selfProductions.push_back(productionSplit);
					continue;
				}
				pretreatedProductions.push_back(productionSplit);
			}
		}
	}


	//根据产生式字符串创建各种类对象
	static void createObjects() {
		for (string production : pretreatedProductions) {
			//提取左部
			string leftString = production.substr(0, 1);
			//添加左部符号，非终结符
			//NONTERMINATOR表示非终结符
			Symbol left = Alphabet::addSymbol(leftString, Symbol::NONTERMINATOR);
			//截取右部
			production = production.substr(3);
			if (isEpsilon(production)) {
				grammar.addProduction(left, Production(left, { Alphabet::getEpsilonSymbol() }));
				continue;
			}
			
			vector<Symbol> right;//产生式的右部，由许多符号组成
			//遍历右部字符，增加符号
			for (char c : production) {
				//根据符号的不同创建终结符和非终结符
				bool isTerminator = false;//创建终结符和非终结符的标志
				if (c >= 'A' && c <= 'Z') {
					isTerminator = Symbol::NONTERMINATOR;
				}
				if (c >= 'a' && c <= 'z') {
					isTerminator = Symbol::TERMINATOR;
				}
				Symbol symbol = Alphabet::addSymbol(c, isTerminator);

				//右部符号一个个放入右部符号的集合中
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


	//检查是否有开始符号“S”
	static void checkStartSymbol() {
		for (string s : pretreatedProductions) {
			//有开始符号“S”，合法
			if (s[0] == 'S') {
				return;
			}
		}
		throw exception();
	}


	//对产生式字符串进行去重
	static void deleteDuplicate() {
		//先转为set，再转回到vector
		set<string> productionsSet(pretreatedProductions.begin(), pretreatedProductions.end());
		vector<string> productionsVector(productionsSet.begin(), productionsSet.end());
		pretreatedProductions.clear();
		pretreatedProductions.insert(pretreatedProductions.end(), productionsVector.begin(), productionsVector.end());
	}


	//判断字符串产生式的右部是否空字
	static bool isEpsilon(string right) {
		return Alphabet::isEpsilon(right);
	}


	//清空保存的内容
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
	string productionSplitSymbol;//产生式的分隔符，“|”

	//get函数
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


	//set函数
	//设置文法文件名
	static void setGrammarFileName(string fileName) {
		grammarFileName = fileName;
	}


	//总的函数调用
	static void pretreatGrammar() {
		clearCache();

		//读入文法的txt文件
		readGrammarFile();

		//文法的预处理
		pretreatInputProductions();

		//检查是否有开始符号S
		try {
			checkStartSymbol();
		}
		catch (const std::exception&) {
			cout << LACK_START_ERROR << endl;
			exit(1);
		}

		//产生式去重
		deleteDuplicate();

		//创建各种类对象
		createObjects();

		//输出
		printAll();
	}
};


//静态成员必须在类外进行定义，分配内存
