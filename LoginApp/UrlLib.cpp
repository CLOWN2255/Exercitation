#include "pch.h"
#include "UrlLib.h"
using namespace std;

UrlLib::UrlLib(string domainName, int port)
{
	this->Cookie = "";
	WSADATA WSAData;
	WSAStartup(0x202, &WSAData);
	this->DomainName = domainName;
	this->Port = port;
	this->Init();
}

void UrlLib::Init()
{
	//try
	//{
	//	this->Socket = socket(AF_INET, SOCK_STREAM, 0);
	//	hostent* host = gethostbyname(this->DomainName.c_str());
	//	if (host == NULL)
	//	{
	//		return;
	//	}
	//	sockaddr_in saddr;
	//	saddr.sin_family = AF_INET;
	//	saddr.sin_port = htons(this->Port);
	//	memcpy(&saddr.sin_addr, host->h_addr, 4);
	//	if (connect(this->Socket, (sockaddr*)& saddr, sizeof(saddr)) == -1)
	//	{
	//		cout << "connect error" << endl;
	//	}
	//	else
	//	{
	//		cout << "connect succeed" << endl;
	//	}
	//	int timeout = 3000; //3s                                               //设置发送数据和接收数据的最大延迟时间
	//	setsockopt(this->Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)& timeout, sizeof(timeout));
	//	setsockopt(this->Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)& timeout, sizeof(timeout));
	//}
	//catch (const std::exception& e)
	//{
	//	cout << e.what() << endl;
	//	string ex = e.what();
	//	cout << "connect error" << endl;
	//}

	try
	{

		this->Socket = socket(AF_INET, SOCK_STREAM, 0);
		hostent* host = gethostbyname(this->DomainName.c_str());
		if (host == NULL)
		{
			return;
		}
		sockaddr_in saddr;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(this->Port);
		memcpy(&saddr.sin_addr, host->h_addr, 4);

		//设置Socket为非阻塞模式
		int iMode = 1;
		int retVal;
		retVal = ioctlsocket(Socket, FIONBIO, (u_long FAR*) & iMode);

		char sendbuf[100];
		while (true)
		{
			int ret = connect(this->Socket, (sockaddr*)& saddr, sizeof(saddr));
			//connect(Socket, (SOCKADDR*)& saddr, sizeof(saddr));
			if (ret == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK)
				{
					Sleep(100);
					continue;
				}
				else if (err == WSAENETDOWN)
				{
					closesocket(Socket);
					break;
				}
			}break;
		}
		
		Sleep(100);

		iMode = 0;
		ioctlsocket(Socket, FIONBIO, (u_long FAR*) & iMode);
		int timeout = 2000; //2s                                               
		//设置发送数据和接收数据的最大延迟时间
		setsockopt(this->Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)& timeout, sizeof(timeout));
		setsockopt(this->Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)& timeout, sizeof(timeout));
		
		
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		string ex = e.what();
		cout << "connect error" << endl;
	}
}


void UrlLib::setSource(string source)
{
	this->SourcePos = source;
}
string UrlLib::getHtml()
{
	string html;
	char buf[1024];
	int n;
	int sum = 0;
	cout << "正在获取html" << endl;
	while ((n = recv(this->Socket, buf, sizeof(buf) - sizeof(char), 0)) != 0)
	{
		cout << n << "字节" << endl;
		for (int i = 0; i < n; i++)
		{
			html += buf[i];
		}
		sum++;
		if (sum == 10)
			break;
	}

	html = UtfToGbk(html.c_str());
	closesocket(this->Socket);
	return html;
}

string UrlLib::UtfToGbk(const char* utf8)   //由于windowscmd窗口支持的编码格式为gbk而网站默认的编码格式为utf8 所以要转换编码
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

void UrlLib::POST(string source, string data)
{
	this->Init();
	this->setSource(source);
	stringstream request;
	request << "POST " << this->SourcePos << " HTTP/1.1\r\n";
	request << "Host:" << this->DomainName << "\r\n";
	request << "Content-Length:" << data.length() << "\r\n";//+ "\r\n";
	//request << "Connection:close\r\n";
	request << "Connection:close\r\n";
	if (this->Cookie != "")
		request << this->Cookie + "\r\n";
	request << "Content-Type:application/x-www-form-urlencoded\r\n";
	//request <<"Referer:http://acm.sdut.edu.cn/onlinejudge2/index.php/Home/Login/login"<<"\r\n";
	request << "\r\n";
	request << data;
	string Post = request.str();
	cout << Post << endl;
	send(this->Socket, Post.c_str(), Post.size(), 0);
}


void UrlLib::POSTJson(string source, string data)
{
	this->Init();
	this->setSource(source);
	stringstream request;
	request << "POST " << this->SourcePos << " HTTP/1.1\r\n";
	request << "Host:" << this->DomainName << "\r\n";
	request << "Content-Length:" << data.length() << "\r\n";//+ "\r\n";
	//request << "Connection:close\r\n";
	request << "Connection:close\r\n";
	if (this->Cookie != "")
		request << this->Cookie + "\r\n";
	request << "Content-Type:application/json\r\n";
	//request <<"Referer:http://acm.sdut.edu.cn/onlinejudge2/index.php/Home/Login/login"<<"\r\n";
	request << "\r\n";

	request << data;
	string Post = request.str();
	cout << Post << endl;
	send(this->Socket, Post.c_str(), Post.size(), 0);
}



void UrlLib::GET(string source)
{
	this->Init();
	//cout<<this->Cookie<<endl;
	this->setSource(source);
	string request = "GET " + this->SourcePos + " HTTP/1.1\r\n";
	request += "Host:" + this->DomainName + "\r\n";
	request += "connection:close\r\n";
	request += this->Cookie + "\r\n";

	request += "\r\n";
	cout << request << endl;
	if (send(this->Socket, request.c_str(), request.size(), 0) > 0)
	{
		cout << "send succeed" << endl;
	}
	else
	{
		cout << "send error" << endl;

	}
}

void UrlLib::GETJson(string source)
{
	this->Init();
	//cout<<this->Cookie<<endl;
	this->setSource(source);
	string request = "GET " + this->SourcePos + " HTTP/1.1\r\n";
	request += "Host:" + this->DomainName + "\r\n";
	request += "connection:close\r\n";
	request += this->Cookie + "\r\n";
	request += "Content-Type:application/json\r\n";

	request += "\r\n";
	cout << request << endl;
	if (send(this->Socket, request.c_str(), request.size(), 0) > 0)
	{
		cout << "send succeed" << endl;
	}
	else
	{
		cout << "send error" << endl;

	}
}


string UrlLib::getCookie(string header)
{
	int begin = header.find("Cookie:");
	int end = header.find("path", begin);
	int len = end - begin;
	string cookie = header.substr(begin, len);
	this->Cookie = cookie;
	return this->Cookie;
}

string UrlLib::getJsonStr(string body)
{
	int begin = body.find("{");
	int end = body.find_last_of("}");
	int len = end - begin + 1;
	string JsonStr = body.substr(begin, len);
	this->BodyJson = JsonStr;
	return this->BodyJson;
}
