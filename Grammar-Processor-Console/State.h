#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"


using namespace std;


//初态终态普通态
enum class StateStatus {
	start,
	end,
	common
};


//状态图的状态
class State {
private:
	int stateId;//状态名

	int divisionId;//划分名

	set<Symbol> nonterminators;//状态所代表的非中介符，NFA中有一个，DFA中有多个

	int status;//初态终态普通态，start，end，common

	map<Symbol, set<State*>> transitionMap;//当前状态经过终结符到达哪个状态

public:
	const static int DEFAULT_ID = -1;//默认id


	//构造函数
	//文法符号串，初态终态普通态
	State(set<Symbol> nonterminators, StateStatus status = StateStatus::common) :State(nonterminators, DEFAULT_ID, status) {
	}
	//文法符号串，状态名，初态终态普通态
	State(set<Symbol> nonterminators, int stateId, StateStatus status = StateStatus::common) {
		this->divisionId = DEFAULT_ID;
		this->stateId = stateId;
		this->status = int(status);
		this->nonterminators = nonterminators;
		//transitionMap = map<Symbol, set<State*, StateCompare>>();
		transitionMap = map<Symbol, set<State*>>();
	}
	////默认构造函数
	//State() :State(set<Symbol>(), int(Status::common)) {
	//}
	//创建一个只有一个符号的状态
	State(Symbol nonterminator, StateStatus status = StateStatus::common) :State(set<Symbol>({ nonterminator }), status) {
	}
	////拷贝构造函数
	//State(const State& stateToCopy) {
	//	this->symbols = stateToCopy.symbols;
	//	this->transitionMap = stateToCopy.transitionMap;
	//	this->status = stateToCopy.status;
	//}
	//

	//set函数
	//设置初态终态普通态
	void setStatus(StateStatus status) {
		this->status = int(status);
	}
	//设置划分
	void setDivisionId(int id) {
		divisionId = id;
	}


	//get函数
	//获得状态名
	int getStateId() const {
		return this->stateId;
	}
	//获得非终结符集合
	set<Symbol> getNonterminators() {
		return this->nonterminators;
	}
	//获得状态转换map
	//map<Symbol, set<State*, StateCompare>> getTransitionMap() {
	auto getTransitionMap() {
		return this->transitionMap;
	}
	////获得一个符号
	////NFA状态的符号中只有一个
	//Symbol getSingleSymbol() {
	//	return *symbols.begin();
	//}
	//获得经过某个符号后到达的状态集
	//如果没有经过该符号，返回空集合
	auto getTransition(Symbol terminator) {
	//set<State*, StateCompare> getTransition(Symbol terminator) {
		if (transitionMap.find(terminator) == transitionMap.end()) {
			return set<State*>();
		}
		return transitionMap[terminator];
	}
	//获得初态终态普通态
	auto getStatus() {
		return StateStatus(status);
	}
	//获得划分名
	auto getDivisionId() {
		return divisionId;
	}


	//重载运算符
	//重载==
	bool operator == (const State& state) const {
		return state.nonterminators == this->nonterminators;
	}
	//重载<
	bool operator < (const State& state) const {
		return state.nonterminators < this->nonterminators;
	}
	//重载输出
	friend ostream& operator<<(ostream& output, State& stateToPrint) {
		set<Symbol> nonterminators = stateToPrint.getNonterminators();
		Util::printSet(nonterminators);
		return output;
	}



	////添加状态转换
	////将map中的键和值加到成员的状态转换map中，如果键已存在
	//void addTransition(map<Symbol, set<State>> mapToAdd) {
	//	for (auto it : mapToAdd) {
	//		for (auto state : it.second) {
	//			addTransition(it.first, state);
	//		}
	//	}
	//}
	//添加状态转换
	void addTransition(Symbol terminator, State* nextState) {
		transitionMap[terminator].insert(nextState);
	}


	//添加非终结符
	void addNonterminators(set<Symbol> nonterminators) {
		this->nonterminators.insert(nonterminators.begin(), nonterminators.end());
	}
	//添加非终结符
	void addNonterminators(State* state) {
		this->addNonterminators(state->getNonterminators());
	}
};
