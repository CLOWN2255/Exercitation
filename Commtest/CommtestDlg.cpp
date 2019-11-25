
// CommtestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Commtest.h"
#include "CommtestDlg.h"
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
/*public:
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();*/
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


// CCommtestDlg 对话框

BYTE  CCommtestDlg::receiveBuf[100] = { 0 };
char CCommtestDlg::m_promptStr[200] = { 0 };
CCommtestDlg::CCommtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMMTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, comm.m_comm);
}

BEGIN_MESSAGE_MAP(CCommtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCommtestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCommtestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCommtestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()



void CCommtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommtestDlg::OnPaint()
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
HCURSOR CCommtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BEGIN_EVENTSINK_MAP(CCommtestDlg, CDialogEx)
	ON_EVENT(CCommtestDlg, IDC_MSCOMM1, 1, CCommtestDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()
// CCommtestDlg 消息处理程序

BOOL CCommtestDlg::OnInitDialog()
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
	comm.initMSCOMM();
	int commNum;
	char DevCOMName[6];
	char m_devCOMname[100] = "";
	char lpPath[MAX_PATH];
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);
	commNum = GetPrivateProfileInt("CAMERA_TYPE", "COMM_NUM", 0, lpPath);
	GetPrivateProfileString("CAMERA_TYPE", "COMM_NUM", "", m_devCOMname, 100, lpPath);
	sprintf_s(DevCOMName, "COM%d", commNum);
	CComboBox* m_DevCom = (CComboBox*)GetDlgItem(IDC_COMBO_POS);
	char comm_num = comm.getMSCOMM();
	bool cmp_dev_comname = 0, cmp_printf_comname = 0;
	m_DevCom->SetWindowTextA(m_devCOMname);
	if (comm_num != 0)
	{

		for (int i = 0; i < 16; i++)
		{
			if (comm.comport[i].used == 1)
			{
				m_DevCom->AddString(comm.comport[i].portname);

				if (strcmp(DevCOMName, comm.comport[i].portname) == 0)
				{
					cmp_dev_comname = 1;
				}
			}
		}
	}
	if (cmp_dev_comname == 0)
	{
		MessageBox(TEXT("串口不匹配!"));
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
bool  CCommtestDlg::SetComm()
{
	//读取配置文件信息
	char lpPath[MAX_PATH];
	//获取程序当前路径
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);

	char cameraType[20];
	int commNum;
	int commBaud;
	commNum = GetPrivateProfileInt("CAMERA_TYPE", "COMM_NUM", 0, lpPath);
	char commName[6];
	sprintf_s(commName, "COM%d", commNum);

	commBaud = GetPrivateProfileInt("CAMERA_TYPE", "COMM_BAUD", 0, lpPath);
	if (commBaud == 0)
	{
		commBaud = 115200;
	}
	char commBaudStr[10];
	sprintf_s(commBaudStr, "%d", commBaud);
	char comm_num = comm.getMSCOMM();

	if (comm_num == 0)
	{
		return false;
	}
	else
	{
		int num;
		bool isHaved = false;

		for (int i = 0; i < 32; i++)
		{
			if (comm.comport[i].used == 1)
			{

				if (strcmp(commName, comm.comport[i].portname) == 0)
				{
					num = i + 1;
					isHaved = true;
					break;
				}
				else
				{
					
				}
			}
		}

		if (isHaved)
		{
			comm.openMSCOMM(num , commBaudStr);
			return true;
		}
		else
		{
			return false;
		}
	}
}
//打开串口
void CCommtestDlg::OnBnClickedButton1()
{
	if (SetComm())
	{
		OutputDebugString("发送串口响应！\n");
		comm.sendData("FE AA BB CC", 1);
		SetTimer(4, 500, NULL);
	}
}
void CCommtestDlg::OnCommMscomm1()
{
	Sleep(100);
	comm.onMSCOMM();
	OutputDebugString("收到串口数据\n");
    CommData();
}
void CCommtestDlg::resetReceiveBuffer()
{
	memset(receiveBuf, 0, 100);
	surplusLen = 0;
	bufLen = 0;
}
void CCommtestDlg::CommData()
{
	
	for (int i = 0; i < comm.rxdataStruct.len; i++)
	{
	
		if (bufLen == 0)
		{
			if (comm.rxdataStruct.rxdata[i] == 0XEF)
			{
				memcpy(receiveBuf, comm.rxdataStruct.rxdata + i, 1);
				//memccpy(&receiveBuf,commManager->rxdataStruct.rxdata,,1);
				bufLen++;
			}


		}
		else
		{
			if ((comm.rxdataStruct.rxdata[i] == 0XEF) && (bufLen == 1))
			{
				//resetReceiveBuffer();
			}
			else
			{
				if (bufLen == 1)
				{
					if (comm.rxdataStruct.rxdata[i] == 0XAA)
					{

						memcpy(receiveBuf + bufLen, comm.rxdataStruct.rxdata + i, 1);
						bufLen++;

						surplusLen = 2;
					}
					else
					{

						surplusLen = comm.rxdataStruct.rxdata[i] + 4 - 2;
						memcpy(receiveBuf + bufLen, comm.rxdataStruct.rxdata + i, 1);
						bufLen++;

					}

				}
				else
				{

					memcpy(receiveBuf + bufLen, comm.rxdataStruct.rxdata + i, 1);
					surplusLen--;
					bufLen++;

				}
				if (surplusLen == 0)
				{
					//#ifdef IS_PRINTF_DEBUG_INFO					
					for (int i = 0; i < bufLen; i++)
					{

						//sprintf(m_promptStr, "0x%x ", receiveBuf[i]);

						//OutputDebugString(m_promptStr);


					}
					//sprintf(m_promptStr, "\n", receiveBuf[i]);

					//OutputDebugString(m_promptStr);
					//#endif			
					if (receiveBuf[1] == 0XAA)
					{
						if (receiveBuf[2] == 0XBB && receiveBuf[3] == 0XCC)
						{
							//KillTimer(4);  //收到串口应答 关闭发送
							//Sleep(200);
							//ComData_received = 1;
						//	sendGetCameraModel();
							MessageBox(TEXT("已经建立串口通信"));

						}
						else
						{

						}
						//清空接收buffer
					   resetReceiveBuffer();
					}
					else
					{
					    Protcol();
						OutputDebugString("disposeProtcol();\n");	
					}
				}
			}
		}
	}
	//resetReceiveBuffer();
	memset(comm.rxdataStruct.rxdata, 0, 2048);
	comm.rxdataStruct.handle_ptr = 0;
	comm.rxdataStruct.rx_ptr = 0;
}
void CCommtestDlg::Protcol()
{
	int bufferLen = 0;
	char lpPath[MAX_PATH];
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);
	switch (receiveBuf[2])
	{
	   case 0X0A:
	 {
		BYTE macByte[25];
		int macLen = receiveBuf[1];
		memcpy(macByte, receiveBuf + 3, macLen);
		BYTE a[1] = { 0 };
		BYTE macByte1[13];
		memcpy(macByte1, macByte, 12);
		macByte1[12] = 0;



		BYTE macByte2[13];
		memcpy(macByte2, macByte + 12, 12);
		macByte2[12] = 0;

		memcpy(wirelessMac, macByte2, 13);

		sprintf_s(m_promptStr, "MAC:%s\r\n", wirelessMac);
		MessageBoxA(m_promptStr);
		resetReceiveBuffer();
	 }
	//default :

	}
}
void CCommtestDlg::OnBnClickedButton2()
{
	BYTE data[4] = { 0XFE,0X00,0X09,0X01 };

	unsigned char sumNum = data[0];


	for (int i = 0; i < 2; i++)
	{
		sumNum = sumNum + data[i + 1];
	}

	data[3] = sumNum;

	CString sendStr;

	for (int i = 0; i < 4; i++)
	{

		if (data[i] < 16)
		{
			sendStr.AppendFormat("0%x", data[i]);
		}
		else
		{
			sendStr.AppendFormat("%x", data[i]);
		}

	}
	comm.sendData(sendStr, 1);
	Sleep(100);
}
void CCommtestDlg::ClickedOk()
{
	CString m_devCOMname;
	CString m_printfCOMname;
	CComboBox* m_ComList = (CComboBox*)GetDlgItem(IDC_COMBO_POS);
	int m_DevComnum = m_ComList->GetCurSel();
	char com_pos;
	if (m_DevComnum < 0)
	{
		m_ComList->GetWindowTextA(m_devCOMname);
		com_pos = m_devCOMname.Find("COM");
		if (com_pos >= 0)
		{
			m_devCOMname = m_devCOMname.Mid(3, 2);
		}
	}
	else
	{
		m_ComList->GetLBText(m_DevComnum, m_devCOMname);
		com_pos = m_devCOMname.Find("COM");
		if (com_pos >= 0)
		{
			m_devCOMname = m_devCOMname.Mid(3, 2);
		}
	}
	char lpPath[MAX_PATH];
	GetCurrentDirectory(256, lpPath);
	sprintf_s(lpPath, "%s\\ConfigFile.ini", lpPath);
	if (m_devCOMname != "")
	{
		WritePrivateProfileString("CAMERA_TYPE", "COMM_NUM", m_devCOMname, lpPath);
	}
}

void CCommtestDlg::OnBnClickedButton3()
{
	ClickedOk();
}
