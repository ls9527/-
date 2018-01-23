#include "stdafx.h"
#include "FeatureCode.h"



BYTE FeatureCode::charToAsciiCode(char a){

	if(a>=0x6a||a<=0x7A){//小写
		a &= ~0x20;//转为大写ascii
	}
	if(a>=0x41&&a<=0x5a){
		if(a<=0x46){
			a -= 0x37;
		}
	}else{//数字
		a &= 0xF;
	}
	return a;
}

BOOL FeatureCode::convertAsciiToCharPoint(const char* szAscii,const int &szAsciiLen,char* charPoint,int &charPointLen){
	int feaIndex = 0;
	for(int i = 0;i<szAsciiLen;i+=3){
		//book[]
		const char* c = &szAscii[i];
		int high = charToAsciiCode(c[0]);
		int low = charToAsciiCode(c[1]);
		
		char myChar = high <<4 | low;
		if(myChar==0){
			charPointLen = i/3;
			break;
		}
		charPoint[feaIndex] = myChar;
		feaIndex ++;
	}
	return TRUE;
}

DWORD FeatureCode::GetFeaturePoint(HANDLE hProcess, char* featureCode,int featureLen)
{
	
	//特征码8B 45 14 66 3D 93 01
	char *tzm =  featureCode;//{0x8B,0x45,0x14,0x66,0x3d,0x93,0x01};//"8b4514663d9301\0";
	int tzmLen = featureLen;
	int searhBufLen = 4096;
	//DWORD startSearch = 0x7FFFFFFF;
	DWORD startSearch = 0x0000000;
	char* searchBuff = new char[searhBufLen];
	DWORD searchRealLen = 0;

	DWORD baseAddr = 0;
	BOOL findBaseAddr = FALSE;
	while(true){
		ReadProcessMemory(hProcess,(LPVOID)startSearch,searchBuff,searhBufLen,&searchRealLen);
		for(int i = 0;i<searhBufLen - tzmLen;i++){
			int cmpRes = memcmp(searchBuff+i,tzm,tzmLen);
			if(!cmpRes){//相等
				baseAddr =  (int)(startSearch+i);
				findBaseAddr =  TRUE;
				break;

				//continue;
			}
		}
		if(findBaseAddr){
			break;
		}
		//startSearch+= (searhBufLen - tzmLen);
		startSearch+= searhBufLen;
		if(startSearch>=0xFFFFFFFF){
			break;
		}
	}

	return baseAddr;
}
const char book []= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/*

readBaseAddrLen 读取字节数量
readData  读取的内容
*/
DWORD FeatureCode::GetBaseAddrOffset(HANDLE hProcess,DWORD baseAddr,DWORD offset,DWORD readBaseAddrLen,__out char* readData)
{

	int len =  readBaseAddrLen;

	char *buff = new char[len];
	memset(buff,0,len);

	baseAddr+=offset;
	DWORD realRead;
	BOOL bRead = ReadProcessMemory(hProcess,(LPVOID)baseAddr,buff,len,&realRead);
	
	
	
	memcpy_s(readData,readBaseAddrLen,buff,readBaseAddrLen);
	//unicode * 2

	delete [] buff;	
	return TRUE;
}


BOOL FeatureCode::GetFmtBuff(char* buff,int readLen,__out TCHAR * text){

	CString strResult;
	TCHAR splitCode[2] = _T(" ");
	for(int i = 0;i<readLen;i++){

		CString strFmt;
		//获取字节的地位和高位
		int high =  buff[i]>>4 & 0x0F;
		int  low = buff[i]& 0x0F;
		//根据数组对应到ascii码
		strFmt.Append(CString(book[high]));
		strFmt.Append(CString(book[low]));
		strFmt.Append(splitCode);

		strResult.Append(strFmt);

	}
	int byteFmtLen = sizeof(TCHAR) + _tcslen(splitCode);
	int realReadLen = readLen * byteFmtLen *2 ;// 一个字节 DB 会格式化为  tchar * byte * 2,一个字节会有两个字
	TCHAR *strBuff = strResult.GetBuffer(realReadLen);
	memcpy_s(text,realReadLen,strBuff,realReadLen);
	strResult.ReleaseBuffer();
	return TRUE;
}

BOOL FeatureCode::GetDataAndBaseAddr(HANDLE hProcess,char* featureCode,DWORD feaLen,int addrOffset,DWORD addrLen,TCHAR* readDataBuff,DWORD& callAddr)
{
	//int nFeatureCode = m_szFeatureCode.GetLength();
	//TCHAR * featureCode = m_szFeatureCode .GetBuffer(nFeatureCode);


	//char feaCode []= {0x8B,0x45,0x14,0x66,0x3d,0x93,0x01};
	
	DWORD baseAddr = GetFeaturePoint(hProcess,featureCode,feaLen);
	
	ASSERT(baseAddr);

	char readData[682] = {}; //4096 / 6    长度6 = 字节4+空格2
	GetBaseAddrOffset(hProcess,baseAddr,addrOffset,addrLen,readData);


	

	GetFmtBuff(readData,addrLen,readDataBuff);

	//载入cpu的下一个指令
	DWORD nextPoint = baseAddr+addrOffset+ 4 ;
	int pAddr = *(int*)&readData;
	callAddr = nextPoint + pAddr;
	if(pAddr<0){
		pAddr = ~pAddr + 1;
		callAddr = nextPoint - pAddr;
	}

	return TRUE;
}


FeatureCode::FeatureCode(void)
{
}


FeatureCode::~FeatureCode(void)
{
}
