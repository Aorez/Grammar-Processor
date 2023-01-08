#pragma once
#include<bits/stdc++.h>


#include"Grammar.h"
#include"State.h"
#include"Util.h"


using namespace std;


//״̬ͼ����
class StateGraph {
private:
	Grammar& grammar;//״̬ͼ��Ӧ���ķ�

	vector<State*> DFAMinimized;//������=>״̬

	vector<vector<State*>> DFADivision;//DFA��С���Ļ���
	
	map<set<Symbol>, State*> DFA;//����DFA

	vector<State*> DFAStates;//DFA

	map<Symbol, State*> NFA;//״̬��=>״̬���������ս��=>״̬

	map<State*, set<State*>> epsilonClosure;//epsilon�հ�

	string additionalLabel;//���Ӷ���ķ��ţ����������б�ʾ��̬�����������б�ʾ��̬��Ĭ��Ϊ#

	bool grammarLinear;//�ķ������Թ��������

	static enum class Selection
	{
		NFA,
		DFA,
	};
	
	
	//���NFA״̬������Ѵ��ڣ������
	//�������ս��
	State* addNFAState(Symbol nonterminator, StateStatus status = StateStatus::common) {
		if (NFA.find(nonterminator) == NFA.end()) {
			NFA[nonterminator] = new State(nonterminator, status);
		}
		return NFA[nonterminator];
	}


	//����״̬ͼ�������Ի���������
	void generateNFA(bool epsilonTwoWay = false) {
		Selection select = Selection::NFA;
		map<bool, StateStatus> pairStatus;
		pairStatus[LEFT_LINEAR] = StateStatus::end;
		pairStatus[RIGHT_LINEAR] = StateStatus::start;

		//���뿪ʼ���ŵ�״̬
		//�����Կ�ʼ����Ϊ��̬�������Կ�ʼ����Ϊ��̬
		addNFAState(grammar.getStart(), pairStatus[grammarLinear]);
		//��������Ҫ����ĳ�̬����������Ҫ�������̬
		State* additionalState = addNFAState(Alphabet::getSymbol(additionalLabel), pairStatus[!grammarLinear]);

		//���������󲿼������ʽ
		for (auto it : grammar.getProductionsMap()) {
			//���󲿵ķ��Ŵ�����Ϊ��̬ͨ
			State* leftState = addNFAState(it.first);

			for (auto production : it.second) {
				//�����������epsilonTwoWayΪtrue����epsilon���������Ϊ˫���
				if (epsilonTwoWay && production.isEpsilon()) {
					additionalState->addTransition(Alphabet::getEpsilonSymbol(), leftState);
					leftState->addTransition(Alphabet::getEpsilonSymbol(), additionalState);
					continue;
				}

				switch (production.getRightSize())
				{
					//�ղ���ʽ
					//�����ԣ������״̬����̬������epsilon�����󲿷��ŵ�״̬
					//�����ԣ��󲿷��ž���epsilon��������״̬����̬��
				case 0: {
					//ǰ���Ѿ�������ˣ�һֱ��ͬһ���ͺ���
					//����״̬ת��
					if (grammarLinear == LEFT_LINEAR) {
						additionalState->addTransition(Alphabet::getEpsilonSymbol(), leftState);
					}
					else {
						leftState->addTransition(Alphabet::getEpsilonSymbol(), additionalState);
					}
					break;
				}
					//��һ�����ţ��������ս��������epsilon
				case 1: {
					if (grammarLinear == LEFT_LINEAR) {
						additionalState->addTransition(production[0], leftState);
					}
					else {
						leftState->addTransition(production[0], additionalState);
					}
					break;
				}
					  //��������A->aB������A����a����״̬B����·
				case 2: {
					//������A->Ba������B��״̬
					//������A->aB
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


	//���epsilon�հ�
	void solveEpsilonClosure() {
		//����ÿ��NFA״̬����ÿ��״̬��epsilon�հ�
		for (auto it : NFA) {
			State* NFAState = it.second;//NFA״̬

			//�����������
			queue<State*> stateStack;//BFSջ
			stateStack.push(NFAState);

			while (!stateStack.empty()) {
				State* state = stateStack.front();//����ջ��
				stateStack.pop();
				//���Լ��ӵ�epsilon�հ���
				epsilonClosure[NFAState].insert(state);

				//��þ���epsilon���״̬��
				auto states = state->getTransition(Alphabet::getEpsilonSymbol());
				//��״̬���ӵ�epsilon�հ���
				epsilonClosure[NFAState].insert(states.begin(), states.end());
				//����ջ��
				for (State* p : states) {
					stateStack.push(p);
				}
			}
		}
	}


	//��ͬһ�������е�DFA״̬��ϳ�һ��
	//ͬһ�������е�DFA״̬��ת����һ����
	//״̬ת��
	//������ ���� �ս�� ���� ������
	//ͬһ��������������̬���Ͷ�����̬
	//����г�̬���ͽ����������Ϊ��̬
	//����û�취ȷ������������Ӧ���µ���С�����DFA״̬
	//����ȫ��������������ȷ������˲�����ת��
	//ת������Ϊ��
	void combineDFAStates(vector<State*> states) {
		StateStatus status = StateStatus::common;
		set<Symbol> nonterminators;//���ս���ļ���

		for (auto state : states) {
			//�����̬����̬
			if (state->getStatus() == StateStatus::start) {
				status = StateStatus::start;
			}
			else if (state->getStatus() == StateStatus::end) {
				status = StateStatus::end;
			}

			//��ÿ��״̬�ķ��ս���ŵ�һ��
			auto subNonterminators = state->getNonterminators();
			nonterminators.insert(subNonterminators.begin(), subNonterminators.end());
		}

		//�ȴ���״̬��Ȼ��������ת��
		State* newState = new State(nonterminators, status);
		//���û�����
		int divisionId = (*states.begin())->getDivisionId();
		newState->setDivisionId(divisionId);

		//��Ϊ�ǰ�˳�򴴽�������ֱ��push_back
		DFAMinimized.push_back(newState);
		//DFAMinimized[divisionId] = newState;
	}


	//NFAתDFA
	void NFAToDFA() {
		solveEpsilonClosure();
		printEpsilonClosure();

		//���DFA��̬
		State* DFAStart = getDFAStart();

		queue<State*> stateStack;
		stateStack.push(DFAStart);
		while (!stateStack.empty()) {
			State* DFAState = stateStack.front();
			stateStack.pop();
			
			//����ÿ���ս����״̬���ܾ���Ҳ���ܲ�����
			for (auto terminator : Alphabet::getAllTerminator()) {
				//����epsilon
				if (terminator == Alphabet::getEpsilonSymbol()) {
					continue;
				}

				//����DFA״̬���Ե���ķ��ս��������֮�󴴽���״̬
				set<Symbol> nonterminators;

				//����DFA״̬�еķ��ռ���
				for (auto nonterminator : DFAState->getNonterminators()) {

					//���ս����Ӧ��NFA״̬
					State* NFAState = NFA[nonterminator];
					//��ø�״̬�����ս���������NFA״̬���ϣ�������ȡ���еķ��ս��
					//Ȼ������ܵķ��ս������
					//nonterminators
					auto NFAStates = NFAState->getTransition(terminator);
					//��������ǿյģ�˵��û�о�������ս��
					if (NFAStates.empty()) {
						continue;
					}

					//��������״̬�������ıհ��е�״̬���뵽ͬһ������
					auto NFAStatesTemp = NFAStates;
					for (auto p : NFAStatesTemp) {
						auto statesTemp = epsilonClosure[p];
						NFAStates.insert(statesTemp.begin(), statesTemp.end());
					}

					//�������ϱհ��е�״̬������״̬��ȡ�÷��ս��
					for (auto p : NFAStates) {
						auto subNonterminators = p->getNonterminators();
						nonterminators.insert(subNonterminators.begin(), subNonterminators.end());
						//nonterminators.insert(p->getNonterminators().begin(), p->getNonterminators().end());
					}
				}

				//��ǰ״̬�еķ��ռ��������������ս��
				if (nonterminators.empty()) {
					continue;
				}

				//������״̬�������״̬�Ѵ��ڣ�������ջ
				//��ͨ״̬
				//״̬��Ϊmap����size���Զ�����
				if (DFA.find(nonterminators) == DFA.end()) {
					State* newState = new State(nonterminators, DFAStates.size());
					DFA[nonterminators] = newState;
					DFAStates.push_back(newState);
					stateStack.push(newState);
				}

				//����״̬ת��
				//��ǰDFA״̬ �����ս�� ������״̬
				DFAState->addTransition(terminator, DFA[nonterminators]);
			}
		}

		//����DFA����̬
		setDFAEnd();
	}


	//���NFA��̬
	State* getNFAStart() {
		//�����Եĳ�̬��#��Ӧ��״̬
		//�����Եĳ�̬�ǿ�ʼ���Ŷ�Ӧ��״̬
		if (grammarLinear == LEFT_LINEAR) {
			return NFA[Alphabet::getSymbol(additionalLabel)];
		}
		if (grammarLinear == RIGHT_LINEAR) {
			return NFA[grammar.getStart()];
		}
	}
	//���DFA��̬
	//������½���
	//ֻ�з��ս����û��״̬ת��
	State* getDFAStart() {
		//���NFA��̬
		State* NFAStart = getNFAStart();
		//��ñհ�
		auto epsilon = epsilonClosure[NFAStart];
		set<Symbol> nonterminators;//���ս��������DFA��̬�Ĵ���
		//�����հ��е�״̬
		for (auto NFAState : epsilon) {
			//ȡ��״̬�ķ��ս��
			for (auto nonterminator : NFAState->getNonterminators()) {
				//���뵽������
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


	//DFA��С��
	void minimizeDFA() {
		//��ʼ���֣�����Ϊ��̬�ͷ���̬
		//���������ռ���
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

		bool hasNewDivision = true;//������µĻ��ֲ�������Ҫ�������֣�ֱ��û���µĻ���
		while (hasNewDivision) {
			hasNewDivision = false;

			for (auto division = DFADivision.begin(); division != DFADivision.end(); division++) {
				//������ֵļ�����ֻ��һ�����ǾͲ��û���
				//����ջǰҲ���ж���
				if (division->size() <= 1) {
					continue;
				}

				//����֮ǰ�Ȳ���һ���¼��ϣ�����¼���Ϊ�գ�֮����ɾ��
				//������push_back�����������ɿ�
				//DFADivision.push_back(vector<State*>());
				//�����½�vector�����µĻ���
				vector<State*> newDivision;

				//�������еĵ�һ����Ϊ���ֵĴ���
				State* first = (*division)[0];

				//����ÿ���ս��
				for (auto terminator : Alphabet::getAllTerminator()) {
					//DFA��û��epsilon
					if (terminator == Alphabet::getEpsilonSymbol()) {
						continue;
					}

					//���ɾ����ֻʣ��һ�������ˣ��Ͳ�����ѭ����
					if (division->size() <= 1) {
						break;
					}

					//���ת�������һ��״̬����ΪDFAת�����״̬��ȷ����
					//����ֻ��һ����ֻ��Ҫ��ȡ�����еĵ�һ��
					//Ҳ�п���Ϊ��
					set<State*> firstNextStates = first->getTransition(terminator);
					State* firstNextState = nullptr;
					if (!firstNextStates.empty()) {
						firstNextState = *(firstNextStates.begin());
					}

					//�ӻ����е���һ����ʼ����
					for (auto it = division->begin() + 1; it != division->end();) {
						//�����һ��״̬�ļ��ϣ��п����ǿռ���
						auto nextStates = (*it)->getTransition(terminator);

						//������ֵĴ����������еĵ�һ��״̬�������������ս����������һ��״̬
						//�����һ��״̬�ǿյģ���ônextStatesҲӦ���ǿռ���
						//�����һ��״̬����״̬�ģ���ô������һ��״̬�Ļ�����Ӧ��һ�£���һ�¾Ͳ��ȼ�
						bool isEqual = false;//�Ƿ�ȼ�
						if (firstNextState) {
							if (!nextStates.empty() && (*(nextStates.begin()))->getDivisionId() == first->getDivisionId()) {
								isEqual = true;
							}
						}
						else if (nextStates.empty()) {
							isEqual = true;
						}

						//�ȼۻ���
						if (isEqual) {
							it++;
						}
						else {
							//��ԭvector�ϲ�����ı������
							newDivision.push_back(*it);
							//���û�����
							(*it)->setDivisionId(DFADivision.size());
							//��״̬��ԭ���Ļ������Ƴ�
							it = division->erase(it);
						}
					}
				}

				//����µĻ�������״̬
				//��Ҫ���µĻ��ּ��뵽ԭvector��
				//DFADivisioin
				if (newDivision.size() > 0) {
					//�ȱ����������ֵ���ٽ���push_back
					int divisionIndex = division - DFADivision.begin();
					DFADivision.push_back(newDivision);
					division = DFADivision.begin() + divisionIndex;

					//���ñ�־������ѭ��
					hasNewDivision = true;
				}
			}
		}
	}


	//����DFA��С��
	void saveDFAMinimized() {
		//��������DFA�Ļ���
		//�����е�״̬��ϳ�һ���µ�״̬
		//��������
		for (auto it : DFADivision) {
			combineDFAStates(it);
		}

		for (auto it : DFADivision) {
			setDFATransition(it);
		}
	}


	//����DFA��С�����״̬ת��
	void setDFATransition(vector<State*> states) {
		//��Ϊ����ǰ�ͻ��ֺ����ֳ����¾�״̬��������״̬�Ƚ��鷳
		for (auto state : states) {
			auto division = state->getDivisionId();//��ǰ������
			auto stateMinimized = DFAMinimized[division];//��ǰ��С���󣬻���������Ӧ��״̬

			for (auto it : state->getTransitionMap()) {
				auto terminator = it.first;//����ĳ���ս��
				auto nextState = *(it.second.begin());//������һ��״̬
				auto nextDivision = nextState->getDivisionId();//�����һ��״̬�Ļ�����
				auto nextStateMinimized = DFAMinimized[nextDivision];//����������Ӧ����״̬
				
				stateMinimized->addTransition(terminator, nextStateMinimized);
			}
		}
	}


	//���NFA��̬�ķ���
	Symbol getNFAEndSymbol() {
		//�����Եĳ�̬��#��Ӧ��״̬
		//�����Եĳ�̬�ǿ�ʼ���Ŷ�Ӧ��״̬
		if (grammarLinear == RIGHT_LINEAR) {
			return Alphabet::getSymbol(additionalLabel);
		}
		if (grammarLinear == LEFT_LINEAR) {
			return grammar.getStart();
		}
	}


	//����DFA����̬
	void setDFAEnd() {
		//���NFA��̬�ķ���
		//��һ������
		Symbol NFAEndSymbol = getNFAEndSymbol();

		//����DFA
		for (auto it : DFA) {
			//���DFA�ķ����к���NFA��̬�ķ��ţ�������Ϊ��̬
			if (it.first.find(NFAEndSymbol) != it.first.end()) {
				it.second->setStatus(StateStatus::end);
			}
		}
	}

public:
	const static bool LEFT_LINEAR = false;//�����Եı�־��ͬʱ��ʾA->Ba��B���±�0

	const static bool RIGHT_LINEAR = true;//�����Եı�־��ͬʱ��ʾA->aB��B���±�1

	const static int STATE_NOT_FOUND = -1;//��DFAmap���Ҳ�����Ӧ״̬ʱ�ķ���ֵ


	//��������
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


	//���캯��
	//�ķ���ʼ����������ŵ��ַ���ʼ��Ϊ#��ͬʱ����״̬ͼ
	StateGraph(Grammar& grammar) :grammar(grammar) {
		this->additionalLabel = "#";
		Alphabet::addSymbol(additionalLabel, Symbol::NONTERMINATOR);
		this->NFA = map<Symbol, State*>();
		generateStateGraph();
	}


	//�����ķ�����״̬ͼ
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


	//���NFA
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


	//���DFA
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


//���epsilon�հ�
void StateGraph::printEpsilonClosure()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	for (auto it : epsilonClosure) {
		cout << "״̬���ţ�" << *(it.first) << "��\n";
		cout << "�հ���";
		for (auto state : it.second) {
			cout << *state << " ";
		}
		cout << "\n\n";
	}
}
