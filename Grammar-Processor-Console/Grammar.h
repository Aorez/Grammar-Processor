#pragma once
#include<bits/stdc++.h>


#include"Production.h"
#include"Symbol.h"
#include"Util.h"
#include"FirstSet.h"
#include"FollowSet.h"
#include"SymbolsFirstSet.h"


#include"Alphabet.h"


using namespace std;


//文法的类
//每个功能类都创建一个对应的静态成员，这样就都有了一个map集合
class Grammar {
private:
	static Symbol start;//文法的开始符号

	static Symbol end;//文法的结束符号

	const static string DEFAULT_START_LABEL;//默认的文法开始符号

	const static string DEFAULT_END_LABEL;//默认的文法结束符号

	map<Symbol, vector<Production>> productionsMap;//一个左部对应一个产生式的集合

	map<Symbol, FirstSet> firstSetMap;//first集合

	map<Symbol, FollowSet> followSetMap;//follow集合


	//求解first集合
	void solveFirstSet() {
		//遍历所有符号
		for (auto symbol : Alphabet::getAllSymbol()) {
			//如果是非终结符，就要求first集合
			if (symbol.isNonTerminator()) {
				getOrCreateFirstSet(symbol);
			}
		}
	}
	//求解follow集合
	void solveFollowSet() {
		for (auto symbol : Alphabet::getAllSymbol()) {
			if (symbol.isNonTerminator()) {
				getOrCreateFollowSet(symbol);
			}
		}
	}
	
	
	//求解某个符号的first集合
	FirstSet getOrCreateFirstSet(Symbol symbolKey) {
		//如果已经求解过，直接返回
		if (firstSetMap.find(symbolKey) != firstSetMap.end()) {
			return firstSetMap[symbolKey];
		}

		FirstSet keyFirstSet(symbolKey);//保存first集合
		vector<Production> productionsHasKey;//保存右部中包含有本身符号的产生式，需要额外拿出来处理

		//遍历当前符号所有产生式
		for (auto keyProduction : productionsMap[symbolKey]) {
			//如果是空产生式，first集合加入空
			if (keyProduction.isEpsilon()) {
				keyFirstSet.setHasEpsilon(true);
				//继续下一个产生式
				continue;
			}

			//如果产生式右部中含有本身符号，需要额外处理
			if (keyProduction.rightHasLabel(symbolKey.getLabel())) {
				productionsHasKey.push_back(keyProduction);
			}

			//如果该产生式的右部中的所有符号都是非终结符
			//并且每个非终结符的first集合都有空字
			//则设置为true
			//初始化为true，在遇到终结符或者非终结符的first集合没有空字时
			//设为false
			bool keyHasEpsilon = true;
			//遍历当前产生式的右部的每个符号
			for (auto& keyProductionSymbol : keyProduction.getRight()) {
				//如果是终结符，直接添加到first集合，退出循环
				if (keyProductionSymbol.isNonTerminator() == false) {
					keyFirstSet.addSymbol(keyProductionSymbol);
					keyHasEpsilon = false;
					break;
				}

				//如果是非终结符，递归调用 getOrCreateFirstSetByKey 获取这个非终结符对应的串首终结符集。
				//但是不允许有间接左递归，即 A->B; B->Aa
				//这样会出现死循环，就无法求解了。
				FirstSet symbolFirstSet = getOrCreateFirstSet(keyProductionSymbol);
				keyFirstSet.addSymbolSet(symbolFirstSet);

				//如果递归获取到的first集合没有空字
				//所求的first集合就到此为止
				//同时不会有空字
				if (symbolFirstSet.hasEpsilon() == false) {
					keyHasEpsilon = false;
					break;
				}
			}
			if (keyHasEpsilon) {
				keyFirstSet.setHasEpsilon(keyHasEpsilon);
			}
		}

		//因为 First(key) 是否包括空串ε，是由不包含key的其他产生式决定的。
		//所以在遍历包含 key 的产生式的时候，就可以通过 First(key)是否包含空串ε，
		//来决定是否添加产生式中 key 后面的字符
		for (auto& productionHasKey : productionsHasKey) {
			//遍历当前产生式右部中的所有符号
			for (auto& productionSymbol : productionHasKey.getRight()) {
				//如果符号是终结符，就可以添加到first集合
				//然后退出循环了
				if (productionSymbol.isNonTerminator() ==false) {
					keyFirstSet.addSymbol(productionSymbol);
					break;
				}

				//如果符号和左部的符号一样，就根据上一次找first集合的结果
				//判断左部符号能否推出空字，不能就不用遍历后面的符号
				if (productionSymbol == symbolKey) {
					if (keyFirstSet.hasEpsilon() == false) {
						break;
					}

					//跳过当前和左部一样的符号
					continue;
				}

				FirstSet symbolFirstSet = getOrCreateFirstSet(productionSymbol);
				keyFirstSet.addSymbolSet(symbolFirstSet);

				//递归获取到的first集合没有空字
				//就可以退出循环
				if (symbolFirstSet.hasEpsilon() == false) {
					break;
				}
			}
		}

		//插入集合并返回
		firstSetMap[symbolKey] = keyFirstSet;
		return keyFirstSet;
	}
	//求解某个符号的follow集合
	FollowSet getOrCreateFollowSet(Symbol symbolToSolve) {
		//如果已经求解过，直接返回
		if (followSetMap.find(symbolToSolve) != followSetMap.end()) {
			return followSetMap[symbolToSolve];
		}

		FollowSet followSet(symbolToSolve);//最终的follow集合结果

		//如果是文法的开始符号，follow集合里要有文法结束符号
		if (symbolToSolve == start) {
			followSet.addSymbol(end);
		}

		//遍历每个非终结符及其产生式
		for (const auto& it : productionsMap) {
			//遍历每个产生式
			for (auto production : it.second) {
				//如果是空产生式，则产生式中不可能有函数参数的符号，即待求解follow集合的符号
				//所以不用处理
				if (production.isEpsilon()) {
					continue;
				}

				int symbolIndex = production.findSymbol(symbolToSolve);//S->……A……，A的下标
				//找不到待求解follow集合的符号，也不用处理
				if (symbolIndex == Production::SYMBOL_NOT_FOUND) {
					continue;
				}

				vector<Symbol> postfixSymbols = production.cutRight(symbolIndex + 1);//S->……A……，A右边的符号
				//获得符号串的first集合
				//如果符号串为空，函数会返回有空字的对象
				SymbolsFirstSet symbolsFirstSet = getSymbolsFirstSet(postfixSymbols);
				followSet.addSymbolSet(symbolsFirstSet);

				//如果有空字，要将左部的follow集合加入到当前的follow集合中
				if (symbolsFirstSet.hasEpsilon()) {
					//A->aA，A->aA……，这些情况不用添加
					if (symbolToSolve == production.getLeft()) {
					
					}
					else {
						followSet.addSymbolSet(getOrCreateFollowSet(production.getLeft()));
					}
				}
			}
		}

		followSetMap[symbolToSolve] = followSet;
		return followSet;
	}


	//获得文法符号串的first集合，即多个符号的first集合，类似一个产生式
	SymbolsFirstSet getSymbolsFirstSet(vector<Symbol> symbolKeys) {
		//如果文法符号串为空，返回一个hasEpsilon为true的对象
		//在求A的follow集合的时候，S->aA，后续符号为空，传参数过来，就可以返回空的对象
		if (symbolKeys.empty()) {
			return SymbolsFirstSet(Symbol(), set<Symbol>(), true);
		}

		SymbolsFirstSet keysFirstSet(symbolKeys);//保存最后的first集合

		bool hasEpsilon = true;//记录最后的first集合能否推出空字
		//遍历所有符号
		for (auto& key : symbolKeys) {
			//如果符号是终结符，插入first集合，并退出循环
			if (key.isNonTerminator() == false) {
				keysFirstSet.addSymbol(key);
				break;
			}

			//非终结符
			FirstSet keyFirstSet = firstSetMap[key];//获得first集合
			//插入
			keysFirstSet.addSymbolSet(keyFirstSet);

			if (keyFirstSet.hasEpsilon() == false) {
				hasEpsilon = false;
				break;
			}
		}

		keysFirstSet.setHasEpsilon(hasEpsilon);
		return keysFirstSet;
	}

public:
	const static string productionSplitSymbol;//产生式的分隔符，“|”

	//构造函数
	//默认构造函数
	Grammar() {
		productionsMap = map<Symbol, vector<Production>>();
		firstSetMap = map<Symbol, FirstSet>();
		followSetMap = map<Symbol, FollowSet>();
	}


	//get函数
	//返回产生式map集合
	map<Symbol, vector<Production>>& getProductionsMap() {
		return this->productionsMap;
	}
	//获得某个符号的产生式集合
	vector<Production> getProductions(Symbol symbol) {
		return Util::getMapValue(this->productionsMap, symbol);
	}
	//获得文法开始符号
	static Symbol getStart() {
		return start;
	}
	//获得first集合
	map<Symbol, FirstSet>& getFirstSetMap() {
		//如果集合是空的，求解first集合
		if (firstSetMap.empty()) {
			solveFirstSet();
		}
		//如果不空，直接返回，只求解一次
		return firstSetMap;
	}
	//获得follow集合
	map<Symbol, FollowSet>& getFollowSetMap() {
		//先求解first集合
		getFirstSetMap();
		if (followSetMap.empty()) {
			solveFollowSet();
		}
		return followSetMap;
	}



	//set函数
	//设置文法开始符号
	static void setStart(Symbol symbol) {
		start = symbol;
	}


	//添加产生式到某个非终结符的产生式集合中
	void addProduction(Symbol symbol, Production production) {
		//查找左部，如果查找不到，插入新的
		//如果已存在，在映射的值后添加
		if (productionsMap.find(symbol) == productionsMap.end()) {
			productionsMap[symbol] = vector<Production>({ production });
			return;
		}
		productionsMap[symbol].push_back(production);
	}
	//在某个非终结符的产生式集合后面追加一个产生式集合
	void addProductions(Symbol symbol, vector<Production> productions) {
		//查找左部，如果找不到，新建
		//如果已存在，在映射的值后添加
		if (productionsMap.find(symbol) == productionsMap.end()) {
			productionsMap[symbol] = productions;
			return;
		}
		productionsMap[symbol].insert(productionsMap[symbol].end(), productions.begin(), productions.end());
	}


	//输出
	//输出productionsMap
	void printProductionsMap() {
		Util::printMapValueInLines(productionsMap);
	}
	//输出first集合
	void printFirstSetMap() {
		getFirstSetMap();
		Util::printMap(firstSetMap);
	}
	//输出follow集合
	void printFollowSetMap() {
		getFollowSetMap();
		Util::printMap(followSetMap);
	}


	//判断产生式是否已在map集合中
	bool existProduction(Symbol symbol, Production production) {
		vector<Production> productions = getProductions(symbol);
		//遍历所有产生式，如果有相等的就返回true
		for (auto it : productions) {
			//这里用到了重载==
			if (production == it) {
				return true;
			}
		}
		return false;
	}


	//判断某个非终结符是否在左部中存在
	bool existLeft(Symbol nonterminator) {
		if (productionsMap.find(nonterminator) == productionsMap.end()) {
			return false;
		}
		return true;
	}


	//判断文法是否左线性
	bool isLeftLinear() {
		for (auto& it : productionsMap) {
			for (auto& production : it.second) {
				if (production.isLeftLinear() == false) {
					return false;
				}
			}
		}
		return true;
	}
	//判断文法是否右线性
	bool isRightLinear() {
		for (auto& it : productionsMap) {
			for (auto& production : it.second) {
				if (production.isRightLinear() == false) {
					return false;
				}
			}
		}
		return true;
	}
};


const string Grammar::DEFAULT_START_LABEL = "S";//默认的文法开始符号
const string Grammar::DEFAULT_END_LABEL = "#";//默认的文法结束符号
const string Grammar::productionSplitSymbol = "|";//产生式的分隔符，“|”
Symbol Grammar::end = Alphabet::addSymbol(Grammar::DEFAULT_END_LABEL, Symbol::NONTERMINATOR);//文法的结束符号
Symbol Grammar::start = Alphabet::addSymbol(Grammar::DEFAULT_START_LABEL, Symbol::NONTERMINATOR);//文法的开始符号
