#pragma once
#include<bits/stdc++.h>


using namespace std;



//�����࣬��̬��
static class Util {
private:

public:
	const static bool RETAIN = true;//�����������ַ�����Ҫ�õ�

	const static bool NOT_RETAIN = false;//�������������ַ�����Ҫ�õ�

	const static string CATCH_EXCEPTION;//�쳣��ʾ


	//map���Ϲ���
	//���map���ϵ�ֵ��һ��ֵһ��
	template<class T, class K>
	static void printMapValueInLines(map<T, K>& map) {
		for (auto& it : map) {
			cout << it.second << endl;
		}
	}
	//��ʽ�����map����
	template<class T, class K>
	static void printMap(map<T, K>& map) {
		cout << "{\n";
		for (auto& it : map) {
			cout << "    " << it.first << " => " << it.second << endl;
		}
		cout << "}\n";
	}
	//��map������ֵתΪvector�󷵻�
	template<class T, class K>
	static vector<K> getMapValueAsVector(map<T, K>& map) {
		vector<K> values;
		for (auto& it : map) {
			values.push_back(it.second);
		}
		return values;
	}
	//���map�����м���Ӧ��ֵ
	//��������ڸü����׳��쳣
	template<class T, class K>
	static K& getMapValue(map<T, K>& mapToGet, T& key) {
		//����Ҳ����ü����׳��쳣
		if (mapToGet.find(key) == mapToGet.end()) {
			throw exception();
		}
		else {
			return mapToGet[key];
		}
	}
	//��map�����в����ֵ
	//����Ѵ��ھͲ�����
	template<class T, class K>
	static void addInitialValueToMap(map<T, K>& mapToAdd, const T& key, const K& value) {
		if (mapToAdd.find(key) == mapToAdd.end()) {
			mapToAdd.insert(pair<T, K>(key, value));
		}
	}


	//vector����
	//���vector�е�ֵ��һ��ֵһ��
	template<class T>
	static void printVectorInLines(vector<T>& vec) {
		for (auto& it : vec) {
			cout << it << endl;
		}
	}
	//���vector�е�ֵ����һ����������Էָ����ָ��������������ʾ���°���
	template<class T>
	static void printVectorInALine(vector<T>& vec, string splitSign, string printPrompt) {
		cout << endl << printPrompt << endl;
		string splitSymbol = "";
		for (auto& it : vec) {
			cout << splitSymbol << it;
			splitSymbol = splitSign;
		}
		cout << endl << printPrompt << endl;
	}


	//set����
	template<class T>
	static void printSet(set<T>& setToPrint) {
		cout << "{";
		for (auto i : setToPrint) {
			cout << i << ", ";
		}
		cout << "}";
	}


	//�ַ���������
	//�����ַ�
	//���retainΪtrue������characters�е�����
	static string filterCharacter(string aString, const vector<char>& characters, bool retain) {
		string result = "";
		string characterString(characters.begin(), characters.end());//���ַ�vector���string
		for (const auto& c : aString) {
			//����ַ�����Ҫ���˵��ַ������У��������ӵ�����ַ�����
			//retain�����������ʵ��һ��switch�Ľṹ��������дswitch
			if (retain && characterString.find(c) != string::npos) {
				result.push_back(c);
			}
			if (!retain && characterString.find(c) == string::npos) {
				result.push_back(c);
			}
		}
		return result;
	}
	//���ַ�������ĳ���ַ������зָ�ɼ���
	static vector<string> splitString(string stringToSplit, string splitSign) {
		int splitIndex = stringToSplit.find(splitSign);

		//�������npos��˵���Ҳ����ָ�����ֱ�ӷ���
		//npos��string��Ԥ�����
		if (splitIndex == string::npos) {
			return vector<string>({ stringToSplit });
		}

		vector<string> stringSplit;//�ָ����ַ�������
		//�ָ�����һ�ε��±� �� �� ����Ҫ��ȡ�ĵ�һ���ַ����ĳ���
		stringSplit.push_back(stringToSplit.substr(0, splitIndex - 0));
		//��a|b|c������Ϊ��b|c��
		stringToSplit = stringToSplit.substr(splitIndex + 1);

		while (true) {
			splitIndex = stringToSplit.find(splitSign);
			//����Ҳ����ָ�����˵���������һ�����ָ���ַ����ˣ���b|c���еġ�c��
			if (splitIndex == string::npos) {
				stringSplit.push_back(stringToSplit);
				break;
			}

			//����ҵõ��ָ�����˵�����ٻ�Ҫ�ָ�һ�Σ���b|c��
			stringSplit.push_back(stringToSplit.substr(0, splitIndex - 0));
			stringToSplit = stringToSplit.substr(splitIndex + 1);
		}

		return stringSplit;

	}


	//������ԣ��������ʾ���°����������
	template<class T>
	static void outputTest(T t, string outputPrompt) {
		cout << endl << outputPrompt;
		cout << endl << t;
		cout << endl << outputPrompt << endl;
	}




};


const string Util::CATCH_EXCEPTION = "δ֪����";//�쳣��ʾ
