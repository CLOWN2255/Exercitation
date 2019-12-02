
// PlaySoundDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PlaySound.h"
#include "PlaySoundDlg.h"
#include "afxdialogex.h"

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


// CPlaySoundDlg 对话框



CPlaySoundDlg::CPlaySoundDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLAYSOUND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlaySoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Path);
	DDX_Control(pDX, IDC_Time, Time);
}

BEGIN_MESSAGE_MAP(CPlaySoundDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPlaySoundDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPlaySoundDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CPlaySoundDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CPlaySoundDlg::OnBnClickedButton4)
	ON_MESSAGE(MCIWNDM_NOTIFYMODE, OnMCINotify)

	ON_BN_CLICKED(IDC_BUTTON5, &CPlaySoundDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CPlaySoundDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CPlaySoundDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CPlaySoundDlg 消息处理程序

BOOL CPlaySoundDlg::OnInitDialog()
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
	char lpPath[MAX_PATH];
	//获取程序当前路径
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);
	GetPrivateProfileString("SOUND", "FILENAME", NULL, filePath.GetBuffer(MAX_PATH), MAX_PATH, lpPath);
//	filePath = "E:\\音频文件\\my.mp3";
	Path.SetWindowTextA(filePath);
	filePath.ReleaseBuffer();
	CTime m_time;
	CString FileName;
	m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	FileName = m_time.Format(_T("%Y/%m/%d/%H/%M/%S"));
	Time.SetWindowTextA(FileName);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPlaySoundDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlaySoundDlg::OnPaint()
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
HCURSOR CPlaySoundDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPlaySoundDlg::OnBnClickedButton1()
{
	//使用文件路径 播不出原声来
	/*if (PlaySound("C:\Windows\Media\Game of Thrones.wav", NULL, SND_LOOP| SND_FILENAME | SND_ASYNC))
	{
		MessageBox(TEXT("播放成功!"));

	}
	else
	{
		MessageBox(TEXT("播放失败!"));
	}
	sndPlaySound(_T("C:\Windows\Media\Alarm01.wav"), SND_LOOP | SND_ASYNC);
	
	PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_LOOP|SND_RESOURCE | SND_ASYNC);
	*/
	CString music;
	Path.GetWindowTextA(music);
	hwndMCI = NULL;
	hwndMCI = MCIWndCreate(GetSafeHwnd(),
		AfxGetInstanceHandle(),
		WS_CHILD | MCIWNDF_NOMENU | MCIWNDF_NOTIFYMODE, music);
	MCIWndPlay(hwndMCI);
}
LRESULT CPlaySoundDlg::OnMCINotify(WPARAM wParam, LPARAM lParam)
{
	if (lParam == MCI_MODE_STOP)
	{
		MCIWndPlay(hwndMCI);
	}
	return 0;
}

void CPlaySoundDlg::OnBnClickedButton3()
{
	//PlaySound(NULL, NULL, SND_FILENAME);
	MCIWndClose(hwndMCI);
}


void CPlaySoundDlg::OnBnClickedButton2()
{
	MCIWndStop(hwndMCI);
}


void CPlaySoundDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];//存放选择的目录路径
	CString str1, str2;
	
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择所需的目录：");
	bi.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, szPath);
	str2.Format(_T("%s"), szPath);
	filePath = str2 ;//路径+文件名
	char lpPath[MAX_PATH];
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);
	WritePrivateProfileString("SOUND", "FILENAME", filePath, lpPath);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		Path.SetWindowTextA(filePath);
		str1.Format(_T("选择的路径为： %s"), szPath);
		if (MessageBox(str1, _T("路径"), MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL) {

			return;
		}
		else {
			CreateDirectory(filePath, NULL);//创建文件夹
			UpdateData(FALSE);
		}
	}
	else
	{
		AfxMessageBox(_T("无效的目录，请重新选择"));
		return;
	}
	
}


void CPlaySoundDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
    MCIAudio = NULL;
	MCIWndClose(MCIAudio);
	MCIAudio = MCIWndCreate(this->m_hWnd, ::AfxGetApp()->m_hInstance, MCIWNDF_NOPLAYBAR, NULL);
	MCIWndNew(MCIAudio, TEXT("waveaudio"));
	if(MCIWndRecord(MCIAudio))
	   MCIWndRecord(MCIAudio);

}


void CPlaySoundDlg::OnBnClickedButton6()
{

	if (MCIWndCanSave(MCIAudio))
	{

		MCIWndSave(MCIAudio, -1);
		//CopyFile("a", m_Path, FALSE);

	}
	if (MCIWndCanPlay(MCIAudio))
		MCIWndPlay(MCIAudio);
	else
		MessageBox("不能播放");
}


void CPlaySoundDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	MCIWndStop(MCIAudio);
}
