
#include <winsock2.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
using namespace std;
class UrlLib
{
public:
	UrlLib(string domainName, int port);
	string getHtml();
	string UtfToGbk(const char* utf8);
	void POST(string source, string data);
	void POSTJson(string source, string data);
	//void POSTJson2(string source, string data);
	void GET(string source);
	void GETJson(string source);
	string getCookie(string header);
	string getJsonStr(string body);
	void Init();
private:
	string DomainName;
	string SourcePos;
	string Cookie;
	string BodyJson;
	int Port;
	SOCKET Socket;
	void setSource(string source);
};

