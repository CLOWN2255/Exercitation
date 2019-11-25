#pragma once
#include "CMSComm.h"

typedef struct ComPort{                                                                                   
	char			portname[6];	                                                                        
	bool			used;
} ComPort;   
typedef struct rxdata_struct
{
	BYTE rxdata[2048];
	int rx_ptr;
	int handle_ptr;
	int len;
}rxdata_struct;

class CommManager
{
public:
	CommManager(void);
	~CommManager(void);
	
public:
	CMSComm m_comm;
	bool commStatus;
	rxdata_struct rxdataStruct;
	ComPort comport[32];
	void initMSCOMM();
	unsigned char getMSCOMM();
	void openMSCOMM(int portnum,char* portbaund);
	void onMSCOMM();
	void sendData(CString str,bool hex);
	void sendCommData(COleVariant theValue);
	int String2Hex(CString str, CByteArray &senddata);
	char ConvertHexChar(char ch);
};

