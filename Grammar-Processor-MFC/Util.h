#pragma once
#include<bits/stdc++.h>


using namespace std;



//工具类，静态类
static class Util {
private:

public:
	const static bool RETAIN = true;//保留，过滤字符函数要用到

	const static bool NOT_RETAIN = false;//不保留，过滤字符函数要用到

	const static string CATCH_EXCEPTION;//异常提示


	//map集合工具
	//输出map集合的值，一个值一行
	template<class T, class K>
	static void printMapValueInLines(map<T, K>& map) {
		for (auto& it : map) {
			cout << it.second << endl;
		}
	}
	//格式化输出map集合
	template<class T, class K>
	static void printMap(map<T, K>& map) {
		cout << "{\n";
		for (auto& it : map) {
			cout << "    " << it.first << " => " << it.second << endl;
		}
		cout << "}\n";
	}
	//将map集合中值转为vector后返回
	template<class T, class K>
	static vector<K> getMapValueAsVector(map<T, K>& map) {
		vector<K> values;
		for (auto& it : map) {
			values.push_back(it.second);
		}
		return values;
	}
	//获得map集合中键对应的值
	//如果不存在该键，抛出异常
	template<class T, class K>
	static K& getMapValue(map<T, K>& mapToGet, T& key) {
		//如果找不到该键，抛出异常
		if (mapToGet.find(key) == mapToGet.end()) {
			throw exception();
		}
		else {
			return mapToGet[key];
		}
	}
	//向map集合中插入初值
	//如果已存在就不插入
	template<class T, class K>
	static void addInitialValueToMap(map<T, K>& mapToAdd, const T& key, const K& value) {
		if (mapToAdd.find(key) == mapToAdd.end()) {
			mapToAdd.insert(pair<T, K>(key, value));
		}
	}
	//map的value转string
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
	//map的value转string，value是指针
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
	//map转string
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


	//vector工具
	//输出vector中的值，一个值一行
	template<class T>
	static void printVectorInLines(vector<T>& vec) {
		for (auto& it : vec) {
			cout << it << endl;
		}
	}
	//输出vector中的值，在一行中输出，以分隔符分隔开，并用输出提示上下包裹
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
	//将vector转为字符串，用分隔符分隔开
	//可以设置开头字符串以及结尾字符串
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
	//vector转string，元素是指针
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
	//vector转string，元素是指针
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



	//set工具
	template<class T>
	static void printSet(set<T>& setToPrint) {
		cout << "{";
		for (auto i : setToPrint) {
			cout << i << ", ";
		}
		cout << "}";
	}
	//set转string
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
	//set转string，元素是指针
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


	//字符串处理工具
	//过滤字符
	//如果retain为true，则保留characters中的内容
	static string filterCharacter(string aString, const vector<char>& characters, bool retain) {
		string result = "";
		string characterString(characters.begin(), characters.end());//将字符vector变成string
		for (const auto& c : aString) {
			//如果字符不在要过滤的字符序列中，则可以添加到结果字符串中
			//retain加在这里，可以实现一个switch的结构，而不用写switch
			if (retain && characterString.find(c) != string::npos) {
				result.push_back(c);
			}
			if (!retain && characterString.find(c) == string::npos) {
				result.push_back(c);
			}
		}
		return result;
	}
	//将字符串按照某个字符串进行分割成集合
	static vector<string> splitString(string stringToSplit, string splitSign) {
		int splitIndex = stringToSplit.find(splitSign);

		//如果等于npos，说明找不到分隔符，直接返回
		//npos是string中预定义的
		if (splitIndex == string::npos) {
			return vector<string>({ stringToSplit });
		}

		vector<string> stringSplit;//分割后的字符串集合
		//分隔符第一次的下标 减 零 就是要截取的第一个字符串的长度
		stringSplit.push_back(stringToSplit.substr(0, splitIndex - 0));
		//“a|b|c”，变为“b|c”
		stringToSplit = stringToSplit.substr(splitIndex + 1);

		while (true) {
			splitIndex = stringToSplit.find(splitSign);
			//如果找不到分隔符，说明到达最后一个待分割的字符串了，“b|c”中的“c”
			if (splitIndex == string::npos) {
				stringSplit.push_back(stringToSplit);
				break;
			}

			//如果找得到分隔符，说明最少还要分割一次，“b|c”
			stringSplit.push_back(stringToSplit.substr(0, splitIndex - 0));
			stringToSplit = stringToSplit.substr(splitIndex + 1);
		}

		return stringSplit;

	}


	//输出测试，用输出提示上下包裹输出内容
	template<class T>
	static void outputTest(T t, string outputPrompt) {
		cout << endl << outputPrompt;
		cout << endl << t;
		cout << endl << outputPrompt << endl;
	}


	//判断是否指针类型
	template<class T>
	static bool isPointer(T* p) {
		return true;
	}
	//判断是否指针类型
	static bool isPointer(...) {
		return false;
	}


	//cout转string
	template<class T>
	static string coutToString(T& t) {

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << t;
		// ss.str("");   // 可以清空ss的内容
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区

		return s;
	}
};


