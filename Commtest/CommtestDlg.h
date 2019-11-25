
// CommtestDlg.h: 头文件
//
#include "CommManager.h"
#pragma once


// CCommtestDlg 对话框
class CCommtestDlg : public CDialogEx
{
// 构造
public:
	CCommtestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMTEST_DIALOG };
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
	CommManager comm;
	int bufLen;
	int surplusLen;
	static BYTE receiveBuf[100];
	BYTE wirelessMac[13];
	static char m_promptStr[200];
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	void CommData();
	void resetReceiveBuffer();
	void Protcol();
	void ClickedOk();
public:
	bool SetComm();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
