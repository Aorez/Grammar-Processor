#pragma once
#include<bits/stdc++.h>


#include"Production.h"
#include"Grammar.h"


#include"Util.h"
#include"GrammarPretreatment.h"


using namespace std;


//����ʽ�Ƿ񲻿���ֹ�ļ��ľ�̬��
static class TerminationFilter {
private:
	static vector<Production> interminableProductions;//���治����ֹ�Ĳ���ʽ

	static vector<string> interminableLabels;//���治����ֹ���󲿵ķ���

	static map<Symbol, pair<int, int>> statusMap;//����Ƿ񲻿���ֹʱ����¼��鵽����һ���Ҳ�����һ�����ţ�������״̬��Ϣ

	static Grammar& previousGrammar;//��һ�����ܽ��к���ķ�

	static Grammar grammar;//ɾ���˲�����ֹ����ķ�

	//��ʼ��statusMap״̬����
	static void initializeStatusMap() {
		vector<Symbol> allSymbol = Alphabet::getAllSymbol();//ȫ�����ŵ�vector
		for (auto& it : allSymbol) {
			//����Ƿ��ս�����ж��Ƿ������д��ڣ����û�У�ֱ�����ò�����ֹ
			if (it.isNonTerminator()) {
				//��������д���
				//�ټ���Ƿ���ȫ�����ս����ɵĲ���ʽ
				if (previousGrammar.existLeft(it)) {

					//�÷��ŵĲ���ʽ����ȫ�����ս����ɵĲ���ʽ������ֹ
					if (hasProductionAllMadeOfTerminators(it)) {
						Util::addInitialValueToMap(statusMap, it, TERMINABILITY);
					}
					else {
						//����δ���
						Util::addInitialValueToMap(statusMap, it, TERMINATION_UNCHECKED);
					}
				}
				else {
					//�����в�����
					//���ò�����ֹ
					Util::addInitialValueToMap(statusMap, it, INTERMINABILITY);

					//��Ҫ���벻����ֹ�ķ��ŵļ���
					//��Ϊ����ɾ������ʽ��ʱ��ŷ���ü��ϣ�������Щ����û�в���ʽ
					interminableLabels.push_back(it.getLabel());
				}
			}
		}
	}


	//��鲻����ֹ�Ĳ���ʽ
	static void checkTermination() {
		//ֻɨ��һ�����в���ʽ�ǲ����ģ����Ҫѭ��
		//��change��¼�Ƿ��в�����ֹ�Ϳ���ֹ�ĸı�
		//����иı䣬��Ӧ����ɨ��һ��
		bool change = true;
		map<Symbol, vector<Production>>& productionsMap = previousGrammar.getProductionsMap();//��ȡ���в���ʽ�ļ���
		while (true) {
			if (change) {
				change = false;
			}
			else {
				break;
			}

			//�������в���ʽ
			for (map<Symbol, vector<Production>>::iterator it = productionsMap.begin(); it != productionsMap.end(); it++) {
				Symbol left = it->first;//��ǰ����
				pair<int, int>& status = statusMap[left];//��鲻����ֹʱ����ǰ�󲿼�鵽����һ���Ҳ�����һ������
				vector<Production>& productions = it->second;//��ǰ�󲿵����в���ʽ

				//TERMINATION_UNCHECKED��ʾ��ǰ�󲿻�δ������
				if (status == TERMINATION_UNCHECKED) {
					status = make_pair(0, 0);
				}

				//TERMINABILITY��ʾ�Ѿ�ȷ������ֹ��INTERMINABILITY��ʾ������ֹ������Ѿ�ȷ���ˣ��������һ���󲿵ļ��
				if (status == TERMINABILITY || status == INTERMINABILITY) {
					continue;
				}

				while (true) {
					//����Ҳ��ĵ�һ���±곬����������˳�ѭ��������ǰ�����Ҳ��Ѿ�������
					if (status.first >= productions.size()) {

						//���������ʵĬ�Ͼ��ǲ�����ֹ��
						//��Ϊֻ���Ҳ�������ֹ�ĲŻ������ȫ���Ҳ�
						//����ҵ��˿���ֹ��ĳһ���Ҳ�����������Ҳ��ķ��ű������ˣ�Ҳ�����������if������
						//�����������catch����
						//�֣�����������ֹ��first�±�Ż�Ӽӣ���������ֹ��second�±�Ż�Ӽ�
						status = INTERMINABILITY;
						change = true;
						break;
					}

					//����ڶ���������ʹ�������������
					//�ڶ����±겻�Ϸ������糬�������׳��쳣
					try {
						Symbol symbol = productions[status.first][status.second];//��ǰ���Ҳ��ĵ�ǰ����

						//�����ǰ���Ҳ��ĵ�ǰ�ķ������ս������ô��Ҫ����ɨ�����ķ���
						if (symbol.isNonTerminator() == false) {
							//�ڶ����±�����
							status.second++;
							continue;
						}

						//�����ǰ���Ҳ��ĵ�ǰ�����Ƿ��ս������ô��Ҫ����������ս���Ƿ����ֹ
						if (symbol.isNonTerminator()) {

							//��A->����A������
							//�������ǽ��к�����Ҳ����жϣ�������ǰ�Ҳ�
							if (symbol.getLabel() == left.getLabel()) {
								status.first++;
								status.second = 0;
								continue;
							}

							pair<int, int> symbolStatus = statusMap[symbol];//��õ�ǰ�Ҳ��ĵ�ǰ���ŵ�pair�������±�

							//�����ǰ�Ҳ��ĵ�ǰ�����Ѿ������������ǿ���ֹ��
							//��ڶ����±�������������鵱ǰ�Ҳ�����һ������
							if (symbolStatus == TERMINABILITY) {
								status.second++;
								continue;
							}

							//�����ǰ�Ҳ��ĵ�ǰ�����Ѿ������������ǲ�����ֹ��
							//Ҫ������Ҳ�ɾ�������������Ȳ�ɾ����Ϊ���ܻ�ɾ���ɾ������硰A->a|C��������C������ֹ�����ʱ����ɾ���ɾ���
							//�����ж���һ���Ҳ�
							if (symbolStatus == INTERMINABILITY) {
								status.first++;
								status.second = 0;
								continue;
							}

							//�����ǰ�Ҳ��ĵ�ǰ���Ż�δ����飬��Ҫ���ֵ�ǰ�󲿵�pair�������±�
							//�˳�ѭ�������������һ����
							if (symbolStatus == TERMINATION_UNCHECKED) {
								break;
							}
							//�������˵����ǰ�Ҳ��ĵ�ǰ���Ż��ڼ���У�ҲҪ�˳�ѭ��
							break;

						}
					}
					catch (const std::exception&) {
						//�±곬����˵���Ѿ�������ĳһ���Ҳ�
						//��ʵ��ζ����ĳһ���Ҳ��ǿ���ֹ�ģ��Ż�����꣬���Ե�ǰ�󲿿���ֹ
						status = TERMINABILITY;
						change = true;
						break;
					}
				}
			}
		}

	}


	//����겻����ֹ�Ĳ���ʽ֮����ִ��ɾ���Ĳ���
	static void deleteInterminabilities() {
		//ǰ���ѭ����pair��������֮��
		//�Ϳ��Կ�ʼɾ��������ֹ�Ĳ���ʽ��
		//��ɾ�����ǲ�����ֹ��
		map<Symbol, vector<Production>>& productionsMap = previousGrammar.getProductionsMap();//��ȡ���в���ʽ�ļ���
		for (auto& it : productionsMap) {
			Symbol left = it.first;//��ǰ����

			//����ǿ���ֹ���󲿣���ӵ����ķ���
			if (statusMap[left] == TERMINABILITY) {
				grammar.addProductions(left, it.second);
			}
			//����������ǲ�����ֹ
			//�����
			else {
				//��������ֹ�Ĳ���ʽ���󲿷��Ŷ���������
				interminableLabels.push_back(left.getLabel());
				interminableProductions.insert(interminableProductions.end(), it.second.begin(), it.second.end());
			}
		}

		//��ɾ���Ҳ��г��ֵġ�������ֹ���󲿷���
		for (map<Symbol, vector<Production>>::iterator it = grammar.getProductionsMap().begin(); it != grammar.getProductionsMap().end(); it++) {
			vector<Production>& productions = it->second;//��ǰ�󲿵������Ҳ�

			//���������Ҳ��������Ƿ��в�����ֹ���󲿷���
			for (vector<Production>::iterator it2 = productions.begin(); it2 != productions.end();) {
				bool hasInterminability = false;//��¼�Ҳ��Ƿ��в�����ֹ�ķ��ŵĳ���

				//�������в�����ֹ�ķ��ţ�һ��һ�������ж�
				for (string interminableLabel : interminableLabels) {
					//���ú��������Ҳ����Ƿ���ĳ�����ŵ��ж�
					hasInterminability = it2->rightHasLabel(interminableLabel);
					if (hasInterminability) {
						break;
					}
				}

				//����в�����ֹ�ķ��ţ��������ʽӦ��ɾ��
				if (hasInterminability) {
					interminableProductions.push_back(*it2);
					it2 = productions.erase(it2);
				}
				else {
					it2++;
				}
			}
		}

		//����ĸ���еĲ�����ֹ�ķ���ɾ��
		for (auto& label : interminableLabels) {
			Alphabet::removeSymbol(label);
		}
	}


	//����ĳ�����ŵ����в���ʽ������ֻ���ս���Ĳ���ʽ
	//������ǰ�ж��Ƿ����ֹ
	static bool hasProductionAllMadeOfTerminators(Symbol left) {
		for (auto& leftProduction : previousGrammar.getProductions(left)) {
			//�����һ������ʽȫ���ս�����
			//ֱ�ӷ���
			if (leftProduction.isAllMadeOfTerminators()) {
				return true;
			}
		}
		return false;
	}

public:
	const static pair<int, int> TERMINATION_UNCHECKED;//��ʾ���󲿻�û�б�����Ƿ񲻿���ֹ

	const static pair<int, int> TERMINABILITY;//��ʾ���󲿱�������ˣ������ǿ�����ֹ��״̬

	const static pair<int, int> INTERMINABILITY;//��ʾ���󲿱�������ˣ��ǲ�����ֹ��


	//get����
	//����ķ�
	static Grammar& getGrammar() {
		return grammar;
	}


	//�ܵĺ�������
	static void filterTermination() {
		//��ʼ��ÿ�����ŵ�pairӳ��
		initializeStatusMap();

		//��鲻����ֹ
		checkTermination();

		//ɾ��������ֹ
		deleteInterminabilities();

		//���
		cout << "\ninterminableProductions\n";
		Util::printVectorInLines(interminableProductions);
		cout << "\nproductionsMap\n";
		grammar.printProductionsMap();
	}
};


vector<Production> TerminationFilter::interminableProductions;//���治����ֹ�Ĳ���ʽ
vector<string> TerminationFilter::interminableLabels;//���治����ֹ���󲿵ķ���
map<Symbol, pair<int, int>> TerminationFilter::statusMap;//����Ƿ񲻿���ֹʱ����¼��鵽����һ���Ҳ�����һ�����ţ�������״̬��Ϣ
const pair<int, int> TerminationFilter::TERMINATION_UNCHECKED(-1, -1);//��ʾ���󲿻�û�б�����Ƿ񲻿���ֹ
const pair<int, int> TerminationFilter::TERMINABILITY(-2, -2);//��ʾ���󲿱�������ˣ������ǿ�����ֹ��״̬
const pair<int, int> TerminationFilter::INTERMINABILITY(-3, -3);//��ʾ���󲿱�������ˣ��ǲ�����ֹ��
Grammar& TerminationFilter::previousGrammar = GrammarPretreatment::getGrammar();//��һ�����ܽ��к���ķ�
Grammar TerminationFilter::grammar;//ɾ���˲�����ֹ����ķ�
