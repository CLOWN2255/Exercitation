
// pcm-G711Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "pcm-G711.h"
#include "pcm-G711Dlg.h"
#include "afxdialogex.h"
#include<iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CpcmG711Dlg 对话框



CpcmG711Dlg::CpcmG711Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PCMG711_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpcmG711Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit);
	DDX_Control(pDX, IDC_EDIT2, edit7);
}

BEGIN_MESSAGE_MAP(CpcmG711Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CpcmG711Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CpcmG711Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CpcmG711Dlg 消息处理程序

BOOL CpcmG711Dlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpcmG711Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpcmG711Dlg::OnPaint()
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
HCURSOR CpcmG711Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//PCM->G711
#define MAX 32635
unsigned char encode(short pcm)
{
	int sign = (pcm & 0x8000) >> 8;
	if (sign != 0)
		pcm = -pcm;
	if (pcm > MAX) pcm = MAX;
	int exponent = 7;
	int expMask;
	for (expMask = 0x4000; (pcm & expMask) == 0
		&& exponent > 0; exponent--, expMask >>= 1) {
	}
	int mantissa = (pcm >> ((exponent == 0) ? 4 : (exponent + 3))) & 0x0f;
	unsigned char alaw = (unsigned char)(sign | exponent << 4 | mantissa);
	return (unsigned char)(alaw ^ 0xD5);
}

int g711_encode(unsigned char* pCodecBits, const char* pBuffer, int nBufferSize)
{
	short* buffer = (short*)pBuffer;
	int i;
	for (i = 0; i < nBufferSize / 2; i++)
	{
		pCodecBits[i] = encode(buffer[i]);
	}
	int Size = nBufferSize/2;
	return Size;
}
//G711->PCM
short decode(unsigned char alaw)
{
	alaw ^= 0xD5;
	int sign = alaw & 0x80;
	int exponent = (alaw & 0x70) >> 4;
	int data = alaw & 0x0f;
	data <<= 4;
	data += 8;
	if (exponent != 0)
		data += 0x100;
	if (exponent > 1)
		data <<= (exponent - 1);
	return (short)(sign == 0 ? data : -data);
}
int g711_decode(short* pRawData, const unsigned char* pBuffer, int nBufferSize)
{
	short* out_data =(short*) pRawData;
 	//short* out_data = new short[nBufferSize];
	int i;
	for (i = 0; i < nBufferSize; i++)
	{
		out_data[i] =  decode(pBuffer[i]);	
	}
//	memcpy(pRawData, out_data, nBufferSize);
	return nBufferSize * 2;
}
//PCM->G711 转化按钮
void CpcmG711Dlg::OnBnClickedButton1()
{
	// pcm 
	CString str;
	Edit.GetWindowText(str);
	std::string play_file_path(str);
	if (play_file_path.empty())
	{
		AfxMessageBox(_T("请拖入声音文件"));
		return;
	}
	FILE* thbgm;//文件
	fopen_s(&thbgm, play_file_path.c_str(), "rb");
	fseek(thbgm, 0L, SEEK_END);
	int buf_len = ftell(thbgm);
	char* buf = new char[buf_len];
	fseek(thbgm, 0L, SEEK_SET);
	//g711
	CString str7;
	edit7.GetWindowText(str7);
	std::string play_file_path7(str7);
	if (play_file_path7.empty())
	{
		AfxMessageBox(_T("请拖入声音文件"));
		return;
	}
	FILE* thbgm7;//文件
	fopen_s(&thbgm7, play_file_path7.c_str(), "wb");
	//pcm->g711
	int len;
    if(len = fread(buf, buf_len, 1, thbgm) > 0)
	{
		CString str;
		str.Format("%d", buf_len);
		unsigned char *g711Buffer=new unsigned char[buf_len];
		int  g711BufSize = g711_encode(g711Buffer, buf, buf_len);
		fwrite(g711Buffer, g711BufSize, 1, thbgm7);
		MessageBox("转化成功！");
	}
	fclose(thbgm);
	fclose(thbgm7);
}

//G711->PCM 按钮
void CpcmG711Dlg::OnBnClickedButton2()
{
	CString str;
	Edit.GetWindowText(str);
	std::string play_file_path(str);
	if (play_file_path.empty())
	{
		AfxMessageBox(_T("请拖入声音文件"));
		return;
	}
	FILE* thbgm;//文件
	fopen_s(&thbgm, play_file_path.c_str(), "wb");
	//g711
	CString str7;
	edit7.GetWindowText(str7);
	std::string play_file_path7(str7);
	if (play_file_path7.empty())
	{
		AfxMessageBox(_T("请拖入声音文件"));
		return;
	}
	FILE* thbgm7;//文件
	fopen_s(&thbgm7, play_file_path7.c_str(), "rb");
	fseek(thbgm7, 0L, SEEK_END);
	int buf_len = ftell(thbgm7);
	unsigned char* buf = new unsigned char[buf_len];
	fseek(thbgm7, 0L, SEEK_SET);
	//pcm->g711
	int len;
	if (len = fread(buf, buf_len, 1, thbgm7) > 0)
	{
		CString str;
		str.Format("%d", buf_len);
		// char* g711Buffer = new  char[buf_len];
		short* g711Buffer = new short[buf_len];
		int  g711BufSize = g711_decode(g711Buffer, buf, buf_len);
		fwrite(g711Buffer, g711BufSize, 1, thbgm);
		MessageBox("转化成功！");
	}
	fclose(thbgm);
	fclose(thbgm7);
}
