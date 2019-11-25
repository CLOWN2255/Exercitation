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
	{//�˳���֧��16������
		str.Format("\\\\.\\COM%d",i);
		hCom = CreateFile(str, 0, 0, 0, 
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(INVALID_HANDLE_VALUE != hCom )
		{//�ܴ򿪸ô��ڣ�����Ӹô���
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
		
		//1����ʾ�Զ����Ʒ�ʽ��ȡ����
		m_comm.put_InputMode(1);
		//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
		m_comm.put_RThreshold(1);
		//���õ�ǰ���������ݳ���Ϊ0
		m_comm.put_InputLen(0);
		//��Ԥ���������������������
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
	//�¼�ֵΪ2��ʾ���ջ����������ַ�
	if (m_comm.get_CommEvent()==2)
	{
		//��������
		variant_inp = m_comm.get_Input();
		//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		safearray_inp = variant_inp;
		//�õ���Ч���ݳ���
		len = safearray_inp.GetOneDimSize();
		char tempstr[100];
		sprintf_s(tempstr,"com receive length:%d\r\n",len);
		OutputDebugString(tempstr);
		for(k=0;k<len;k++)
		{
			if (rxdataStruct.rx_ptr+k<2048)
			{
				safearray_inp.GetElement(&k,rxdataStruct.rxdata+rxdataStruct.rx_ptr+k);//ת��ΪBYTE������
			}
			else
			{
				if (rxdataStruct.rx_ptr+k<2048)
				{
					safearray_inp.GetElement(&k,rxdataStruct.rxdata+rxdataStruct.rx_ptr+k);//ת��ΪBYTE������
				}
				else
				{
					safearray_inp.GetElement(&k,rxdataStruct.rxdata+k);//ת��ΪBYTE������
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
		int len=String2Hex(str,hexdata); //�˴����ص�len�������ڼ��㷢���˶��ٸ�ʮ��������
		m_comm.put_Output(COleVariant(hexdata));
		//m_comm.SetOutput(COleVariant(hexdata)); //����ʮ����������
	}
	else 
		m_comm.put_Output(COleVariant(str));
	//m_comm.SetOutput(COleVariant(str));//����ASCII�ַ�����
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
		hexdata=ConvertHexChar(hstr);		//�����ʮ�����Ƶ�ת��
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
