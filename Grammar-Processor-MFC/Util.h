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
	//map��valueתstring
	template<class T, class K>
	static string mapValueToString(map<T, K>& m, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string split = "";
		for (auto it : m) {
			//if (isPointer(it.second)) {
			//	result += split + string(*(it.second));
			//}
			//else {
			//	result += split + string(it.second);
			//}
			K k = it.second;
			result += split + coutToString(k);
			//result += split + it.second.toString();
			split = splitSign;
		}
		result += endPrompt;
		return result;
	}
	//map��valueתstring��value��ָ��
	template<class T, class K>
	static string mapValueToString(map<T, K*>& m, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string split = "";
		for (auto it : m) {
			//if (isPointer(it.second)) {
			//	result += split + string(*(it.second));
			//}
			//else {
			//	result += split + string(it.second);
			//}
			result += split + it.second->toString();
			split = splitSign;
		}
		result += endPrompt;
		return result;
	}
	//mapתstring
	template<class T, class K>
	static string mapToString(map<T, K>& m, string mapSign, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string split = "";
		for (auto it : m) {
			T t = it.first;
			K k = it.second;
			result += split + t.toString() + mapSign + k.toString();
			split = splitSign;
		}
		result += endPrompt;
		return result;
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
	//��vectorתΪ�ַ������÷ָ����ָ���
	//�������ÿ�ͷ�ַ����Լ���β�ַ���
	template<class T>
	static string vectorToString(vector<T>& vec, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string splitSymbol = "";
		for (auto v : vec) {
			result += splitSymbol + string(v);
			splitSymbol = splitSign;
		}
		result += endPrompt;
		return result;
	}
	//vectorתstring��Ԫ����ָ��
	template<class T>
	static string vectorToString(vector<T*>& vec, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string splitSymbol = "";
		for (auto v : vec) {
			result += splitSymbol + string(*v);
			splitSymbol = splitSign;
		}
		result += endPrompt;
		return result;
	}
	//vectorתstring��Ԫ����ָ��
	template<class T>
	static string vectorToStringToString(vector<T*>& vec, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string splitSymbol = "";
		for (auto v : vec) {
			result += splitSymbol + v->toString();
			splitSymbol = splitSign;
		}
		result += endPrompt;
		return result;
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
	//setתstring
	template<class T>
	static string setToString(set<T>& se, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string split = "";
		for (auto s : se) {
			//if (isPointer(s)) {
			//	result += split + string(*s);
			//}
			//else {
			//	result += split + string(s);
			//}
			result += split + string(s);
			split = splitSign;
		}
		result += endPrompt;
		return result;
	}
	//setתstring��Ԫ����ָ��
	template<class T>
	static string setToString(set<T*>& se, string splitSign, string startPrompt = "", string endPrompt = "") {
		string result = startPrompt;
		string split = "";
		for (auto s : se) {
			//if (isPointer(s)) {
			//	result += split + string(*s);
			//}
			//else {
			//	result += split + string(s);
			//}
			result += split + string(*s)
			split = splitSign;
		}
		result += endPrompt;
		return result;
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


	//�ж��Ƿ�ָ������
	template<class T>
	static bool isPointer(T* p) {
		return true;
	}
	//�ж��Ƿ�ָ������
	static bool isPointer(...) {
		return false;
	}


	//coutתstring
	template<class T>
	static string coutToString(T& t) {

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT�Ļ�����
		cout.rdbuf(ss.rdbuf());
		cout << t;
		// ss.str("");   // �������ss������
		string s(ss.str());
		cout.rdbuf(buffer); // ���ã���������STDOUT�Ļ�����

		return s;
	}
};


