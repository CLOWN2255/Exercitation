
// pcm-capture-playDlg.h : ͷ�ļ�
//

#pragma once
#include "DropEdit.h"

// CpcmcaptureplayDlg �Ի���
class CpcmcaptureplayDlg : public CDialog
{
// ����
public:
	CpcmcaptureplayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PCMCAPTUREPLAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
    //afx_msg void OnDropFiles(HDROP hDropInfo);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
