// Login.cpp: 实现文件
//

#include "pch.h"
#include "LoginApp.h"
#include "Login.h"
#include "afxdialogex.h"
#include "UrlLib.h"

// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit1);
	DDX_Control(pDX, IDC_EDIT2, edit2);
	DDX_Control(pDX, IDC_PIC, Pic);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Login::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Login::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Login 消息处理程序
bool Login::Http_Post(CString Detail,  CString Host, int Port)
{
	string html="";
	string host = LPCSTR(Host);
	string Det = LPCSTR(Detail);
	UrlLib Lib(host, Port);
    Lib.POSTJson("/authenticate", Det);
    //Lib.getHtml(html);
	html = Lib.getHtml();
	if (html == "")
	{
		return false;
	}
	string jsonresult = Lib.getJsonStr(html);
	if (jsonresult == "{\"resultMessage\":\"200\"}")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Login::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString name_str;
	CString password_str;
	CString Host = TEXT("52.80.241.121");
	int Port = 327;
	edit1.GetWindowText(name_str);
	edit2.GetWindowText(password_str);
	//\"username\": \"1\", \"password\": \"123456\", \"appVersion\": 99999
	string URLDetail = "{\"username\":\"" + name_str + "\",\"password\":\"" + password_str + "\",\"appVersion\":9999}";
	//string PostURLDetail = "{ \"mac\": \"" + string_Mac + "\", \"product_model\": \"" + string_ProductModel + "\", \"firmware_version\": \"" + string_FirmwareVersion + "\", \"is_passed\": " + ispass + ", \"username\": \"" + WifiEssid + "\", \"test_content\": \"{\\\"qxdL\\\":\\\"" + qingxiduLeft + "\\\",\\\"qxdC\\\":\\\"" + qingxiduCenter + "\\\",\\\"qxdR\\\":\\\"" + qingxiduRight + "\\\",\\\"wifiDB\\\":\\\"" + to_string(wifiDBvalue) + "\\\",\\\"laohua\\\":\\\"" + Laohuavalue + "\\\"}\"}";
	//CString name,password;
	//CInfoFile file;
	//file.ReadLogin(name, password);
	CString Detail = URLDetail.c_str();
	if (Http_Post(Detail,Host, Port))
	{
		CDialogEx::OnCancel();
	}
	else
	{
		MessageBox(TEXT("用户或密码有错误!"));
	}
	/*if ( name== name_str && password == password_str)
	{
		CDialogEx::OnCancel();
	}
	else
	{
		MessageBox(TEXT("用户或密码有错误!"));
	}*/
}


void Login::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


void Login::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    //CDialogEx::OnClose();
	exit(0);
}


BOOL Login::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Pic.ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//设置句柄
#define HBMP(filepath,width,height)  (HBITMAP)LoadImage(AfxGetInstanceHandle(),filepath,IMAGE_BITMAP,width,height,LR_LOADFROMFILE|LR_CREATEDIBSECTION)
	CRect rect;
	Pic.GetWindowRect(rect);
	Pic.SetBitmap(HBMP(TEXT("cat.bmp"), rect.Width(), rect.Height()));
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Login::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
