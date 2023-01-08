
// Grammar-ProcessorDlg.h: 头文件
//

#pragma once


#include<bits/stdc++.h>
#include"Intermediary.h"


using namespace std;


// CGrammarProcessorDlg 对话框
class CGrammarProcessorDlg : public CDialogEx
{
// 构造
public:
	CGrammarProcessorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAMMARPROCESSOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonProcess();
	bool SaveFile(int CID, string fileName, string savePath);
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPretreate();
	string getString(Intermediary::ClassName classname);
	string getAllString();
	afx_msg void OnBnClickedButtonEliminate();
	afx_msg void OnBnClickedButtonExtract();
	afx_msg void OnBnClickedButtonSolve();
	afx_msg void OnBnClickedButtonState();
};
