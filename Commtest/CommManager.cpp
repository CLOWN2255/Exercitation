#include "pch.h"
#include "CommManager.h"

CommManager::CommManager(void)
{
}


CommManager::~CommManager(void)
{
}


void CommManager::initMSCOMM()
{
	commStatus = false;
	memset(rxdataStruct.rxdata,0,2048);
	rxdataStruct.handle_ptr=0;
	rxdataStruct.rx_ptr=0;
	memset(comport,0,sizeof(comport));
}

unsigned char CommManager::getMSCOMM()
{
	HANDLE  hCom;
    char i,num,k;
	CString str;
	BOOL flag;

	flag = FALSE;
	num = 0;
	for (i = 1;i <= 32;i++)
	{//此程序支持16个串口
		str.Format("\\\\.\\COM%d",i);
		hCom = CreateFile(str, 0, 0, 0, 
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(INVALID_HANDLE_VALUE != hCom )
		{//能打开该串口，则添加该串口
			CloseHandle(hCom);
			sprintf_s(comport[i-1].portname,"COM%d",i);
			comport[i-1].used=1;
			if (flag == FALSE)
			{
				flag = TRUE;
				num = i;
			}
		}
	}

	return num;
}

void CommManager::openMSCOMM(int portnum,char* portbaund)
{
	char config[15];
	//char *config;
	sprintf_s(config,"%s,n,8,1",portbaund);

	if (commStatus)
	{
		m_comm.put_PortOpen(FALSE);
		commStatus=false;
	}
	else
	{
		if (!m_comm.get_PortOpen())
		{
			m_comm.put_Settings(config);
			m_comm.put__CommPort(portnum);
		    m_comm.put_PortOpen(TRUE);

		}
			//m_comm.SetCommPort(portnum);
		
		//1：表示以二进制方式检取数据
		m_comm.put_InputMode(1);
		//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
		m_comm.put_RThreshold(1);
		//设置当前接收区数据长度为0
		m_comm.put_InputLen(0);
		//先预读缓冲区以清除残留数据
		m_comm.get_Input();
		commStatus = true;
	}
}


void CommManager::onMSCOMM()
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	CString strtemp;
	//事件值为2表示接收缓冲区内有字符
	if (m_comm.get_CommEvent()==2)
	{
		//读缓冲区
		variant_inp = m_comm.get_Input();
		//VARIANT型变量转换为ColeSafeArray型变量
		safearray_inp = variant_inp;
		//得到有效数据长度
		len = safearray_inp.GetOneDimSize();
		char tempstr[100];
		sprintf_s(tempstr,"com receive length:%d\r\n",len);
		OutputDebugString(tempstr);
		for(k=0;k<len;k++)
		{
			if (rxdataStruct.rx_ptr+k<2048)
			{
				safearray_inp.GetElement(&k,rxdataStruct.rxdata+rxdataStruct.rx_ptr+k);//转换为BYTE型数组
			}
			else
			{
				if (rxdataStruct.rx_ptr+k<2048)
				{
					safearray_inp.GetElement(&k,rxdataStruct.rxdata+rxdataStruct.rx_ptr+k);//转换为BYTE型数组
				}
				else
				{
					safearray_inp.GetElement(&k,rxdataStruct.rxdata+k);//转换为BYTE型数组
				}
			}
		}
		rxdataStruct.len = len;
		rxdataStruct.rx_ptr+=k;
		rxdataStruct.rx_ptr%=2048;
	}
}


void CommManager::sendCommData(COleVariant theValue)
{
	m_comm.put_Output(theValue);
}

void CommManager::sendData(CString str,bool hex)
{
	if(hex)
	{
		CByteArray hexdata;
		int len=String2Hex(str,hexdata); //此处返回的len可以用于计算发送了多少个十六进制数
		m_comm.put_Output(COleVariant(hexdata));
		//m_comm.SetOutput(COleVariant(hexdata)); //发送十六进制数据
	}
	else 
		m_comm.put_Output(COleVariant(str));
	//m_comm.SetOutput(COleVariant(str));//发送ASCII字符数据
}


int CommManager::String2Hex(CString str, CByteArray &senddata)
{
	int hexdata,lowhexdata;
	int hexdatalen=0;
	int len=str.GetLength();
	senddata.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr==' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		lstr=str[i];
		hexdata=ConvertHexChar(hstr);		//完成了十六进制的转换
		lowhexdata=ConvertHexChar(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else 
			hexdata=hexdata*16+lowhexdata;
		i++;
		senddata[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	senddata.SetSize(hexdatalen);
	return hexdatalen;
}

char CommManager::ConvertHexChar(char ch) 
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	else if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	else if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else 
		return (-1);
}
