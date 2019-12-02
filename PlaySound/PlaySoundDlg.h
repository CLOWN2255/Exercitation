
// PlaySoundDlg.h: 头文件
//
#include <windows.h>

#include <mmsystem.h>
#include  <Vfw.h>
#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "vfw32.lib")
#pragma once


// CPlaySoundDlg 对话框
class CPlaySoundDlg : public CDialogEx
{
// 构造
public:
	CPlaySoundDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAYSOUND_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg  LRESULT  OnMCINotify(WPARAM   wParam, LPARAM   lParam);
private:
//	COCX m_player;
	HWND hwndMCI;
	HWND MCIAudio;
	CString filePath;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
private:
	CEdit Path;
	CStatic Time;
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};
