
// Grammar-ProcessorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Grammar-Processor.h"
#include "Grammar-ProcessorDlg.h"
#include "afxdialogex.h"


#include<bits/stdc++.h>


#include"StateGraph.h"
#include"GrammarProcessor.h"
#include"Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGrammarProcessorDlg 对话框



CGrammarProcessorDlg::CGrammarProcessorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRAMMARPROCESSOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGrammarProcessorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGrammarProcessorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CGrammarProcessorDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CGrammarProcessorDlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CGrammarProcessorDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PRETREATE, &CGrammarProcessorDlg::OnBnClickedButtonPretreate)
	ON_BN_CLICKED(IDC_BUTTON_ELIMINATE, &CGrammarProcessorDlg::OnBnClickedButtonEliminate)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CGrammarProcessorDlg::OnBnClickedButtonExtract)
	ON_BN_CLICKED(IDC_BUTTON_SOLVE, &CGrammarProcessorDlg::OnBnClickedButtonSolve)
	ON_BN_CLICKED(IDC_BUTTON_STATE, &CGrammarProcessorDlg::OnBnClickedButtonState)
END_MESSAGE_MAP()


// CGrammarProcessorDlg 消息处理程序

BOOL CGrammarProcessorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//AllocConsole();//打开测试的输出窗口，
	//freopen("CONOUT$", "w", stdout); //设置可以输出中文

	//try
	//{
	//	GrammarProcessor::processGrammar();
	//}
	//catch (const std::exception&)
	//{
	//	cout << "未知错误";
	//}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGrammarProcessorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGrammarProcessorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGrammarProcessorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGrammarProcessorDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString CFileName;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), /*_T("*.txt")*/NULL, OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		CFileName = opendlg.GetPathName();
		USES_CONVERSION;
		std::string fileName(W2A(CFileName));
		//设置文法文件名
		Intermediary::setGrammarFileName(fileName);
		//执行文法处理
		GrammarProcessor::processGrammar();
		//获得文法输入
		auto inputProductions = Intermediary::getInputProductions();
		auto input = Util::vectorToString(inputProductions, "\r\n");
		CString CInput;
		CInput = input.c_str();
		SetDlgItemText(IDC_EDIT_SHOW, CInput);
		//GetDlgItem(IDC_EDIT_SHOW)->SetWindowText(CInput);

	}
}


void CGrammarProcessorDlg::OnBnClickedButtonProcess()
{
	// TODO: 在此添加控件通知处理程序代码

	string grammarFileName = "grammar.txt";
	SaveFile(IDC_EDIT_SHOW, grammarFileName, "");
	Intermediary::setGrammarFileName(grammarFileName);
	GrammarProcessor::processGrammar();
	string all = getAllString();
	CString CAll;
	CAll = all.c_str();
	CAll.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CAll);

}


bool CGrammarProcessorDlg::SaveFile(int CID, string fileName, string savePath)
{
	// TODO: 在此处添加实现代码.
	CString CSave;
	//获取编辑框内容
	GetDlgItemText(CID, CSave);
	//将“\r\n”替换为“\n”
	CSave.Replace(L"\r\n", L"\n");
	USES_CONVERSION;
	string save(W2A(CSave));

	ofstream ofs;
	ofs.open(savePath + fileName, ios::out);
	ofs << save;
	ofs.close();

	return true;
}


void CGrammarProcessorDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码

	CString CSavePath;
	//打开文件，获取文件路径名
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = L"请选择保存路径";
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, szPath))
		{
			CSavePath = szPath;
			CSavePath = CSavePath + L"\\";
		}

		string fileName = "grammar.txt";
		USES_CONVERSION;
		if (SaveFile(IDC_EDIT_SHOW, fileName, string(W2A(CSavePath)))) {
			MessageBox(L"已保存到" + CSavePath + fileName.c_str());
		}

		//use IMalloc interface for avoiding memory leak  
		IMalloc* pMalloc;
		if (SHGetMalloc(&pMalloc) != NOERROR)
		{
			TRACE(_T("Can't get the IMalloc interface\n"));
		}

		pMalloc->Free(pItemIDList);
		if (pMalloc)
			pMalloc->Release();
		UpdateData(FALSE);
	}

}


BOOL CGrammarProcessorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	//{
	//	// 如果消息是键盘按下事件，且是Esc键，执行以下代码（什么都不做，你可以自己添加想要的代码）
	//	return TRUE;
	//}
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	//{
	//	// 如果消息是键盘按下事件，且是Entert键，执行以下代码（什么都不做，你可以自己添加想要的代码）
	//	return TRUE;
	//}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGrammarProcessorDlg::OnBnClickedButtonPretreate()
{
	// TODO: 在此添加控件通知处理程序代码

	string s = getString(Intermediary::ClassName::GrammarPretreatment);
	CString CS;
	CS = s.c_str();
	CS.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CS);

}


string CGrammarProcessorDlg::getString(Intermediary::ClassName classname)
{
	// TODO: 在此处添加实现代码.

	string endPrompt = "\n------------------------\n";
	vector<string> results;
	StateGraph* state = nullptr;

	switch (classname)
	{
	case Intermediary::ClassName::GrammarPretreatment:
		results.clear();

		results.push_back(Util::vectorToString(Intermediary::getTooShortProductions(),
			"\n", "产生式太短：\n", endPrompt));
		results.push_back(Util::vectorToString(Intermediary::getBeginningErrorProductions(), "\n", "产生式开头错误：\n", endPrompt));
		results.push_back(Util::vectorToString(Intermediary::getPretreatedProductions(), "\n", "合法的产生式：\n", endPrompt));


		results.push_back(Util::vectorToString(Intermediary::getInterminableProductions(), "\n", "不可终止的产生式：\n", endPrompt));

		results.push_back(Util::vectorToString(Intermediary::getUnreachableProductions(), "\n", "不可到达的产生式：\n", endPrompt));

		results.push_back(Intermediary::getProductionsMap(Intermediary::ClassName::UnreachabilityFilter));

		return Util::vectorToString(results, "\n", "化简结果：\n", endPrompt);
		break;
	case Intermediary::ClassName::LeftRecursionEliminator:
		results.clear();

		results.push_back(Intermediary::getProductionsMap(classname));

		return Util::vectorToString(results, "\n", "消除左递归后的结果：\n", endPrompt);

		break;
	case Intermediary::ClassName::LeftCommonFactorExtractor:
		results.clear();

		results.push_back(Intermediary::getProductionsMap(classname));

		return Util::vectorToString(results, "\n", "提取左公因子后的结果：\n", endPrompt);
		break;
	case Intermediary::ClassName::First:
		results.clear();
		results.push_back(Util::mapToString(Intermediary::getGrammar(Intermediary::ClassName::LeftRecursionEliminator).getFirstSetMap(), "：\n", "\n", "所有非终结符的first集合：\n", endPrompt));
	case Intermediary::ClassName::Follow:
		results.push_back(Util::mapToString(Intermediary::getGrammar(Intermediary::ClassName::LeftRecursionEliminator).getFollowSetMap(), "：\n", "\n", "所有非终结符的follow集合：\n", endPrompt));

		return Util::vectorToString(results, "\n", "first&follow：\n", "\n\n");
		break;
	case Intermediary::ClassName::StateGraph:
		results.clear();

		state = GrammarProcessor::getState();
		if (state == nullptr) {
			results.push_back("不是线性文法");
		}
		else {
			string NFA = Util::mapValueToString(state->getNFA(), "\n", "NFA：\n", endPrompt);
			string DFA = Util::vectorToString(state->getDFA(), "\n", "DFA：\n", endPrompt);
			string DFAMinimized = Util::vectorToStringToString(state->getDFAMinimized(), "\n", "最小化DFA：\n", endPrompt);
			results.push_back(NFA);
			results.push_back(DFA);
			results.push_back(DFAMinimized);
		}

		return Util::vectorToString(results, "\n", "线性：\n", "\n\n");
		break;
	default:
		break;
	}

	return string();
}


string CGrammarProcessorDlg::getAllString()
{
	// TODO: 在此处添加实现代码.

	string result;

	result += getString(Intermediary::ClassName::GrammarPretreatment);
	result += getString(Intermediary::ClassName::LeftCommonFactorExtractor);
	result += getString(Intermediary::ClassName::LeftRecursionEliminator);
	result += getString(Intermediary::ClassName::First);
	//result += getString(Intermediary::ClassName::Follow);
	result += getString(Intermediary::ClassName::StateGraph);
	
	return result;
}


void CGrammarProcessorDlg::OnBnClickedButtonEliminate()
{
	// TODO: 在此添加控件通知处理程序代码

	string s = getString(Intermediary::ClassName::LeftRecursionEliminator);
	CString CS;
	CS = s.c_str();
	CS.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CS);

}


void CGrammarProcessorDlg::OnBnClickedButtonExtract()
{
	// TODO: 在此添加控件通知处理程序代码

	string s = getString(Intermediary::ClassName::LeftCommonFactorExtractor);
	CString CS;
	CS = s.c_str();
	CS.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CS);

}


void CGrammarProcessorDlg::OnBnClickedButtonSolve()
{
	// TODO: 在此添加控件通知处理程序代码

	string s = getString(Intermediary::ClassName::First);
	s += getString(Intermediary::ClassName::Follow);
	CString CS;
	CS = s.c_str();
	CS.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CS);

}


void CGrammarProcessorDlg::OnBnClickedButtonState()
{
	// TODO: 在此添加控件通知处理程序代码

	string s = getString(Intermediary::ClassName::StateGraph);
	CString CS;
	CS = s.c_str();
	CS.Replace(L"\n", L"\r\n");
	SetDlgItemText(IDC_EDIT_OUT, CS);

}
