#pragma once


// Login 对话框
#include "InfoFile.h"
class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Login();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit edit1;
	CEdit edit2;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	bool Http_Post(CString ,CString, int);
private:
	CStatic Pic;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
