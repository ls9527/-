#pragma once
class FeatureCode
{
public:
	FeatureCode(void);
	~FeatureCode(void);
	BOOL convertAsciiToCharPoint(const char* szAscii,const int &szAsciiLen,char* charPoint,int &charPointLen);
	BOOL GetDataAndBaseAddr(HANDLE hProcess,char* featureCode,DWORD feaLen,int addrOffset,DWORD addrLen,TCHAR * readDataBuff,DWORD& baseAddr);
private:
	BYTE charToAsciiCode(char a);
	
	DWORD GetFeaturePoint(HANDLE hProcess, char* featureCode,int featureLen);
	DWORD GetBaseAddrOffset(HANDLE hProcess,DWORD baseAddr,DWORD offset,DWORD readBaseAddrLen,__out char* readData);
	
	BOOL GetFmtBuff(char* buff,int buffLen,__out TCHAR * text);

	
	
};

