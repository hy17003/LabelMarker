/*------------------------------------------------------------------------------------------*\
类  名:ImgCenterDib
版本号：201505101418
说  明:该类在谢凤英主编的《Visual C++数字图像处理》的ImgCenterDib上进行开发，加入了支持OpenCV2.0
支持，可以通过ConvertDIBToOpenCVMat和ConvertOpenCVMatToDIB函数在cv::Mat及DIB之间进行转换，方便
OpenCV在MFC上的应用。
Copyright (C) 2014-2015 Huang Yin
\*------------------------------------------------------------------------------------------*/
#ifndef _INSIDE_VISUAL_CPP_IMGCENTERDIB
#define _INSIDE_VISUAL_CPP_IMGCENTERDIB
#include <opencv2\opencv.hpp>
//ImgCenterDib类,image processing center,北航图像中心编写的DIB类

//ImgCenterDib类
class ImgCenterDib
{
public:	
	//图像数据指针
	unsigned char * m_pImgData; 

	//图像颜色表指针
	LPRGBQUAD m_lpColorTable; 

	//每像素占的位数
	int m_nBitCount;


private:
	//指向DIB的指针（包含BITMAPFILEHEADER，BITMAPINFOHEADER和颜色表）
	LPBYTE m_lpDib;
	
	//逻辑调色板句柄
	HPALETTE m_hPalette;

	//颜色表长度（多少个表项）
	int m_nColorTableLength;

protected:
	//图像的宽，像素为单位
	int m_imgWidth;

	//图像的高，像素为单位
	int m_imgHeight;

	//图像信息头指针
	LPBITMAPINFOHEADER m_lpBmpInfoHead; 

public:
	//不带参数的构造函数
	ImgCenterDib();

	//带参数的构造函数
	ImgCenterDib(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);

	//析构函数
	~ImgCenterDib();

	//将DIB对象转成OpenCV2.0以上版本的Mat对象
	void ConvertDIBToOpenCVMat(cv::Mat& matOpenCV, BOOL bInverse = FALSE);
	//将OpenCV2.0以上版本的Mat对象转成DIB对象
	BOOL ConvertOpenCVMatToDIB(cv::Mat& matOpenCV, BOOL bInverse = FALSE);

	//获取DIB的尺寸（宽高）
	CSize GetDimensions();

	//DIB读函数
	BOOL Read(LPCTSTR lpszPathName);

	//DIB写函数
	BOOL Write(LPCTSTR lpszPathName);

	//显示DIB
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);  

	//用新的数据替换DIB
	void ReplaceDib(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		unsigned char *pImgData);

	//计算颜色表的长度
	int ComputeColorTabalLength(int nBitCount);
	
private:
	//创建逻辑调色板
	void MakePalette();

	//清理空间
	void Empty();
};
#endif // _INSIDE_VISUAL_CPP_IMGCENTERDIB
