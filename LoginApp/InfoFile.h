#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include "framework.h"
#include "resource.h"
#define _F_LOGIN "./login.ini"


using namespace std;


class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//��ȡ��¼��Ϣ
	void ReadLogin(CString &name, CString &pwd);
	//�޸�����
	void WritePwd(char* name, char* pwd);
	
};

