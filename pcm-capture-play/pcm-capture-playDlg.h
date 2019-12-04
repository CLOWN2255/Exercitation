
// pcm-capture-playDlg.h : 头文件
//

#pragma once
#include "DropEdit.h"

// CpcmcaptureplayDlg 对话框
class CpcmcaptureplayDlg : public CDialog
{
// 构造
public:
	CpcmcaptureplayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PCMCAPTUREPLAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
    //afx_msg void OnDropFiles(HDROP hDropInfo);

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
    afx_msg void OnCapPcm();
    afx_msg void OnCapG711a();
    afx_msg void OnPlayPcm();
    afx_msg void OnPlayG711a();

private:
    CEdit m_save_file_name;

    CDropEdit m_play_file_path;
public:
};
