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


//�ķ�����
//ÿ�������඼����һ����Ӧ�ľ�̬��Ա�������Ͷ�����һ��map����
class Grammar {
private:
	static Symbol start;//�ķ��Ŀ�ʼ����

	static Symbol end;//�ķ��Ľ�������

	const static string DEFAULT_START_LABEL;//Ĭ�ϵ��ķ���ʼ����

	const static string DEFAULT_END_LABEL;//Ĭ�ϵ��ķ���������

	map<Symbol, vector<Production>> productionsMap;//һ���󲿶�Ӧһ������ʽ�ļ���

	map<Symbol, FirstSet> firstSetMap;//first����

	map<Symbol, FollowSet> followSetMap;//follow����


	//���first����
	void solveFirstSet() {
		//�������з���
		for (auto symbol : Alphabet::getAllSymbol()) {
			//����Ƿ��ս������Ҫ��first����
			if (symbol.isNonTerminator()) {
				getOrCreateFirstSet(symbol);
			}
		}
	}
	//���follow����
	void solveFollowSet() {
		for (auto symbol : Alphabet::getAllSymbol()) {
			if (symbol.isNonTerminator()) {
				getOrCreateFollowSet(symbol);
			}
		}
	}
	
	
	//���ĳ�����ŵ�first����
	FirstSet getOrCreateFirstSet(Symbol symbolKey) {
		//����Ѿ�������ֱ�ӷ���
		if (firstSetMap.find(symbolKey) != firstSetMap.end()) {
			return firstSetMap[symbolKey];
		}

		FirstSet keyFirstSet(symbolKey);//����first����
		vector<Production> productionsHasKey;//�����Ҳ��а����б�����ŵĲ���ʽ����Ҫ�����ó�������

		//������ǰ�������в���ʽ
		for (auto keyProduction : productionsMap[symbolKey]) {
			//����ǿղ���ʽ��first���ϼ����
			if (keyProduction.isEpsilon()) {
				keyFirstSet.setHasEpsilon(true);
				//������һ������ʽ
				continue;
			}

			//�������ʽ�Ҳ��к��б�����ţ���Ҫ���⴦��
			if (keyProduction.rightHasLabel(symbolKey.getLabel())) {
				productionsHasKey.push_back(keyProduction);
			}

			//����ò���ʽ���Ҳ��е����з��Ŷ��Ƿ��ս��
			//����ÿ�����ս����first���϶��п���
			//������Ϊtrue
			//��ʼ��Ϊtrue���������ս�����߷��ս����first����û�п���ʱ
			//��Ϊfalse
			bool keyHasEpsilon = true;
			//������ǰ����ʽ���Ҳ���ÿ������
			for (auto& keyProductionSymbol : keyProduction.getRight()) {
				//������ս����ֱ����ӵ�first���ϣ��˳�ѭ��
				if (keyProductionSymbol.isNonTerminator() == false) {
					keyFirstSet.addSymbol(keyProductionSymbol);
					keyHasEpsilon = false;
					break;
				}

				//����Ƿ��ս�����ݹ���� getOrCreateFirstSetByKey ��ȡ������ս����Ӧ�Ĵ����ս������
				//���ǲ������м����ݹ飬�� A->B; B->Aa
				//�����������ѭ�������޷�����ˡ�
				FirstSet symbolFirstSet = getOrCreateFirstSet(keyProductionSymbol);
				keyFirstSet.addSymbolSet(symbolFirstSet);

				//����ݹ��ȡ����first����û�п���
				//�����first���Ͼ͵���Ϊֹ
				//ͬʱ�����п���
				if (symbolFirstSet.hasEpsilon() == false) {
					keyHasEpsilon = false;
					break;
				}
			}
			if (keyHasEpsilon) {
				keyFirstSet.setHasEpsilon(keyHasEpsilon);
			}
		}

		//��Ϊ First(key) �Ƿ�����մ��ţ����ɲ�����key����������ʽ�����ġ�
		//�����ڱ������� key �Ĳ���ʽ��ʱ�򣬾Ϳ���ͨ�� First(key)�Ƿ�����մ��ţ�
		//�������Ƿ���Ӳ���ʽ�� key ������ַ�
		for (auto& productionHasKey : productionsHasKey) {
			//������ǰ����ʽ�Ҳ��е����з���
			for (auto& productionSymbol : productionHasKey.getRight()) {
				//����������ս�����Ϳ�����ӵ�first����
				//Ȼ���˳�ѭ����
				if (productionSymbol.isNonTerminator() ==false) {
					keyFirstSet.addSymbol(productionSymbol);
					break;
				}

				//������ź��󲿵ķ���һ�����͸�����һ����first���ϵĽ��
				//�ж��󲿷����ܷ��Ƴ����֣����ܾͲ��ñ�������ķ���
				if (productionSymbol == symbolKey) {
					if (keyFirstSet.hasEpsilon() == false) {
						break;
					}

					//������ǰ����һ���ķ���
					continue;
				}

				FirstSet symbolFirstSet = getOrCreateFirstSet(productionSymbol);
				keyFirstSet.addSymbolSet(symbolFirstSet);

				//�ݹ��ȡ����first����û�п���
				//�Ϳ����˳�ѭ��
				if (symbolFirstSet.hasEpsilon() == false) {
					break;
				}
			}
		}

		//���뼯�ϲ�����
		firstSetMap[symbolKey] = keyFirstSet;
		return keyFirstSet;
	}
	//���ĳ�����ŵ�follow����
	FollowSet getOrCreateFollowSet(Symbol symbolToSolve) {
		//����Ѿ�������ֱ�ӷ���
		if (followSetMap.find(symbolToSolve) != followSetMap.end()) {
			return followSetMap[symbolToSolve];
		}

		FollowSet followSet(symbolToSolve);//���յ�follow���Ͻ��

		//������ķ��Ŀ�ʼ���ţ�follow������Ҫ���ķ���������
		if (symbolToSolve == start) {
			followSet.addSymbol(end);
		}

		//����ÿ�����ս���������ʽ
		for (const auto& it : productionsMap) {
			//����ÿ������ʽ
			for (auto production : it.second) {
				//����ǿղ���ʽ�������ʽ�в������к��������ķ��ţ��������follow���ϵķ���
				//���Բ��ô���
				if (production.isEpsilon()) {
					continue;
				}

				int symbolIndex = production.findSymbol(symbolToSolve);//S->����A������A���±�
				//�Ҳ��������follow���ϵķ��ţ�Ҳ���ô���
				if (symbolIndex == Production::SYMBOL_NOT_FOUND) {
					continue;
				}

				vector<Symbol> postfixSymbols = production.cutRight(symbolIndex + 1);//S->����A������A�ұߵķ���
				//��÷��Ŵ���first����
				//������Ŵ�Ϊ�գ������᷵���п��ֵĶ���
				SymbolsFirstSet symbolsFirstSet = getSymbolsFirstSet(postfixSymbols);
				followSet.addSymbolSet(symbolsFirstSet);

				//����п��֣�Ҫ���󲿵�follow���ϼ��뵽��ǰ��follow������
				if (symbolsFirstSet.hasEpsilon()) {
					//A->aA��A->aA��������Щ����������
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


	//����ķ����Ŵ���first���ϣ���������ŵ�first���ϣ�����һ������ʽ
	SymbolsFirstSet getSymbolsFirstSet(vector<Symbol> symbolKeys) {
		//����ķ����Ŵ�Ϊ�գ�����һ��hasEpsilonΪtrue�Ķ���
		//����A��follow���ϵ�ʱ��S->aA����������Ϊ�գ��������������Ϳ��Է��ؿյĶ���
		if (symbolKeys.empty()) {
			return SymbolsFirstSet(Symbol(), set<Symbol>(), true);
		}

		SymbolsFirstSet keysFirstSet(symbolKeys);//��������first����

		bool hasEpsilon = true;//��¼����first�����ܷ��Ƴ�����
		//�������з���
		for (auto& key : symbolKeys) {
			//����������ս��������first���ϣ����˳�ѭ��
			if (key.isNonTerminator() == false) {
				keysFirstSet.addSymbol(key);
				break;
			}

			//���ս��
			FirstSet keyFirstSet = firstSetMap[key];//���first����
			//����
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
	const static string productionSplitSymbol;//����ʽ�ķָ�������|��

	//���캯��
	//Ĭ�Ϲ��캯��
	Grammar() {
		productionsMap = map<Symbol, vector<Production>>();
		firstSetMap = map<Symbol, FirstSet>();
		followSetMap = map<Symbol, FollowSet>();
	}


	//get����
	//���ز���ʽmap����
	map<Symbol, vector<Production>>& getProductionsMap() {
		return this->productionsMap;
	}
	//���ĳ�����ŵĲ���ʽ����
	vector<Production> getProductions(Symbol symbol) {
		return Util::getMapValue(this->productionsMap, symbol);
	}
	//����ķ���ʼ����
	static Symbol getStart() {
		return start;
	}
	//���first����
	map<Symbol, FirstSet>& getFirstSetMap() {
		//��������ǿյģ����first����
		if (firstSetMap.empty()) {
			solveFirstSet();
		}
		//������գ�ֱ�ӷ��أ�ֻ���һ��
		return firstSetMap;
	}
	//���follow����
	map<Symbol, FollowSet>& getFollowSetMap() {
		//�����first����
		getFirstSetMap();
		if (followSetMap.empty()) {
			solveFollowSet();
		}
		return followSetMap;
	}



	//set����
	//�����ķ���ʼ����
	static void setStart(Symbol symbol) {
		start = symbol;
	}


	//��Ӳ���ʽ��ĳ�����ս���Ĳ���ʽ������
	void addProduction(Symbol symbol, Production production) {
		//�����󲿣�������Ҳ����������µ�
		//����Ѵ��ڣ���ӳ���ֵ�����
		if (productionsMap.find(symbol) == productionsMap.end()) {
			productionsMap[symbol] = vector<Production>({ production });
			return;
		}
		productionsMap[symbol].push_back(production);
	}
	//��ĳ�����ս���Ĳ���ʽ���Ϻ���׷��һ������ʽ����
	void addProductions(Symbol symbol, vector<Production> productions) {
		//�����󲿣�����Ҳ������½�
		//����Ѵ��ڣ���ӳ���ֵ�����
		if (productionsMap.find(symbol) == productionsMap.end()) {
			productionsMap[symbol] = productions;
			return;
		}
		productionsMap[symbol].insert(productionsMap[symbol].end(), productions.begin(), productions.end());
	}


	//���
	//���productionsMap
	void printProductionsMap() {
		Util::printMapValueInLines(productionsMap);
	}
	//���first����
	void printFirstSetMap() {
		getFirstSetMap();
		Util::printMap(firstSetMap);
	}
	//���follow����
	void printFollowSetMap() {
		getFollowSetMap();
		Util::printMap(followSetMap);
	}


	//�жϲ���ʽ�Ƿ�����map������
	bool existProduction(Symbol symbol, Production production) {
		vector<Production> productions = getProductions(symbol);
		//�������в���ʽ���������ȵľͷ���true
		for (auto it : productions) {
			//�����õ�������==
			if (production == it) {
				return true;
			}
		}
		return false;
	}


	//�ж�ĳ�����ս���Ƿ������д���
	bool existLeft(Symbol nonterminator) {
		if (productionsMap.find(nonterminator) == productionsMap.end()) {
			return false;
		}
		return true;
	}


	//�ж��ķ��Ƿ�������
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
	//�ж��ķ��Ƿ�������
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


const string Grammar::DEFAULT_START_LABEL = "S";//Ĭ�ϵ��ķ���ʼ����
const string Grammar::DEFAULT_END_LABEL = "#";//Ĭ�ϵ��ķ���������
const string Grammar::productionSplitSymbol = "|";//����ʽ�ķָ�������|��
Symbol Grammar::end = Alphabet::addSymbol(Grammar::DEFAULT_END_LABEL, Symbol::NONTERMINATOR);//�ķ��Ľ�������
Symbol Grammar::start = Alphabet::addSymbol(Grammar::DEFAULT_START_LABEL, Symbol::NONTERMINATOR);//�ķ��Ŀ�ʼ����
