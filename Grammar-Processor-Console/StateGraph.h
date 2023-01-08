#pragma once
#include<bits/stdc++.h>


#include"Grammar.h"
#include"State.h"
#include"Util.h"


using namespace std;


//状态图的类
class StateGraph {
private:
	Grammar& grammar;//状态图对应的文法

	vector<State*> DFAMinimized;//划分名=>状态

	vector<vector<State*>> DFADivision;//DFA最小化的划分
	
	map<set<Symbol>, State*> DFA;//保存DFA

	vector<State*> DFAStates;//DFA

	map<Symbol, State*> NFA;//状态名=>状态，单个非终结符=>状态

	map<State*, set<State*>> epsilonClosure;//epsilon闭包

	string additionalLabel;//增加额外的符号，在左线性中表示初态，在右线性中表示终态，默认为#

	bool grammarLinear;//文法的线性规则，左或右

	static enum class Selection
	{
		NFA,
		DFA,
	};
	
	
	//添加NFA状态，如果已存在，则不添加
	//单个非终结符
	State* addNFAState(Symbol nonterminator, StateStatus status = StateStatus::common) {
		if (NFA.find(nonterminator) == NFA.end()) {
			NFA[nonterminator] = new State(nonterminator, status);
		}
		return NFA[nonterminator];
	}


	//生成状态图，左线性或者右线性
	void generateNFA(bool epsilonTwoWay = false) {
		Selection select = Selection::NFA;
		map<bool, StateStatus> pairStatus;
		pairStatus[LEFT_LINEAR] = StateStatus::end;
		pairStatus[RIGHT_LINEAR] = StateStatus::start;

		//插入开始符号的状态
		//左线性开始符号为终态，右线性开始符号为初态
		addNFAState(grammar.getStart(), pairStatus[grammarLinear]);
		//左线性需要额外的初态，右线性需要额外的终态
		State* additionalState = addNFAState(Alphabet::getSymbol(additionalLabel), pairStatus[!grammarLinear]);

		//遍历所有左部及其产生式
		for (auto it : grammar.getProductionsMap()) {
			//将左部的符号创建成为普通态
			State* leftState = addNFAState(it.first);

			for (auto production : it.second) {
				//如果函数参数epsilonTwoWay为true，则将epsilon有向边设置为双向的
				if (epsilonTwoWay && production.isEpsilon()) {
					additionalState->addTransition(Alphabet::getEpsilonSymbol(), leftState);
					leftState->addTransition(Alphabet::getEpsilonSymbol(), additionalState);
					continue;
				}

				switch (production.getRightSize())
				{
					//空产生式
					//左线性，额外的状态（初态）经过epsilon到达左部符号的状态
					//右线性，左部符号经过epsilon到达额外的状态（终态）
				case 0: {
					//前面已经定义过了，一直用同一个就好了
					//设置状态转换
					if (grammarLinear == LEFT_LINEAR) {
						additionalState->addTransition(Alphabet::getEpsilonSymbol(), leftState);
					}
					else {
						leftState->addTransition(Alphabet::getEpsilonSymbol(), additionalState);
					}
					break;
				}
					//有一个符号，可能是终结符，或者epsilon
				case 1: {
					if (grammarLinear == LEFT_LINEAR) {
						additionalState->addTransition(production[0], leftState);
					}
					else {
						leftState->addTransition(production[0], additionalState);
					}
					break;
				}
					  //两个符号A->aB，构建A经过a到达状态B的线路
				case 2: {
					//左线性A->Ba，创建B的状态
					//右线性A->aB
					State* rightState = addNFAState(production[grammarLinear]);
					if (grammarLinear == LEFT_LINEAR) {
						rightState->addTransition(production[!grammarLinear], leftState);
					}
					else {
						leftState->addTransition(production[!grammarLinear], rightState);
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}


	//求解epsilon闭包
	void solveEpsilonClosure() {
		//遍历每个NFA状态，对每个状态求epsilon闭包
		for (auto it : NFA) {
			State* NFAState = it.second;//NFA状态

			//广度优先搜索
			queue<State*> stateStack;//BFS栈
			stateStack.push(NFAState);

			while (!stateStack.empty()) {
				State* state = stateStack.front();//弹出栈顶
				stateStack.pop();
				//把自己加到epsilon闭包中
				epsilonClosure[NFAState].insert(state);

				//获得经过epsilon后的状态集
				auto states = state->getTransition(Alphabet::getEpsilonSymbol());
				//将状态集加到epsilon闭包中
				epsilonClosure[NFAState].insert(states.begin(), states.end());
				//加入栈中
				for (State* p : states) {
					stateStack.push(p);
				}
			}
		}
	}


	//将同一个划分中的DFA状态组合成一个
	//同一个划分中的DFA状态的转换是一样的
	//状态转换
	//划分名 经过 终结符 到达 划分名
	//同一个划分里，如果有终态，就都是终态
	//如果有初态，就将这个划分作为初态
	//由于没办法确定划分名所对应的新的最小化后的DFA状态
	//必须全部创建出来才能确定，因此不设置转换
	//转换设置为空
	void combineDFAStates(vector<State*> states) {
		StateStatus status = StateStatus::common;
		set<Symbol> nonterminators;//非终结符的集合

		for (auto state : states) {
			//赋予初态或终态
			if (state->getStatus() == StateStatus::start) {
				status = StateStatus::start;
			}
			else if (state->getStatus() == StateStatus::end) {
				status = StateStatus::end;
			}

			//将每个状态的非终结符放到一起
			auto subNonterminators = state->getNonterminators();
			nonterminators.insert(subNonterminators.begin(), subNonterminators.end());
		}

		//先创建状态，然后再输入转换
		State* newState = new State(nonterminators, status);
		//设置划分名
		int divisionId = (*states.begin())->getDivisionId();
		newState->setDivisionId(divisionId);

		//因为是按顺序创建，所以直接push_back
		DFAMinimized.push_back(newState);
		//DFAMinimized[divisionId] = newState;
	}


	//NFA转DFA
	void NFAToDFA() {
		solveEpsilonClosure();
		printEpsilonClosure();

		//获得DFA初态
		State* DFAStart = getDFAStart();

		queue<State*> stateStack;
		stateStack.push(DFAStart);
		while (!stateStack.empty()) {
			State* DFAState = stateStack.front();
			stateStack.pop();
			
			//遍历每个终结符，状态可能经过也可能不经过
			for (auto terminator : Alphabet::getAllTerminator()) {
				//跳过epsilon
				if (terminator == Alphabet::getEpsilonSymbol()) {
					continue;
				}

				//保存DFA状态可以到达的非终结符，便于之后创建新状态
				set<Symbol> nonterminators;

				//遍历DFA状态中的非终极符
				for (auto nonterminator : DFAState->getNonterminators()) {

					//非终结符对应的NFA状态
					State* NFAState = NFA[nonterminator];
					//获得该状态经过终结符所到达的NFA状态集合，便于提取其中的非终结符
					//然后存入总的非终结符集合
					//nonterminators
					auto NFAStates = NFAState->getTransition(terminator);
					//如果集合是空的，说明没有经过这个终结符
					if (NFAStates.empty()) {
						continue;
					}

					//遍历各个状态，将它的闭包中的状态加入到同一个集合
					auto NFAStatesTemp = NFAStates;
					for (auto p : NFAStatesTemp) {
						auto statesTemp = epsilonClosure[p];
						NFAStates.insert(statesTemp.begin(), statesTemp.end());
					}

					//遍历加上闭包中的状态的所有状态，取得非终结符
					for (auto p : NFAStates) {
						auto subNonterminators = p->getNonterminators();
						nonterminators.insert(subNonterminators.begin(), subNonterminators.end());
						//nonterminators.insert(p->getNonterminators().begin(), p->getNonterminators().end());
					}
				}

				//当前状态中的非终极符都不经过该终结符
				if (nonterminators.empty()) {
					continue;
				}

				//创建新状态，如果该状态已存在，则不用入栈
				//普通状态
				//状态名为map集合size，自动增加
				if (DFA.find(nonterminators) == DFA.end()) {
					State* newState = new State(nonterminators, DFAStates.size());
					DFA[nonterminators] = newState;
					DFAStates.push_back(newState);
					stateStack.push(newState);
				}

				//加入状态转换
				//当前DFA状态 经过终结符 到达新状态
				DFAState->addTransition(terminator, DFA[nonterminators]);
			}
		}

		//设置DFA的终态
		setDFAEnd();
	}


	//获得NFA初态
	State* getNFAStart() {
		//左线性的初态是#对应的状态
		//右线性的初态是开始符号对应的状态
		if (grammarLinear == LEFT_LINEAR) {
			return NFA[Alphabet::getSymbol(additionalLabel)];
		}
		if (grammarLinear == RIGHT_LINEAR) {
			return NFA[grammar.getStart()];
		}
	}
	//获得DFA初态
	//如果是新建的
	//只有非终结符，没有状态转换
	State* getDFAStart() {
		//获得NFA初态
		State* NFAStart = getNFAStart();
		//获得闭包
		auto epsilon = epsilonClosure[NFAStart];
		set<Symbol> nonterminators;//非终结符，方便DFA初态的创建
		//遍历闭包中的状态
		for (auto NFAState : epsilon) {
			//取出状态的非终结符
			for (auto nonterminator : NFAState->getNonterminators()) {
				//插入到集合中
				nonterminators.insert(nonterminator);
			}
		}
		if (DFA.find(nonterminators) == DFA.end()) {
			State* newState = new State(nonterminators, DFAStates.size(), StateStatus::start);
			DFA[nonterminators] = newState;
			DFAStates.push_back(newState);
		}
		return DFA[nonterminators];
	}


	//DFA最小化
	void minimizeDFA() {
		//初始划分，划分为终态和非终态
		//插入两个空集合
		DFADivision.push_back(vector<State*>());
		DFADivision.push_back(vector<State*>());
		for (auto state : DFAStates) {
			if (state->getStatus() == StateStatus::end) {
				state->setDivisionId(1);
				DFADivision[1].push_back(state);
			}
			else {
				state->setDivisionId(0);
				DFADivision[0].push_back(state);
			}
		}

		bool hasNewDivision = true;//如果有新的划分产生，就要继续划分，直到没有新的划分
		while (hasNewDivision) {
			hasNewDivision = false;

			for (auto division = DFADivision.begin(); division != DFADivision.end(); division++) {
				//如果划分的集合里只有一个，那就不用划分
				//在入栈前也有判断了
				if (division->size() <= 1) {
					continue;
				}

				//划分之前先插入一个新集合，如果新集合为空，之后再删掉
				//在这里push_back，迭代器会变成空
				//DFADivision.push_back(vector<State*>());
				//所以新建vector代表新的划分
				vector<State*> newDivision;

				//将划分中的第一个作为划分的代表
				State* first = (*division)[0];

				//遍历每个终结符
				for (auto terminator : Alphabet::getAllTerminator()) {
					//DFA中没有epsilon
					if (terminator == Alphabet::getEpsilonSymbol()) {
						continue;
					}

					//如果删除到只剩下一个划分了，就不用再循环了
					if (division->size() <= 1) {
						break;
					}

					//获得转换后的下一个状态，因为DFA转换后的状态是确定的
					//所以只有一个，只需要获取集合中的第一个
					//也有可能为空
					set<State*> firstNextStates = first->getTransition(terminator);
					State* firstNextState = nullptr;
					if (!firstNextStates.empty()) {
						firstNextState = *(firstNextStates.begin());
					}

					//从划分中的下一个开始遍历
					for (auto it = division->begin() + 1; it != division->end();) {
						//获得下一个状态的集合，有可能是空集合
						auto nextStates = (*it)->getTransition(terminator);

						//如果划分的代表，即划分中的第一个状态，经过给定的终结符，到达下一个状态
						//如果下一个状态是空的，那么nextStates也应该是空集合
						//如果下一个状态是有状态的，那么两个下一个状态的划分名应该一致，不一致就不等价
						bool isEqual = false;//是否等价
						if (firstNextState) {
							if (!nextStates.empty() && (*(nextStates.begin()))->getDivisionId() == first->getDivisionId()) {
								isEqual = true;
							}
						}
						else if (nextStates.empty()) {
							isEqual = true;
						}

						//等价划分
						if (isEqual) {
							it++;
						}
						else {
							//在原vector上操作会改变迭代器
							newDivision.push_back(*it);
							//设置划分名
							(*it)->setDivisionId(DFADivision.size());
							//将状态从原来的划分中移除
							it = division->erase(it);
						}
					}
				}

				//如果新的划分中有状态
				//就要将新的划分加入到原vector中
				//DFADivisioin
				if (newDivision.size() > 0) {
					//先保存迭代器的值，再进行push_back
					int divisionIndex = division - DFADivision.begin();
					DFADivision.push_back(newDivision);
					division = DFADivision.begin() + divisionIndex;

					//设置标志，继续循环
					hasNewDivision = true;
				}
			}
		}
	}


	//保存DFA最小化
	void saveDFAMinimized() {
		//遍历所有DFA的划分
		//将其中的状态组合成一个新的状态
		//保存起来
		for (auto it : DFADivision) {
			combineDFAStates(it);
		}

		for (auto it : DFADivision) {
			setDFATransition(it);
		}
	}


	//设置DFA最小化后的状态转换
	void setDFATransition(vector<State*> states) {
		//因为划分前和划分后区分出了新旧状态，所以求状态比较麻烦
		for (auto state : states) {
			auto division = state->getDivisionId();//当前划分名
			auto stateMinimized = DFAMinimized[division];//当前最小化后，划分名所对应的状态

			for (auto it : state->getTransitionMap()) {
				auto terminator = it.first;//经过某个终结符
				auto nextState = *(it.second.begin());//到达下一个状态
				auto nextDivision = nextState->getDivisionId();//获得下一个状态的划分名
				auto nextStateMinimized = DFAMinimized[nextDivision];//划分名所对应的新状态
				
				stateMinimized->addTransition(terminator, nextStateMinimized);
			}
		}
	}


	//获得NFA终态的符号
	Symbol getNFAEndSymbol() {
		//右线性的初态是#对应的状态
		//左线性的初态是开始符号对应的状态
		if (grammarLinear == RIGHT_LINEAR) {
			return Alphabet::getSymbol(additionalLabel);
		}
		if (grammarLinear == LEFT_LINEAR) {
			return grammar.getStart();
		}
	}


	//设置DFA的终态
	void setDFAEnd() {
		//获得NFA终态的符号
		//有一个符号
		Symbol NFAEndSymbol = getNFAEndSymbol();

		//遍历DFA
		for (auto it : DFA) {
			//如果DFA的符号中含有NFA终态的符号，则设置为终态
			if (it.first.find(NFAEndSymbol) != it.first.end()) {
				it.second->setStatus(StateStatus::end);
			}
		}
	}

public:
	const static bool LEFT_LINEAR = false;//左线性的标志，同时表示A->Ba中B的下标0

	const static bool RIGHT_LINEAR = true;//右线性的标志，同时表示A->aB中B的下标1

	const static int STATE_NOT_FOUND = -1;//在DFAmap中找不到对应状态时的返回值


	//析构函数
	~StateGraph() {
		for (auto it = NFA.begin(); it != NFA.end(); it++) {
			State* p = it->second;
			delete p;
		}
		for (auto it = DFA.begin(); it != DFA.end(); it++) {
			State* p = it->second;
			delete p;
		}
	}


	//构造函数
	//文法初始化，额外符号的字符初始化为#，同时生成状态图
	StateGraph(Grammar& grammar) :grammar(grammar) {
		this->additionalLabel = "#";
		Alphabet::addSymbol(additionalLabel, Symbol::NONTERMINATOR);
		this->NFA = map<Symbol, State*>();
		generateStateGraph();
	}


	//根据文法生成状态图
	void generateStateGraph() {
		if (grammar.isLeftLinear()) {
			grammarLinear = LEFT_LINEAR;
			generateNFA();
		}
		else if (grammar.isRightLinear()) {
			grammarLinear = RIGHT_LINEAR;
			generateNFA();
		}
		else {
			throw exception();
		}
		printNFA();
		NFAToDFA();
		printDFA();
		minimizeDFA();
		saveDFAMinimized();
		printDFAMinimized();
	}


	//输出NFA
	void printNFA() {
		for (auto it : NFA) {
			cout << it.first << ": \n";
			for (auto i : it.second->getTransitionMap()) {
				cout << i.first << "=>";
				for (auto* it : i.second) {
					cout << *it;
				}
			}
			cout << "\n\n";
		}
	}


	//输出DFA
	void printDFA() {
		cout << "\n\nDFA\n\n";
		for (auto it : DFAStates) {
			cout << it->getStateId() << ": ";
			auto nonterminators = it->getNonterminators();
			Util::printSet(nonterminators);
			cout << ": \n";
			for (auto transition : it->getTransitionMap()) {
				cout << transition.first << "=>";
				for (auto state : transition.second) {
					cout << state->getStateId() << ": ";
					auto nonterminators = state->getNonterminators();
					Util::printSet(nonterminators);
				}
			}
			cout << "\n\n";
		}
		cout << "\n\nDFA\n\n";
	}

	void printDFAMinimized() {
		for (auto it : DFAMinimized) {
			cout << it->getDivisionId() << ": ";
			auto nonterminators = it->getNonterminators();
			Util::printSet(nonterminators);
			cout << ": \n";
			for (auto transition : it->getTransitionMap()) {
				cout << transition.first << "=>";
				for (auto state : transition.second) {
					cout << state->getDivisionId() << ": ";
					auto nonterminators = state->getNonterminators();
					Util::printSet(nonterminators);
				}
			}
			cout << "\n\n";
		}
	}
private:
	void printEpsilonClosure();
};


enum class StateGraph::Selection;


//输出epsilon闭包
void StateGraph::printEpsilonClosure()
{
	// TODO: 在此处添加实现代码.
	for (auto it : epsilonClosure) {
		cout << "状态符号：" << *(it.first) << "：\n";
		cout << "闭包：";
		for (auto state : it.second) {
			cout << *state << " ";
		}
		cout << "\n\n";
	}
}
