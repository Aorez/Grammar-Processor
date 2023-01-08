#pragma once
#include<bits/stdc++.h>


#include"Production.h"
#include"Grammar.h"


#include"Util.h"
#include"GrammarPretreatment.h"


using namespace std;


//产生式是否不可终止的检查的静态类
static class TerminationFilter {
private:
	static vector<Production> interminableProductions;//保存不可终止的产生式

	static vector<string> interminableLabels;//保存不可终止的左部的符号

	static map<Symbol, pair<int, int>> statusMap;//检查是否不可终止时，记录检查到了哪一个右部的哪一个符号，或者其状态信息

	static Grammar& previousGrammar;//上一个功能进行后的文法

	static Grammar grammar;//删除了不可终止后的文法

	//初始化statusMap状态集合
	static void initializeStatusMap() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//全部符号的vector
		for (auto& it : allSymbol) {
			//如果是非终结符，判断是否在左部中存在，如果没有，直接设置不可终止
			if (it.isNonTerminator()) {
				//如果在左部中存在
				//再检查是否有全部由终结符组成的产生式
				if (previousGrammar.existLeft(it)) {

					//该符号的产生式中有全部由终结符组成的产生式，可终止
					if (hasProductionAllMadeOfTerminators(it)) {
						Util::addInitialValueToMap(statusMap, it, TERMINABILITY);
					}
					else {
						//设置未检查
						Util::addInitialValueToMap(statusMap, it, TERMINATION_UNCHECKED);
					}
				}
				else {
					//在左部中不存在
					//设置不可终止
					Util::addInitialValueToMap(statusMap, it, INTERMINABILITY);

					//还要放入不可终止的符号的集合
					//因为后面删除产生式的时候才放入该集合，但是这些符号没有产生式
					interminableLabels.push_back(it.getLabel());
				}
			}
		}
	}


	//检查不可终止的产生式
	static void checkTermination() {
		//只扫描一遍所有产生式是不够的，因此要循环
		//用change记录是否有不可终止和可终止的改变
		//如果有改变，就应该再扫描一遍
		bool change = true;
		map<Symbol, vector<Production>>& productionsMap = previousGrammar.getProductionsMap();//获取所有产生式的集合
		while (true) {
			if (change) {
				change = false;
			}
			else {
				break;
			}

			//遍历所有产生式
			for (map<Symbol, vector<Production>>::iterator it = productionsMap.begin(); it != productionsMap.end(); it++) {
				Symbol left = it->first;//当前的左部
				pair<int, int>& status = statusMap[left];//检查不可终止时，当前左部检查到了哪一个右部的哪一个符号
				vector<Production>& productions = it->second;//当前左部的所有产生式

				//TERMINATION_UNCHECKED表示当前左部还未被检查过
				if (status == TERMINATION_UNCHECKED) {
					status = make_pair(0, 0);
				}

				//TERMINABILITY表示已经确定可终止，INTERMINABILITY表示不可终止，如果已经确定了，则继续下一个左部的检查
				if (status == TERMINABILITY || status == INTERMINABILITY) {
					continue;
				}

				while (true) {
					//如果右部的第一个下标超出，则可以退出循环，即当前所有右部已经遍历完
					if (status.first >= productions.size()) {

						//到了这里，其实默认就是不可终止了
						//因为只有找不到可终止的才会遍历完全部右部
						//如果找到了可终止的某一个右部，但是这个右部的符号遍历完了，也不会来到这个if条件中
						//而是在下面的catch块中
						//又，遇到不可终止，first下标才会加加，遇到可终止，second下标才会加加
						status = INTERMINABILITY;
						change = true;
						break;
					}

					//这里第二个方括号使用了重载运算符
					//第二个下标不合法，例如超出，会抛出异常
					try {
						Symbol symbol = productions[status.first][status.second];//当前的右部的当前符号

						//如果当前的右部的当前的符号是终结符，那么就要继续扫描后面的符号
						if (symbol.isNonTerminator() == false) {
							//第二个下标增加
							status.second++;
							continue;
						}

						//如果当前的右部的当前符号是非终结符，那么就要看看这个非终结符是否可终止
						if (symbol.isNonTerminator()) {

							//“A->……A……”
							//处理方法是进行后面的右部的判断，跳过当前右部
							if (symbol.getLabel() == left.getLabel()) {
								status.first++;
								status.second = 0;
								continue;
							}

							pair<int, int> symbolStatus = statusMap[symbol];//获得当前右部的当前符号的pair的两个下标

							//如果当前右部的当前符号已经检查过，并且是可终止的
							//则第二个下标自增，继续检查当前右部的下一个符号
							if (symbolStatus == TERMINABILITY) {
								status.second++;
								continue;
							}

							//如果当前右部的当前符号已经检查过，而且是不可终止的
							//要把这个右部删掉，但是这里先不删，因为可能会删不干净，比如“A->a|C”，但是C不可终止，这个时候是删不干净的
							//继续判断下一个右部
							if (symbolStatus == INTERMINABILITY) {
								status.first++;
								status.second = 0;
								continue;
							}

							//如果当前右部的当前符号还未被检查，则要保持当前左部的pair的两个下标
							//退出循环，继续检查下一个左部
							if (symbolStatus == TERMINATION_UNCHECKED) {
								break;
							}
							//其他情况说明当前右部的当前符号还在检查中，也要退出循环
							break;

						}
					}
					catch (const std::exception&) {
						//下标超出，说明已经遍历完某一个右部
						//其实意味着这某一个右部是可终止的，才会遍历完，所以当前左部可终止
						status = TERMINABILITY;
						change = true;
						break;
					}
				}
			}
		}

	}


	//检查完不可终止的产生式之后，再执行删除的操作
	static void deleteInterminabilities() {
		//前面的循环对pair进行设置之后
		//就可以开始删除不可终止的产生式了
		//先删除左部是不可终止的
		map<Symbol, vector<Production>>& productionsMap = previousGrammar.getProductionsMap();//获取所有产生式的集合
		for (auto& it : productionsMap) {
			Symbol left = it.first;//当前的左部

			//如果是可终止的左部，添加到新文法中
			if (statusMap[left] == TERMINABILITY) {
				grammar.addProductions(left, it.second);
			}
			//其他情况都是不可终止
			//不添加
			else {
				//将不可终止的产生式和左部符号都保存下来
				interminableLabels.push_back(left.getLabel());
				interminableProductions.insert(interminableProductions.end(), it.second.begin(), it.second.end());
			}
		}

		//再删除右部中出现的、不可终止的左部符号
		for (map<Symbol, vector<Production>>::iterator it = grammar.getProductionsMap().begin(); it != grammar.getProductionsMap().end(); it++) {
			vector<Production>& productions = it->second;//当前左部的所有右部

			//遍历所有右部，看看是否有不可终止的左部符号
			for (vector<Production>::iterator it2 = productions.begin(); it2 != productions.end();) {
				bool hasInterminability = false;//记录右部是否有不可终止的符号的出现

				//遍历所有不可终止的符号，一个一个进行判断
				for (string interminableLabel : interminableLabels) {
					//调用函数进行右部中是否有某个符号的判断
					hasInterminability = it2->rightHasLabel(interminableLabel);
					if (hasInterminability) {
						break;
					}
				}

				//如果有不可终止的符号，这个产生式应该删除
				if (hasInterminability) {
					interminableProductions.push_back(*it2);
					it2 = productions.erase(it2);
				}
				else {
					it2++;
				}
			}
		}

		//将字母表中的不可终止的符号删除
		for (auto& label : interminableLabels) {
			Alphabet::removeSymbol(label);
		}
	}


	//遍历某个符号的所有产生式，查找只有终结符的产生式
	//用于提前判断是否可终止
	static bool hasProductionAllMadeOfTerminators(Symbol left) {
		for (auto& leftProduction : previousGrammar.getProductions(left)) {
			//如果有一个产生式全由终结符组成
			//直接返回
			if (leftProduction.isAllMadeOfTerminators()) {
				return true;
			}
		}
		return false;
	}

public:
	const static pair<int, int> TERMINATION_UNCHECKED;//表示该左部还没有被检查是否不可终止

	const static pair<int, int> TERMINABILITY;//表示该左部被检查完了，而且是可以终止的状态

	const static pair<int, int> INTERMINABILITY;//表示该左部被检查完了，是不可终止的


	//get函数
	//获得文法
	static Grammar& getGrammar() {
		return grammar;
	}


	//总的函数调用
	static void filterTermination() {
		//初始化每个符号的pair映射
		initializeStatusMap();

		//检查不可终止
		checkTermination();

		//删除不可终止
		deleteInterminabilities();

		//输出
		cout << "\ninterminableProductions\n";
		Util::printVectorInLines(interminableProductions);
		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}
};


vector<Production> TerminationFilter::interminableProductions;//保存不可终止的产生式
vector<string> TerminationFilter::interminableLabels;//保存不可终止的左部的符号
map<Symbol, pair<int, int>> TerminationFilter::statusMap;//检查是否不可终止时，记录检查到了哪一个右部的哪一个符号，或者其状态信息
const pair<int, int> TerminationFilter::TERMINATION_UNCHECKED(-1, -1);//表示该左部还没有被检查是否不可终止
const pair<int, int> TerminationFilter::TERMINABILITY(-2, -2);//表示该左部被检查完了，而且是可以终止的状态
const pair<int, int> TerminationFilter::INTERMINABILITY(-3, -3);//表示该左部被检查完了，是不可终止的
Grammar& TerminationFilter::previousGrammar = GrammarPretreatment::getGrammar();//上一个功能进行后的文法
Grammar TerminationFilter::grammar;//删除了不可终止后的文法
