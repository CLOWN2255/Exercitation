
// pcm-capture-playDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pcm-capture-play.h"
#include "pcm-capture-playDlg.h"
#include "G711Codec.h"
#include <string>
#include <windows.h>
#include "time_func.h"

using namespace std;

#ifndef _INC_MMSYSTEM
    #pragma warning(disable: 4201)
    #include <mmsystem.h>
#endif
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CpcmcaptureplayDlg 对话框




CpcmcaptureplayDlg::CpcmcaptureplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpcmcaptureplayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpcmcaptureplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_SAVE_NAME,m_save_file_name);
    DDX_Control(pDX, IDC_EDIT_PLAY_FILE,m_play_file_path);
}

BEGIN_MESSAGE_MAP(CpcmcaptureplayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_CAP_PCM, &CpcmcaptureplayDlg::OnCapPcm)
    ON_BN_CLICKED(IDC_BUTTON_CAP_G711A, &CpcmcaptureplayDlg::OnCapG711a)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_PCM, &CpcmcaptureplayDlg::OnPlayPcm)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_G711A, &CpcmcaptureplayDlg::OnPlayG711a)
END_MESSAGE_MAP()


// CpcmcaptureplayDlg 消息处理程序

BOOL CpcmcaptureplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpcmcaptureplayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpcmcaptureplayDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpcmcaptureplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CpcmcaptureplayDlg::OnCapPcm()
{
    CString str;m_save_file_name.GetWindowText(str);
    std::string save_file_name(str);
    if(save_file_name.empty())
        save_file_name = "录音测试";
    save_file_name += ".pcm";

    HWAVEIN hWaveIn;  //输入设备
    WAVEFORMATEX waveform; //采集音频的格式，结构体
    BYTE *pBuffer1;//采集音频时的数据缓存
    WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体

    FILE *pf;

    HANDLE          wait;
    waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
    waveform.nSamplesPerSec = 8000;//采样率，16000次/秒
    waveform.wBitsPerSample = 16;//采样比特，16bits/次
    waveform.nChannels = 1;//采样声道数，2声道
    waveform.nAvgBytesPerSec = 16384;//每秒的数据率，就是每秒能采集多少字节的数据
    waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数
    waveform.cbSize = 0;//一般为0

    wait = CreateEvent(NULL, 0, 0, NULL);
    //使用waveInOpen函数开启音频采集
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);

    //建立两个数组（这里可以建立多个数组）用来缓冲音频数据
    DWORD bufsize = 1024*100;//每次开辟10k的缓存存储录音数据
    int i = 100;
    fopen_s(&pf, save_file_name.c_str(), "wb");
    while (i--)
    {
        //录制20秒左右声音，结合音频解码和网络传输可以修改为实时录音播放的机制以实现对讲功能
        pBuffer1 = new BYTE[bufsize];
        wHdr1.lpData = (LPSTR)pBuffer1;
        wHdr1.dwBufferLength = bufsize;
        wHdr1.dwBytesRecorded = 0;
        wHdr1.dwUser = 0;
        wHdr1.dwFlags = 0;
        wHdr1.dwLoops = 1;
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//指定波形数据块为录音输入缓存
        waveInStart(hWaveIn);//开始录音
        Sleep(80);//等待声音录制1s
        waveInReset(hWaveIn);//停止录音

        fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);
        delete []pBuffer1;

        printf("%03d  %d\n", i,wHdr1.dwBytesRecorded);
    }
    fclose(pf);

    waveInClose(hWaveIn);
}

void CpcmcaptureplayDlg::OnCapG711a()
{
    CString str;m_save_file_name.GetWindowText(str);
    std::string save_file_name(str);
    if(save_file_name.empty())
        save_file_name = "录音测试";
    save_file_name += ".g711-alaw";

    HWAVEIN hWaveIn;  //输入设备
    WAVEFORMATEX waveform; //采集音频的格式，结构体
    BYTE *pBuffer1;//采集音频时的数据缓存
    WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体

    FILE *pf;


    HANDLE          wait;
    waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
    waveform.nSamplesPerSec = 8000;//采样率，16000次/秒
    waveform.wBitsPerSample = 16;//采样比特，16bits/次
    waveform.nChannels = 1;//采样声道数，2声道
    waveform.nAvgBytesPerSec = 16000;//每秒的数据率，就是每秒能采集多少字节的数据
    waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数
    waveform.cbSize = 0;//一般为0

    wait = CreateEvent(NULL, 0, 0, NULL);
    //使用waveInOpen函数开启音频采集
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);

    //建立两个数组（这里可以建立多个数组）用来缓冲音频数据
    DWORD bufsize = 1024*100;//每次开辟10k的缓存存储录音数据
    int i = 100;
    fopen_s(&pf, save_file_name.c_str(), "wb");
    while (i--)
    {
        //录制20秒左右声音，结合音频解码和网络传输可以修改为实时录音播放的机制以实现对讲功能
        pBuffer1 = new BYTE[bufsize];
        wHdr1.lpData = (LPSTR)pBuffer1;
        wHdr1.dwBufferLength = bufsize;
        wHdr1.dwBytesRecorded = 0;
        wHdr1.dwUser = 0;
        wHdr1.dwFlags = 0;
        wHdr1.dwLoops = 1;
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//指定波形数据块为录音输入缓存
        waveInStart(hWaveIn);//开始录音
        Sleep(80);//等待声音录制1s
        waveInReset(hWaveIn);//停止录音

        unsigned char *g711 = new unsigned char[wHdr1.dwBytesRecorded];
        int g711len = wHdr1.dwBytesRecorded/2;
        Pcm2G711(wHdr1.lpData,wHdr1.dwBytesRecorded,g711,g711len,0);
        fwrite(g711, 1, g711len, pf);
        delete []pBuffer1;
        delete []g711;
        printf("%03d  %d-%d\n", i,wHdr1.dwBytesRecorded,g711len);
    }
    fclose(pf);

    waveInClose(hWaveIn);
}

void CpcmcaptureplayDlg::OnPlayPcm()
{
    CString str;m_play_file_path.GetWindowText(str);
    std::string play_file_path(str);
    if(play_file_path.empty())
    {
        AfxMessageBox(_T("请拖入声音文件"));
        return;
    }

    int buf_len = 1024 * 1024 * 4;
    char *buf = new char[buf_len];

    FILE*           thbgm;//文件
    int             cnt;
    HWAVEOUT        hwo;
    WAVEHDR         wh;
    WAVEFORMATEX    wfx;
    HANDLE          wait;

    wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式
    wfx.nChannels = 1;//设置音频文件的通道数量
    wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率
    wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的
    wfx.nBlockAlign = 2;//以字节为单位设置块对齐
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;//额外信息的大小
    wait = CreateEvent(NULL, 0, 0, NULL);
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//打开一个给定的波形音频输出装置来进行回放
    fopen_s(&thbgm, play_file_path.c_str(), "rb");
    cnt = fread(buf, sizeof(char), buf_len, thbgm);//读取文件4M的数据到内存来进行播放，通过这个部分的修改，增加线程可变成网络音频数据的实时传输。当然如果希望播放完整的音频文件，也是要在这里稍微改一改
    int dolenght = 0;
    int playsize = 640;//不知道什么原因，一次送入1024字节的pcm-buffer，播放器播放时长和while循环退出的时长总是一样。
    //就是说，我录制10s的pcm，那么播放和while循环就会消耗10s。
    //加入我录制15s的pcm，那么播放和while循环就会消耗15s。
    //也就是下面的tb-ta的值是跟pcm实际录制时长相等。
    int i=0;
    uint64_t ta = gettime_ms();
    while (cnt>0) 
    {
        //这一部分需要特别注意的是在循环回来之后不能花太长的时间去做读取数据之类的工作，
        //不然在每个循环的间隙会有“哒哒”的噪音
        wh.lpData = buf+dolenght;
        wh.dwBufferLength = playsize;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;
        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//准备一个波形数据块用于播放
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//在音频媒体中播放第二个函数wh指定的数据
       
		WaitForSingleObject(wait, INFINITE);//用来检测hHandle事件的信号状态，在某一线程中调用该函数时，线程暂时挂起，如果在挂起的INFINITE毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回
        dolenght += playsize;
        cnt -= playsize;

        printf("%03d\n",++i);
    }
    waveOutClose(hwo);
    uint64_t tb = gettime_ms();
    printf("tb-ta=%lld\r\n",tb-ta);
    fclose(thbgm);
}

void CpcmcaptureplayDlg::OnPlayG711a()
{
    CString str;m_play_file_path.GetWindowText(str);
    std::string play_file_path(str);
    if(play_file_path.empty())
    {
        AfxMessageBox("请拖入声音文件");
        return;
    }

    int buf_len = 1024 * 1024 * 4;
    unsigned char *buf = new unsigned char[buf_len];

    FILE*           thbgm;//文件
    int             cnt;
    HWAVEOUT        hwo;
    WAVEHDR         wh;
    WAVEFORMATEX    wfx;
    HANDLE          wait;

    wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式
    wfx.nChannels = 1;//设置音频文件的通道数量
    wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率
    wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的
    wfx.nBlockAlign = 2;//以字节为单位设置块对齐
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;//额外信息的大小
    wait = CreateEvent(NULL, 0, 0, NULL);
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//打开一个给定的波形音频输出装置来进行回放
    fopen_s(&thbgm, play_file_path.c_str(), "rb");
    cnt = fread(buf, sizeof(char), buf_len, thbgm);//读取文件4M的数据到内存来进行播放，通过这个部分的修改，增加线程可变成网络音频数据的实时传输。当然如果希望播放完整的音频文件，也是要在这里稍微改一改
    int dolenght = 0;
    int playsize = 640;//不知道什么原因，一次送入1024字节的pcm-buffer，播放器播放时长和while循环退出的时长总是一样。
    //就是说，我录制10s的pcm，那么播放和while循环就会消耗10s。
    //加入我录制15s的pcm，那么播放和while循环就会消耗15s。
    //也就是下面的tb-ta的值是跟pcm实际录制时长相等。
    int i=0;
    uint64_t ta = gettime_ms();
    while (cnt>0) 
    {
        //这一部分需要特别注意的是在循环回来之后不能花太长的时间去做读取数据之类的工作，
        //不然在每个循环的间隙会有“哒哒”的噪音
        if(dolenght==0)playsize=320;
        else playsize=320;
        int len = playsize*2;
        char *pcm = new char[len];
        G7112Pcm(buf+dolenght,playsize,pcm,len,0);
        wh.lpData = pcm;
        wh.dwBufferLength = len;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;
        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//准备一个波形数据块用于播放
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//在音频媒体中播放第二个函数wh指定的数据
        WaitForSingleObject(wait, INFINITE);//用来检测hHandle事件的信号状态，在某一线程中调用该函数时，线程暂时挂起，如果在挂起的INFINITE毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回
        dolenght += playsize;
        cnt -= playsize;

        delete []pcm;
        printf("%d %03d\n",len,++i);
    }
    waveOutClose(hwo);
    uint64_t tb = gettime_ms();
    printf("tb-ta=%lld\r\n",tb-ta);
    fclose(thbgm);
}
