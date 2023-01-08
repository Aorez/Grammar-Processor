#pragma once
#include<bits/stdc++.h>


#include"Symbol.h"


using namespace std;


//��̬��̬��̬ͨ
enum class StateStatus {
	start,
	end,
	common
};


//״̬ͼ��״̬
class State {
private:
	int stateId;//״̬��

	int divisionId;//������

	set<Symbol> nonterminators;//״̬������ķ��н����NFA����һ����DFA���ж��

	int status;//��̬��̬��̬ͨ��start��end��common

	map<Symbol, set<State*>> transitionMap;//��ǰ״̬�����ս�������ĸ�״̬

public:
	const static int DEFAULT_ID = -1;//Ĭ��id


	//���캯��
	//�ķ����Ŵ�����̬��̬��̬ͨ
	State(set<Symbol> nonterminators, StateStatus status = StateStatus::common) :State(nonterminators, DEFAULT_ID, status) {
	}
	//�ķ����Ŵ���״̬������̬��̬��̬ͨ
	State(set<Symbol> nonterminators, int stateId, StateStatus status = StateStatus::common) {
		this->divisionId = DEFAULT_ID;
		this->stateId = stateId;
		this->status = int(status);
		this->nonterminators = nonterminators;
		//transitionMap = map<Symbol, set<State*, StateCompare>>();
		transitionMap = map<Symbol, set<State*>>();
	}
	////Ĭ�Ϲ��캯��
	//State() :State(set<Symbol>(), int(Status::common)) {
	//}
	//����һ��ֻ��һ�����ŵ�״̬
	State(Symbol nonterminator, StateStatus status = StateStatus::common) :State(set<Symbol>({ nonterminator }), status) {
	}
	////�������캯��
	//State(const State& stateToCopy) {
	//	this->symbols = stateToCopy.symbols;
	//	this->transitionMap = stateToCopy.transitionMap;
	//	this->status = stateToCopy.status;
	//}
	//

	//set����
	//���ó�̬��̬��̬ͨ
	void setStatus(StateStatus status) {
		this->status = int(status);
	}
	//���û���
	void setDivisionId(int id) {
		divisionId = id;
	}


	//get����
	//���״̬��
	int getStateId() const {
		return this->stateId;
	}
	//��÷��ս������
	set<Symbol> getNonterminators() {
		return this->nonterminators;
	}
	//���״̬ת��map
	//map<Symbol, set<State*, StateCompare>> getTransitionMap() {
	auto getTransitionMap() {
		return this->transitionMap;
	}
	////���һ������
	////NFA״̬�ķ�����ֻ��һ��
	//Symbol getSingleSymbol() {
	//	return *symbols.begin();
	//}
	//��þ���ĳ�����ź󵽴��״̬��
	//���û�о����÷��ţ����ؿռ���
	auto getTransition(Symbol terminator) {
	//set<State*, StateCompare> getTransition(Symbol terminator) {
		if (transitionMap.find(terminator) == transitionMap.end()) {
			return set<State*>();
		}
		return transitionMap[terminator];
	}
	//��ó�̬��̬��̬ͨ
	auto getStatus() {
		return StateStatus(status);
	}
	//��û�����
	auto getDivisionId() {
		return divisionId;
	}


	//���������
	//����==
	bool operator == (const State& state) const {
		return state.nonterminators == this->nonterminators;
	}
	//����<
	bool operator < (const State& state) const {
		return state.nonterminators < this->nonterminators;
	}
	//�������
	friend ostream& operator<<(ostream& output, State& stateToPrint) {
		set<Symbol> nonterminators = stateToPrint.getNonterminators();
		Util::printSet(nonterminators);
		return output;
	}
	//����string
	operator string() {
		string name;//״̬����NFA��DFA���õ�״̬����һ��
		if (stateId != DEFAULT_ID) {
			name = to_string(stateId);
		}
		else {
			Symbol s = *(nonterminators.begin());
			name = string(s);
		}
		string indent = "    ";
		string result = name + ": \n";
		string splitSign = ", ";
		for (auto it : transitionMap) {
			Symbol s = it.first;
			result += indent + string(s) + "->";
			result += "{";
			string split = "";
			for (auto state : it.second) {
				string name;
				if (state->getStateId() != DEFAULT_ID) {
					name = to_string(state->getStateId());
				}
				else {
					Symbol s = *(state->getNonterminators().begin());
					name = string(s);
				}
				result += split + name;
				split = splitSign;
			}
			result += "}\n";
		}
		return result;
	}
	string toString() {
		string name;//״̬����NFA��DFA���õ�״̬����һ��
		if (divisionId != DEFAULT_ID) {
			name = to_string(divisionId);
		}
		else {
			Symbol s = *(nonterminators.begin());
			name = string(s);
		}
		string indent = "    ";
		string result = name + ": \n";
		string splitSign = ", ";
		for (auto it : transitionMap) {
			Symbol s = it.first;
			result += indent + string(s) + "->";
			result += "{";
			string split = "";
			for (auto state : it.second) {
				string name;
				if (state->getDivisionId() != DEFAULT_ID) {
					name = to_string(state->getDivisionId());
				}
				else {
					Symbol s = *(state->getNonterminators().begin());
					name = string(s);
				}
				result += split + name;
				split = splitSign;
			}
			result += "}\n";
		}
		return result;

	}



	////���״̬ת��
	////��map�еļ���ֵ�ӵ���Ա��״̬ת��map�У�������Ѵ���
	//void addTransition(map<Symbol, set<State>> mapToAdd) {
	//	for (auto it : mapToAdd) {
	//		for (auto state : it.second) {
	//			addTransition(it.first, state);
	//		}
	//	}
	//}
	//���״̬ת��
	void addTransition(Symbol terminator, State* nextState) {
		transitionMap[terminator].insert(nextState);
	}


	//��ӷ��ս��
	void addNonterminators(set<Symbol> nonterminators) {
		this->nonterminators.insert(nonterminators.begin(), nonterminators.end());
	}
	//��ӷ��ս��
	void addNonterminators(State* state) {
		this->addNonterminators(state->getNonterminators());
	}
};
