#pragma once
#include<bits/stdc++.h>


#include"GrammarPretreatment.h"
#include"TerminationFilter.h"
#include"UnreachabilityFilter.h"
#include"LeftCommonFactorExtractor.h"
#include"LeftRecursionEliminator.h"
#include"StateGraph.h"
#include"Util.h"


using namespace std;


//总的功能调用的静态类
static class GrammarProcessor {
private:
	static StateGraph* state;
public:
	~GrammarProcessor() {
		delete state;
	}

	static void processGrammar() {
		try
		{

		//文法预处理
		GrammarPretreatment::pretreatGrammar();

		//删除不可终止的产生式
		TerminationFilter::filterTermination();

		//删除不可到达的产生式
		UnreachabilityFilter::filterUnreachability();

		//输出字母表
		Alphabet::printAllSymbol();

		//提取左公因子
		LeftCommonFactorExtractor::extractLeftCommonFactor();

		//左递归
		LeftRecursionEliminator::eliminateLeftRecursion();

		//删除不可到达
		UnreachabilityFilter::setPrevious(2);
		//UnreachabilityFilter::filterUnreachability();

		//first集合follow集合
		Grammar& grammar = LeftRecursionEliminator::getGrammar();
		cout << "\nfirstSetMap\n";
		grammar.printFirstSetMap();
		cout << "\nfollowSetMap\n";
		grammar.printFollowSetMap();

		delete state;
		if (!grammar.isLeftLinear() && !grammar.isRightLinear()) {
			state = nullptr;
		}
		else if (grammar.getProductionsMap().empty()) {
			state = nullptr;
		}
		else {
			state = new StateGraph(grammar);
		}

		}
		catch (const std::exception&)
		{
			cout << Util::CATCH_EXCEPTION;
		}
	}


	static StateGraph* getState() {
		return state;
	}
};


StateGraph* GrammarProcessor::state = nullptr;

map<string, Symbol> Alphabet::symbolMap;//字符的集合
string Alphabet::epsilonLabel = "@";//空字的符号，“@”
map<Symbol, int> Alphabet::copyCountMap;//记录A'、A''等的数量，提取公因子、消除左递归中需要新建符号，因此需要记录数量，避免符号重复

const string Grammar::DEFAULT_START_LABEL = "S";//默认的文法开始符号
const string Grammar::DEFAULT_END_LABEL = "#";//默认的文法结束符号
const string Grammar::productionSplitSymbol = "|";//产生式的分隔符，“|”
Symbol Grammar::end = Alphabet::addSymbol(Grammar::DEFAULT_END_LABEL, Symbol::NONTERMINATOR);//文法的结束符号
Symbol Grammar::start = Alphabet::addSymbol(Grammar::DEFAULT_START_LABEL, Symbol::NONTERMINATOR);//文法的开始符号

vector<string> GrammarPretreatment::inputProductions;//从文件中读入的产生式
vector<string> GrammarPretreatment::tooShortProductions;//产生式太短，产生式长度要求大于3
vector<string> GrammarPretreatment::beginningErrorProductions;//产生式开头格式错误，产生式开头格式应类似“A->”
vector<string> GrammarPretreatment::selfProductions;//自产生式的集合，即“A->A”这样的字符串
vector<string> GrammarPretreatment::pretreatedProductions;//对productionInputs进行预处理后的产生式输入集合
const string GrammarPretreatment::LACK_START_ERROR = "缺少开始符号S！";//缺少开始符号后的出错提示
string GrammarPretreatment::grammarFileName = "grammar.txt";//文法文件的文件名
Grammar GrammarPretreatment::grammar;//预处理后的文法

enum class Intermediary::ClassName;//记录每一个类的名字，在获取每个类的grammar变量时使用

map<Symbol, vector<Production>> LeftCommonFactorExtractor::deducedProductionsMap;//保存代入推导后得到的所有产生式
Grammar LeftCommonFactorExtractor::grammar;//提取左公因子后的文法
Grammar& LeftCommonFactorExtractor::previousGrammar = UnreachabilityFilter::getGrammar();//上一个功能进行后的文法

const string LeftCommonFactorTree::TREE_ROOT_LABEL = "&";//树的根节点的符号代表，可以为&号

Grammar& LeftRecursionEliminator::previousGrammar = LeftCommonFactorExtractor::getGrammar();//上一个功能进行后的文法

Grammar LeftRecursionEliminator::grammar;//消除左递归后的文法

enum class StateGraph::Selection;

vector<Production> TerminationFilter::interminableProductions;//保存不可终止的产生式
vector<string> TerminationFilter::interminableLabels;//保存不可终止的左部的符号
map<Symbol, pair<int, int>> TerminationFilter::statusMap;//检查是否不可终止时，记录检查到了哪一个右部的哪一个符号，或者其状态信息
const pair<int, int> TerminationFilter::TERMINATION_UNCHECKED(-1, -1);//表示该左部还没有被检查是否不可终止
const pair<int, int> TerminationFilter::TERMINABILITY(-2, -2);//表示该左部被检查完了，而且是可以终止的状态
const pair<int, int> TerminationFilter::INTERMINABILITY(-3, -3);//表示该左部被检查完了，是不可终止的
Grammar& TerminationFilter::previousGrammar = GrammarPretreatment::getGrammar();//上一个功能进行后的文法
Grammar TerminationFilter::grammar;//删除了不可终止后的文法

map<Symbol, int> UnreachabilityFilter::recordMap;//记录符号是否可到达
enum UnreachabilityFilter::reachabilityRecord;//在记录符号是否可到达的map集合中，分别表示可到达，不可到达，未检查
vector<vector<Production>> UnreachabilityFilter::unreachableProductions;//保存不可到达的产生式
Grammar UnreachabilityFilter::grammar;//删除不可到达后的文法
Grammar& UnreachabilityFilter::previousGrammar = TerminationFilter::getGrammar();//上一个功能进行后的文法
Grammar& UnreachabilityFilter::previousGrammar2 = LeftRecursionEliminator::getGrammar();//上一个文法
int UnreachabilityFilter::previous = 1;//上一个文法的标志


const string Util::CATCH_EXCEPTION = "未知错误！";//异常提示
