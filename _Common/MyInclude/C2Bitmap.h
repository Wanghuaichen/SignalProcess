
// C2Bitmap.h

/*
����:SDK
//*/

#ifndef _TWen_C2Bitmap_H
#define _TWen_C2Bitmap_H

class C2Bitmap
{
public:
	static HANDLE DDBToDIB(CBitmap*pBitmap,DWORD dwCompression,CPalette*pPal,CDC*pDC=NULL);//ת��CBitmap������(DDB)ΪDIB
	static BOOL WriteDIB(LPCTSTR szFile, HANDLE hDIB);//����hDIB�����ݵ�bmp�ļ�
	static BOOL SaveToFile(CBitmap*pBitmap,LPCTSTR lpszFile,CDC*pDC);//����CBitmap�����ݵ�bmp�ļ�
	static BOOL SaveToFile(CDC*pDC,LPCRECT rc,LPCTSTR lpszFile,int w=0,int h=0);//����pDC��rc���������Ϊָ��w��h��bmp��0��ʾ������
	static BOOL SaveAsFile(CBitmap*pBitmap,LPCTSTR lpszFile,CDC*pDC,LPCRECT rc=NULL,int w=0,int h=0);//���챣��CBitmap��rc���򵽿�w,��h��λͼ�ļ�,rcΪ�ձ�ʾ������Ч����,w��h<1��ʾ���������
	static HBITMAP LoadFromFile(LPCTSTR fileName);//��λͼ�ļ�����λͼ,����λͼ���,ʧ�ܷ���NULL
	static void _32To24(LPCTSTR file32, LPCTSTR file24);//�������32λλͼ����ת��
//	static BOOL LoadFromFile(CBitmap&bitmap,LPCTSTR fileName);//����ʹ��,��λͼ�ļ�����λͼ,��Attach��ָ����CBitmap����

	static bool CaptureScreen(LPCTSTR fileName);
};

#endif//_TWen_C2Bitmap_H

