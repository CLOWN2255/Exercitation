
// pcm-capture-playDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CpcmcaptureplayDlg �Ի���




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


// CpcmcaptureplayDlg ��Ϣ�������

BOOL CpcmcaptureplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpcmcaptureplayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpcmcaptureplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CpcmcaptureplayDlg::OnCapPcm()
{
    CString str;m_save_file_name.GetWindowText(str);
    std::string save_file_name(str);
    if(save_file_name.empty())
        save_file_name = "¼������";
    save_file_name += ".pcm";

    HWAVEIN hWaveIn;  //�����豸
    WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��
    BYTE *pBuffer1;//�ɼ���Ƶʱ�����ݻ���
    WAVEHDR wHdr1; //�ɼ���Ƶʱ�������ݻ���Ľṹ��

    FILE *pf;

    HANDLE          wait;
    waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM
    waveform.nSamplesPerSec = 8000;//�����ʣ�16000��/��
    waveform.wBitsPerSample = 16;//�������أ�16bits/��
    waveform.nChannels = 1;//������������2����
    waveform.nAvgBytesPerSec = 16384;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����
    waveform.nBlockAlign = 2;//һ����Ĵ�С������bit���ֽ�������������
    waveform.cbSize = 0;//һ��Ϊ0

    wait = CreateEvent(NULL, 0, 0, NULL);
    //ʹ��waveInOpen����������Ƶ�ɼ�
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);

    //�����������飨������Խ���������飩����������Ƶ����
    DWORD bufsize = 1024*100;//ÿ�ο���10k�Ļ���洢¼������
    int i = 100;
    fopen_s(&pf, save_file_name.c_str(), "wb");
    while (i--)
    {
        //¼��20�����������������Ƶ��������紫������޸�Ϊʵʱ¼�����ŵĻ�����ʵ�ֶԽ�����
        pBuffer1 = new BYTE[bufsize];
        wHdr1.lpData = (LPSTR)pBuffer1;
        wHdr1.dwBufferLength = bufsize;
        wHdr1.dwBytesRecorded = 0;
        wHdr1.dwUser = 0;
        wHdr1.dwFlags = 0;
        wHdr1.dwLoops = 1;
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//׼��һ���������ݿ�ͷ����¼��
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//ָ���������ݿ�Ϊ¼�����뻺��
        waveInStart(hWaveIn);//��ʼ¼��
        Sleep(80);//�ȴ�����¼��1s
        waveInReset(hWaveIn);//ֹͣ¼��

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
        save_file_name = "¼������";
    save_file_name += ".g711-alaw";

    HWAVEIN hWaveIn;  //�����豸
    WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��
    BYTE *pBuffer1;//�ɼ���Ƶʱ�����ݻ���
    WAVEHDR wHdr1; //�ɼ���Ƶʱ�������ݻ���Ľṹ��

    FILE *pf;


    HANDLE          wait;
    waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM
    waveform.nSamplesPerSec = 8000;//�����ʣ�16000��/��
    waveform.wBitsPerSample = 16;//�������أ�16bits/��
    waveform.nChannels = 1;//������������2����
    waveform.nAvgBytesPerSec = 16000;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����
    waveform.nBlockAlign = 2;//һ����Ĵ�С������bit���ֽ�������������
    waveform.cbSize = 0;//һ��Ϊ0

    wait = CreateEvent(NULL, 0, 0, NULL);
    //ʹ��waveInOpen����������Ƶ�ɼ�
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);

    //�����������飨������Խ���������飩����������Ƶ����
    DWORD bufsize = 1024*100;//ÿ�ο���10k�Ļ���洢¼������
    int i = 100;
    fopen_s(&pf, save_file_name.c_str(), "wb");
    while (i--)
    {
        //¼��20�����������������Ƶ��������紫������޸�Ϊʵʱ¼�����ŵĻ�����ʵ�ֶԽ�����
        pBuffer1 = new BYTE[bufsize];
        wHdr1.lpData = (LPSTR)pBuffer1;
        wHdr1.dwBufferLength = bufsize;
        wHdr1.dwBytesRecorded = 0;
        wHdr1.dwUser = 0;
        wHdr1.dwFlags = 0;
        wHdr1.dwLoops = 1;
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//׼��һ���������ݿ�ͷ����¼��
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//ָ���������ݿ�Ϊ¼�����뻺��
        waveInStart(hWaveIn);//��ʼ¼��
        Sleep(80);//�ȴ�����¼��1s
        waveInReset(hWaveIn);//ֹͣ¼��

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
        AfxMessageBox(_T("�����������ļ�"));
        return;
    }

    int buf_len = 1024 * 1024 * 4;
    char *buf = new char[buf_len];

    FILE*           thbgm;//�ļ�
    int             cnt;
    HWAVEOUT        hwo;
    WAVEHDR         wh;
    WAVEFORMATEX    wfx;
    HANDLE          wait;

    wfx.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ
    wfx.nChannels = 1;//������Ƶ�ļ���ͨ������
    wfx.nSamplesPerSec = 8000;//����ÿ���������źͼ�¼ʱ������Ƶ��
    wfx.nAvgBytesPerSec = 16000;//���������ƽ�����ݴ�����,��λbyte/s�����ֵ���ڴ��������С�Ǻ����õ�
    wfx.nBlockAlign = 2;//���ֽ�Ϊ��λ���ÿ����
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;//������Ϣ�Ĵ�С
    wait = CreateEvent(NULL, 0, 0, NULL);
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//��һ�������Ĳ�����Ƶ���װ�������лط�
    fopen_s(&thbgm, play_file_path.c_str(), "rb");
    cnt = fread(buf, sizeof(char), buf_len, thbgm);//��ȡ�ļ�4M�����ݵ��ڴ������в��ţ�ͨ��������ֵ��޸ģ������߳̿ɱ��������Ƶ���ݵ�ʵʱ���䡣��Ȼ���ϣ��������������Ƶ�ļ���Ҳ��Ҫ��������΢��һ��
    int dolenght = 0;
    int playsize = 640;//��֪��ʲôԭ��һ������1024�ֽڵ�pcm-buffer������������ʱ����whileѭ���˳���ʱ������һ����
    //����˵����¼��10s��pcm����ô���ź�whileѭ���ͻ�����10s��
    //������¼��15s��pcm����ô���ź�whileѭ���ͻ�����15s��
    //Ҳ���������tb-ta��ֵ�Ǹ�pcmʵ��¼��ʱ����ȡ�
    int i=0;
    uint64_t ta = gettime_ms();
    while (cnt>0) 
    {
        //��һ������Ҫ�ر�ע�������ѭ������֮���ܻ�̫����ʱ��ȥ����ȡ����֮��Ĺ�����
        //��Ȼ��ÿ��ѭ���ļ�϶���С����ա�������
        wh.lpData = buf+dolenght;
        wh.dwBufferLength = playsize;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;
        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//׼��һ���������ݿ����ڲ���
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//����Ƶý���в��ŵڶ�������whָ��������
       
		WaitForSingleObject(wait, INFINITE);//�������hHandle�¼����ź�״̬����ĳһ�߳��е��øú���ʱ���߳���ʱ��������ڹ����INFINITE�����ڣ��߳����ȴ��Ķ����Ϊ���ź�״̬����ú�����������
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
        AfxMessageBox("�����������ļ�");
        return;
    }

    int buf_len = 1024 * 1024 * 4;
    unsigned char *buf = new unsigned char[buf_len];

    FILE*           thbgm;//�ļ�
    int             cnt;
    HWAVEOUT        hwo;
    WAVEHDR         wh;
    WAVEFORMATEX    wfx;
    HANDLE          wait;

    wfx.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ
    wfx.nChannels = 1;//������Ƶ�ļ���ͨ������
    wfx.nSamplesPerSec = 8000;//����ÿ���������źͼ�¼ʱ������Ƶ��
    wfx.nAvgBytesPerSec = 16000;//���������ƽ�����ݴ�����,��λbyte/s�����ֵ���ڴ��������С�Ǻ����õ�
    wfx.nBlockAlign = 2;//���ֽ�Ϊ��λ���ÿ����
    wfx.wBitsPerSample = 16;
    wfx.cbSize = 0;//������Ϣ�Ĵ�С
    wait = CreateEvent(NULL, 0, 0, NULL);
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//��һ�������Ĳ�����Ƶ���װ�������лط�
    fopen_s(&thbgm, play_file_path.c_str(), "rb");
    cnt = fread(buf, sizeof(char), buf_len, thbgm);//��ȡ�ļ�4M�����ݵ��ڴ������в��ţ�ͨ��������ֵ��޸ģ������߳̿ɱ��������Ƶ���ݵ�ʵʱ���䡣��Ȼ���ϣ��������������Ƶ�ļ���Ҳ��Ҫ��������΢��һ��
    int dolenght = 0;
    int playsize = 640;//��֪��ʲôԭ��һ������1024�ֽڵ�pcm-buffer������������ʱ����whileѭ���˳���ʱ������һ����
    //����˵����¼��10s��pcm����ô���ź�whileѭ���ͻ�����10s��
    //������¼��15s��pcm����ô���ź�whileѭ���ͻ�����15s��
    //Ҳ���������tb-ta��ֵ�Ǹ�pcmʵ��¼��ʱ����ȡ�
    int i=0;
    uint64_t ta = gettime_ms();
    while (cnt>0) 
    {
        //��һ������Ҫ�ر�ע�������ѭ������֮���ܻ�̫����ʱ��ȥ����ȡ����֮��Ĺ�����
        //��Ȼ��ÿ��ѭ���ļ�϶���С����ա�������
        if(dolenght==0)playsize=320;
        else playsize=320;
        int len = playsize*2;
        char *pcm = new char[len];
        G7112Pcm(buf+dolenght,playsize,pcm,len,0);
        wh.lpData = pcm;
        wh.dwBufferLength = len;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;
        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//׼��һ���������ݿ����ڲ���
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//����Ƶý���в��ŵڶ�������whָ��������
        WaitForSingleObject(wait, INFINITE);//�������hHandle�¼����ź�״̬����ĳһ�߳��е��øú���ʱ���߳���ʱ��������ڹ����INFINITE�����ڣ��߳����ȴ��Ķ����Ϊ���ź�״̬����ú�����������
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
