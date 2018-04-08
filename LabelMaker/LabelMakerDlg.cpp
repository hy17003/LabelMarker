
// LabelMakerDlg.cpp : implementation file
//
#include "stdafx.h"
#include "LabelMaker.h"
#include "LabelMakerDlg.h"
#include "afxdialogex.h"
#include <opencv2\opencv.hpp>
#include "HelpDlg.h"
#include "EnhanceDlg.h"
#include "ProcessDlg.h"
#include <stdio.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>  
#include <fstream>
#include "OtherToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hThread = NULL;
HANDLE hResizeThread = NULL;
HANDLE hRadomCutThread = NULL;
HANDLE hVideo2ImageThread = NULL;
CRITICAL_SECTION cs;
// CAboutDlg dialog used for App About
bool RectInImage(cv::Rect testRect, cv::Mat testMat)
{
	return (testRect.x > 0 && testRect.y > 0 &&
		testRect.x + testRect.width < testMat.cols &&
		testRect.y + testRect.height < testMat.rows);
}

std::string CString2String(CString str)
{
	USES_CONVERSION;
	std::string str1 = W2A(str);
	return str1;
}

CString GetFileName(CString filePath, BOOL isWithFileSuffix)
{
	int pos = filePath.ReverseFind('\\');
	CString fileName = filePath.Right(filePath.GetLength() - pos - 1);
	if (FALSE == isWithFileSuffix)
	{
		pos = fileName.Find('.');
		fileName = fileName.Left(pos);
	}
	return fileName;
}

CString GetFolderName(CString filePath)
{
	int pos = filePath.ReverseFind('\\');
	CString folderPath = filePath.Left(pos - 1);
	return folderPath;
}

DWORD WINAPI ResizeProc(PVOID pParam)
{
	COtherToolDlg *pDlg = (COtherToolDlg*)pParam;
	std::vector<CString>& srcList = pDlg->srcList;
	CString dstFolder = pDlg->strDstPath;
	std::string folder = CString2String(dstFolder);
	int num = srcList.size();
	::PostMessage(pDlg->FatherWnd->m_hWnd, WM_SHOWPROGRESSBAR, 0, (LPARAM)&num);
	for (int i = 0; i < num;i++)
	{
		EnterCriticalSection(&cs);
		BOOL bRunning = ((CLabelMakerDlg*)(pDlg->FatherWnd))->bRunning;
		LeaveCriticalSection(&cs);
		if (!bRunning)
		{
			break;
		}
		std::string strImagePath = CString2String(srcList[i]);
		cv::Mat image = cv::imread(strImagePath);
		cv::resize(image, image, cv::Size(pDlg->iNormalWidth, pDlg->iNormalHeight));
		char filename[200];
		CString strFileName = GetFileName(srcList[i], TRUE);
		std::string tmpName = CString2String(strFileName);
		sprintf(filename, "%s\\%s", folder.c_str(), tmpName.c_str());
		imwrite(filename, image);
		PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
			WM_PROGRESSUPDATE, 0, (LPARAM)&i);
	}
	Sleep(500);
	PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
		WM_CLOSEPROGRESS, 0, 0);
	return 0;
}

#define RandomRange(a, b) (rand()%(b-a)+a)

DWORD WINAPI RadomCutProc(PVOID pParam)
{
	COtherToolDlg *pDlg = (COtherToolDlg*)pParam;
	std::vector<CString>& srcList = pDlg->srcList;
	CString dstFolder = pDlg->strDstPath2;
	std::string folder = CString2String(dstFolder);
	int num = srcList.size();
	int minSize = pDlg->iMinSize;
	int maxSize = pDlg->iMaxSize;
	int norSize = pDlg->iNormalSize;
	srand((unsigned)time(NULL));
	::PostMessage(pDlg->FatherWnd->m_hWnd, WM_SHOWPROGRESSBAR, 0, (LPARAM)&num);
	for (int i = 0; i < num; i++)
	{
		EnterCriticalSection(&cs);
		BOOL bRunning = ((CLabelMakerDlg*)(pDlg->FatherWnd))->bRunning;
		LeaveCriticalSection(&cs);
		if (!bRunning)
		{
			break;
		}
		std::string strImagePath = CString2String(srcList[i]);
		cv::Mat image = cv::imread(strImagePath);
		if (image.empty())
		{
			continue;
		}
		for (int j = 0; j < pDlg->iCutNum;j++)
		{
			int x1 = RandomRange(0, image.cols - minSize);
			int y1 = RandomRange(0, image.rows - minSize);
			int s = RandomRange(minSize, maxSize);
			int x2 = x1 + s;
			int y2 = y1 + s;
			cv::Rect rect = cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
			if (RectInImage(rect, image))
			{
				cv::Mat patchMat = cv::Mat(image, rect);
				if (pDlg->bNormalSize)
				{
					cv::resize(patchMat, patchMat, cv::Size(norSize, norSize));
				}				
				char filename[200];
				CString strFileName = GetFileName(srcList[i], FALSE);
				std::string tmpName = CString2String(strFileName);
				sprintf(filename, "%s\\%s_%03d.jpg", folder.c_str(), tmpName.c_str(), j);
				imwrite(filename, patchMat);
			}
		}
		PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
			WM_PROGRESSUPDATE, 0, (LPARAM)&i);
	}
	Sleep(500);
	PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
		WM_CLOSEPROGRESS, 0, 0);
	return 0;
}

DWORD WINAPI EnhanceProc(PVOID pParam)
{
	CEnhanceDlg *pDlg = (CEnhanceDlg*)pParam;
	std::string strImagePath = CString2String(pDlg->mImagesPath);
	std::string strSamplePath = CString2String(pDlg->mSamplePath);
	std::string filename = strImagePath + std::string("\\label.txt");
	FILE *pfile = fopen(filename.c_str(), "r");
	if (pfile == NULL)
	{
		MessageBox(NULL, _T("Cannot Find label.txt!"), _T("Information"), MB_OK);
		return 0;
	}
	char imagename[100];
	int x = 0, y = 0, w = 0, h = 0;
	std::vector<std::string> vcName;
	std::vector<int> vcX, vcY, vcW, vcH;
	int num = 0;
	while (!feof(pfile))
	{
		fscanf(pfile, "%s %d %d %d %d", &imagename, &x, &y, &w, &h);
		vcName.push_back(imagename);
		vcX.push_back(x);
		vcY.push_back(y);
		vcW.push_back(w);
		vcH.push_back(h);
		num++;
	}
	fclose(pfile);
	//创建过程对话框
	//::SendMessage(pDlg->FatherWnd->m_hWnd, WM_SHOWPROGRESSBAR, 0, (LPARAM)&num);
	::PostMessage(pDlg->FatherWnd->m_hWnd, WM_SHOWPROGRESSBAR, 0, (LPARAM)&num);
	for (int i = 0;i < num;i++)
	{
		EnterCriticalSection(&cs);
		BOOL bRunning = ((CLabelMakerDlg*)(pDlg->FatherWnd))->bRunning;
		LeaveCriticalSection(&cs);
		if (!bRunning)
		{
			break;
		}
		std::string imageName = strImagePath + "\\" + vcName[i];
		cv::Mat image = cv::imread(imageName);
		if (image.data)
		{
			//X位置
			if (pDlg->bXPos)
			{
				int idx = 0;
				for (double dOffsetX = pDlg->dStartPos;dOffsetX <= pDlg->dEndPos;
				dOffsetX += pDlg->dPosInterval)
				{
					idx++;
					int dx = dOffsetX * vcW[i];
					cv::Rect rect = cv::Rect(vcX[i] + dx, vcY[i], vcW[i], vcH[i]);
					if (RectInImage(rect, image) && rect.area() > 0)
					{
						cv::Mat patchImg = cv::Mat(image, rect);
						if (pDlg->bResize)
						{
							cv::resize(patchImg, patchImg, 
								cv::Size(pDlg->iResizeWidth, pDlg->iResizeWidth));
						}
						char saveName[100];
						sprintf(saveName, "%s\\sample_%05dX%02d.jpg", 
							strSamplePath.c_str(), i, idx);
						cv::imwrite(std::string(saveName), patchImg);
					}
				}
			}
			//Y位置
			if (pDlg->bXPos)
			{
				int idx = 0;
				for (double dOffsetY = pDlg->dStartPos;dOffsetY <= pDlg->dEndPos;
				dOffsetY += pDlg->dPosInterval)
				{
					idx++;
					int dy = dOffsetY * vcH[i];
					cv::Rect rect = cv::Rect(vcX[i], vcY[i] + dy, vcW[i], vcH[i]);
					if (RectInImage(rect, image) && rect.area() > 0)
					{
						cv::Mat patchImg = cv::Mat(image, rect);
						if (pDlg->bResize)
						{
							cv::resize(patchImg, patchImg,
								cv::Size(pDlg->iResizeWidth, pDlg->iResizeWidth));
						}
						char saveName[100];
						sprintf(saveName, "%s\\sample_%05dY%02d.jpg", 
							strSamplePath.c_str(), i, idx);
						cv::imwrite(std::string(saveName), patchImg);
					}
				}
			}
			//角度
			if (pDlg->bAngle)
			{
				int idx = 0;

				for (double dAngle = pDlg->dStartAngle;dAngle <= pDlg->dEndAngle;
				dAngle += pDlg->dAngleInterval)
				{
					idx++;
					cv::Point2f centerPt;
					centerPt.x = vcX[i] + 0.5 * vcW[i];
					centerPt.y = vcY[i] + 0.5 * vcH[i];
					cv::Mat rot_mat = cv::getRotationMatrix2D(centerPt, dAngle, 1.0);
					cv::Mat dst;
					cv::warpAffine(image, dst, rot_mat, cv::Size(image.cols, image.rows));
					cv::Rect rect = cv::Rect(vcX[i], vcY[i], vcW[i], vcH[i]);
					if (RectInImage(rect, dst) && rect.area() > 0)
					{
						cv::Mat patchImg = cv::Mat(dst, rect);
						if (pDlg->bResize)
						{
							cv::resize(patchImg, patchImg,
								cv::Size(pDlg->iResizeWidth, pDlg->iResizeWidth));
						}
						char saveName[100];
						sprintf(saveName, "%s\\sample_%05dA%02d.jpg", 
							strSamplePath.c_str(), i, idx);
						cv::imwrite(std::string(saveName), patchImg);
					}
				}
			}
		}
		PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
			WM_PROGRESSUPDATE, 0, (LPARAM)&i);
	}
	Sleep(500);
	PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
		WM_CLOSEPROGRESS, 0, 0);
	return 0;
}


DWORD WINAPI VideoToImagesProc(PVOID pParam)
{
	COtherToolDlg *pDlg = (COtherToolDlg*)pParam;
	std::string videoPath = CString2String(pDlg->strVideoFilePath);
	std::string videoName = CString2String(GetFileName(pDlg->strVideoFilePath, FALSE));
	std::string imageFolder = CString2String(pDlg->strImageOutPutFolder);
	std::string imageExt[2] = { ".jpg", ".bmp" }; 
	int iForMatIdx = pDlg->iFormatIdx;
	cv::VideoCapture cap(videoPath);
	if (!cap.isOpened())
	{
		return 0;
	}
	int num = cap.get(CV_CAP_PROP_FRAME_COUNT);
	::PostMessage(pDlg->FatherWnd->m_hWnd, WM_SHOWPROGRESSBAR, 0, (LPARAM)&num);
	cv::Mat frame;
	int idx = 0;
	do 
	{
		EnterCriticalSection(&cs);
		BOOL bRunning = ((CLabelMakerDlg*)(pDlg->FatherWnd))->bRunning;
		LeaveCriticalSection(&cs);
		if (!bRunning)
		{
			break;
		}
		idx++;
		cap >> frame;
		if (idx%pDlg->iSampleInterval == 0)
		{
			char filename[200];
			sprintf(filename, "%s\\%s_%03d%s", imageFolder.c_str(),
				videoName.c_str(), idx, imageExt[iForMatIdx].c_str());
			imwrite(filename, frame);
		}
		PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
			WM_PROGRESSUPDATE, 0, (LPARAM)&idx);

	} while (frame.data);
	Sleep(500);
	PostMessage(((CLabelMakerDlg*)(pDlg->FatherWnd))->progressDlg.m_hWnd,
		WM_CLOSEPROGRESS, 0, 0);
	return 0;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLabelMakerDlg dialog



CLabelMakerDlg::CLabelMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLabelMakerDlg::IDD, pParent)
	, m_strPath(_T(""))
	, m_iType(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bMouseDown = FALSE;
	bToSave = FALSE;
}

void CLabelMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PATH, m_strPath);
	DDX_Control(pDX, IDC_LIST_FILENAME, m_ctrList);
	DDX_Radio(pDX, IDC_RADIO_STYPE1, m_iType);
}

BEGIN_MESSAGE_MAP(CLabelMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CLabelMakerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CLabelMakerDlg::OnBnClickedBtnOpen)
	ON_LBN_DBLCLK(IDC_LIST_FILENAME, &CLabelMakerDlg::OnLbnDblclkListFilename)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_STYPE1, &CLabelMakerDlg::OnBnClickedRadioStype1)
	ON_BN_CLICKED(IDC_RADIO_STYPE2, &CLabelMakerDlg::OnBnClickedRadioStype2)
	//ON_WM_CHAR()
	//ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BTN_PREVIOUS, &CLabelMakerDlg::OnBnClickedBtnPrevious)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CLabelMakerDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_HELP, &CLabelMakerDlg::OnBnClickedBtnHelp)
	ON_BN_CLICKED(IDC_BTN_ENHANCE, &CLabelMakerDlg::OnBnClickedBtnEnhance)
	//建立Progress对话框
	ON_MESSAGE(WM_SHOWPROGRESSBAR, ShowProgressDlg)
	ON_BN_CLICKED(IDC_BTN_TOOL, &CLabelMakerDlg::OnBnClickedBtnTool)
END_MESSAGE_MAP()


// CLabelMakerDlg message handlers

BOOL CLabelMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	enhanceDlg.FatherWnd = this;
	toolDlg.FatherWnd = this;
	// TODO: Add extra initialization here
	ResetDlgItemPosition(640, 480);
	InitializeCriticalSection(&cs);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLabelMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLabelMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		ShowImage(IDC_PIC, &m_dib, frame);
		ShowImage(IDC_LOCAL_PIC, &m_smallDib, patch);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLabelMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLabelMakerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

BOOL CLabelMakerDlg::IsDirectoryExists(CString const& path)
{
	if (!PathFileExists(path))
		return false;
	DWORD attributes;
	attributes = ::GetFileAttributes(path);
	attributes &= FILE_ATTRIBUTE_DIRECTORY;
	return attributes == FILE_ATTRIBUTE_DIRECTORY;
}

void CLabelMakerDlg::RecursiveFindFile(CString& strPath, CString ext)
{
	CFileFind ff;
	BOOL bFind = ff.FindFile(strPath + _T("\\*.") + ext);
	while (bFind)
	{
		bFind = ff.FindNextFile();
		if (ff.IsDots() || ff.IsSystem() || ff.IsHidden())
			continue;

		if (ff.IsDirectory())
		{
			CString folder = ff.GetFilePath();
			if (folder == strPath)
			{
				continue;
			}
			else
			{
				RecursiveFindFile(ff.GetFilePath());
			}
		}
		else
		{
			CString strFileName = ff.GetFilePath();
			filelist.push_back(strFileName);
		}
	}
}

void CLabelMakerDlg::RecursiveFindFile(CString& strPath)
{
	RecursiveFindFile(strPath, _T("jpg"));
	RecursiveFindFile(strPath, _T("bmp"));
}

void CLabelMakerDlg::OnBnClickedBtnOpen()
{
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("Please choice folder:");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		m_strPath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
	filelist.clear();
	RecursiveFindFile(m_strPath);
	ShowFileList();
	if (filelist.size() > 0)
	{
		OpenImageOfList(0);
	}
}


void CLabelMakerDlg::ShowFileList()
{
	m_ctrList.ResetContent();
	for (int i = 0; i < filelist.size(); i++)
	{
		CString filePath = filelist[i];
		CString filename = filePath.Mid(filePath.ReverseFind('\\') + 1);
		m_ctrList.AddString(filename);
	}
}

void CLabelMakerDlg::OnLbnDblclkListFilename()
{
	// TODO: Add your control notification handler code here
	int index = m_ctrList.GetCurSel();
	m_ctrList.SetCurSel(index);
	if (index >= 0)
	{	
		targetRect = cv::Rect(-1, -1, 0, 0);
		OpenImageOfList(index);



		//CString filePath = filelist[index];
		//std::string strPath = (LPCSTR)(CStringA)filePath;
		//frame = cv::imread(strPath);
		//if (frame.data)
		//{
		//	ResetDlgItemPosition(frame.cols, frame.rows);
		//	CRect rc;
		//	GetDlgItem(IDC_PIC)->GetWindowRect(rc);
		//	Rectangle(GetDlgItem(IDC_PIC)->GetDC()->m_hDC, 0, 0, rc.Width(), rc.Height());
		//	ShowImage(IDC_PIC, &m_dib, frame);
		//}
	}
}


void CLabelMakerDlg::ShowImage(UINT nID, ImgCenterDib* pDib, cv::Mat image)
{
	pDib->ConvertOpenCVMatToDIB(image, TRUE);
	CDC* pDC = GetDlgItem(nID)->GetDC();
	CPoint startPt;
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(rc);
	startPt.x = 0.5 * (rc.Width() - pDib->GetDimensions().cx);
	startPt.y = 0.5 * (rc.Height() - pDib->GetDimensions().cy);
	pDib->Draw(pDC, startPt, pDib->GetDimensions());
	ReleaseDC(pDC);
}

void CLabelMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt = GetMousePointOfPIC(point);
	CRect picRect;
	GetDlgItem(IDC_PIC)->GetClientRect(picRect);
//	ScreenToClient(picRect);
	if (PtInRect(picRect, pt))
	{
		if (bMouseDown)
		{			
			pt2 = pt;
			if (frame.data)
			{
				cv::Mat tmp = frame.clone();
				cv::Point ipt1, ipt2;
				ipt1 = ToCVPoint(pt1);
				ipt2 = ToCVPoint(pt2);
				ipt1 = PICControlToImage(ipt1, frame);
				ipt2 = PICControlToImage(ipt2, frame);
				cv::rectangle(tmp, targetRect, cv::Scalar(0, 255, 0), 2);
				cv::line(tmp, cv::Point(ipt2.x, 0), cv::Point(ipt2.x, tmp.rows - 1), cv::Scalar(0, 255, 255));
				cv::line(tmp, cv::Point(0, ipt2.y), cv::Point(tmp.cols - 1, ipt2.y), cv::Scalar(0, 255, 255));
				ShowImage(IDC_PIC, &m_dib, tmp);
				cv::Rect tmpRect(ipt1, ipt2);
				if (m_iType)
				{
					int w = 0;
					if (tmpRect.width > tmpRect.height)
					{
						w = tmpRect.height;
					}
					else
					{
						w = tmpRect.width;
					}
					if (ipt2.x > ipt1.x)
					{
						ipt2.x = ipt1.x + w;
					}
					else
					{
						ipt2.x = ipt1.x - w;
					}
					if (ipt2.y > ipt1.y)
					{
						ipt2.y = ipt1.y + w;
					}
					else
					{
						ipt2.y = ipt1.y - w;
					}
				}
				targetRect = cv::Rect(ipt1, ipt2);
				bToSave = TRUE;
				/*cv::rectangle(tmp, targetRect, cv::Scalar(0, 255, 0), 2);
				cv::line(tmp, cv::Point(ipt2.x, 0), cv::Point(ipt2.x, tmp.rows - 1), cv::Scalar(0, 255, 255));
				cv::line(tmp, cv::Point(0, ipt2.y), cv::Point(tmp.cols - 1, ipt2.y), cv::Scalar(0, 255, 255));
				ShowImage(IDC_PIC, &m_dib, tmp);*/
			}
		}
		if (frame.data)
		{
			cv::Point centerPt;
			centerPt = ToCVPoint(pt);
			centerPt = PICControlToImage(centerPt, frame);
			cv::Point tlPt(centerPt.x - 20, centerPt.y - 20);
			cv::Point brPt(centerPt.x + 20, centerPt.y + 20);
			if(tlPt.x < 0)tlPt.x = 0;
			if(tlPt.y < 0)tlPt.y = 0;
			if (brPt.x > frame.cols - 1)brPt.x = frame.cols - 1;
			if (brPt.y > frame.rows - 1)brPt.y = frame.rows - 1;
			if (tlPt.x < brPt.x && tlPt.y < brPt.y)
			{
				cv::Mat patchMat = cv::Mat(frame, cv::Rect(tlPt, brPt));
				int offset_x = 40 - patchMat.cols;
				int offset_y = 40 - patchMat.rows;
				cv::Mat dstMat(cv::Size(40, 40), CV_8UC3, cv::Scalar::all(0));
				cv::Mat roi(dstMat, cv::Rect(offset_x, offset_y, patchMat.cols, patchMat.rows));
				patchMat.copyTo(roi);
				resize(dstMat, dstMat, cv::Size(160, 160));
				cv::line(dstMat, cv::Point(0, 80), cv::Point(160, 80), cv::Scalar(0, 0, 255));
				cv::line(dstMat, cv::Point(80, 0), cv::Point(80, 160), cv::Scalar(0, 0, 255));
				patch = dstMat.clone();
				ShowImage(IDC_LOCAL_PIC, &m_smallDib, patch);
				cv::Mat tmp = frame.clone();
				cv::line(tmp, cv::Point(centerPt.x, 0), cv::Point(centerPt.x, tmp.rows - 1), cv::Scalar(0, 255, 255));
				cv::line(tmp, cv::Point(0, centerPt.y), cv::Point(tmp.cols - 1, centerPt.y), cv::Scalar(0, 255, 255));
				cv::rectangle(tmp, targetRect, cv::Scalar(0, 255, 0), 2);
				ShowImage(IDC_PIC, &m_dib, tmp);
			}
		}
		CString strInfo;
		strInfo.Format(_T("%d, %d"), pt.x, pt.y);
		ShowInformation(strInfo);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


CPoint CLabelMakerDlg::GetMousePointOfPIC(CPoint point)
{
	CRect picRect;
	GetDlgItem(IDC_PIC)->GetWindowRect(picRect);
	ScreenToClient(picRect);
	CString str;
	CPoint pt = CPoint(point.x - picRect.left, point.y - picRect.top);
	return pt;
}

void CLabelMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt = GetMousePointOfPIC(point);
	CRect picRect;
	GetDlgItem(IDC_PIC)->GetWindowRect(picRect);
	ScreenToClient(picRect);
	if (PtInRect(picRect, pt))
	{
		bMouseDown = TRUE;
		pt1 = pt;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLabelMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt = GetMousePointOfPIC(point);
	CRect picRect;
	GetDlgItem(IDC_PIC)->GetWindowRect(picRect);
	ScreenToClient(picRect);
	if (PtInRect(picRect, pt))
	{
		pt2 = pt;
	}
	pt1 = CPoint(-1, -1);
	pt2 = CPoint(-1, -1);
	bMouseDown = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}


cv::Point CLabelMakerDlg::ToCVPoint(CPoint pt)
{
	cv::Point cpt;
	cpt.x = pt.x;
	cpt.y = pt.y;
	return cpt;
}


cv::Point CLabelMakerDlg::PICControlToImage(cv::Point pt, cv::Mat image)
{
	cv::Point startPt;
	CRect rc;
	GetDlgItem(IDC_PIC)->GetWindowRect(rc);
	startPt.x = 0.5 * (rc.Width() - image.cols);
	startPt.y = 0.5 * (rc.Height() - image.rows);
	cv::Point offsetPt;
	offsetPt.x = pt.x - startPt.x;
	offsetPt.y = pt.y - startPt.y;
	return offsetPt;
}

void CLabelMakerDlg::ResetDlgItemPosition(int imageWidth, int imageHeight)
{
	imageWidth = imageWidth > 640 ? imageWidth : 640;
	imageHeight = imageHeight > 480 ? imageHeight : 480;
	int iSpace = 10;
	int btnW = 80;
	int btnH = 25;
	int patchW = 160;
	int newWidth = imageWidth + 5 * iSpace + patchW; //150为小图的宽度
	int newHeight = imageHeight + 8 * iSpace + 2 * btnH; //50为按钮的高度
	SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_SHOWWINDOW);
	CRect rect = CRect(iSpace, iSpace, iSpace + imageWidth, iSpace + imageHeight);
	GetDlgItem(IDC_PIC)->MoveWindow(rect);
	//小图像框
	rect = CRect(2 * iSpace + imageWidth, iSpace, iSpace + 2 * iSpace + imageWidth + patchW, iSpace + patchW);
	GetDlgItem(IDC_LOCAL_PIC)->MoveWindow(rect);
	//列表框
	rect = CRect(2 * iSpace + imageWidth, 2 * iSpace + patchW, 3 * iSpace + imageWidth + patchW, iSpace + imageHeight);
	GetDlgItem(IDC_LIST_FILENAME)->MoveWindow(rect);
	//第一行按钮
	rect = CRect(iSpace, 2 * iSpace + imageHeight, iSpace + btnW, 2 * iSpace + imageHeight + btnH);
	GetDlgItem(IDC_BTN_OPEN)->MoveWindow(rect);
	rect = CRect(2 * iSpace + btnW, 2 * iSpace + imageHeight, 2 * iSpace + 2 * btnW, 2 * iSpace + imageHeight + btnH);
	GetDlgItem(IDC_STATIC_STYPE)->MoveWindow(rect);
	rect = CRect(3 * iSpace + 2 * btnW, 2 * iSpace + imageHeight, 3 * iSpace + 3 * btnW, 2 * iSpace + imageHeight + btnH);
	GetDlgItem(IDC_RADIO_STYPE1)->MoveWindow(rect);
	rect = CRect(4 * iSpace + 3 * btnW, 2 * iSpace + imageHeight, 4 * iSpace + 4 * btnW, 2 * iSpace + imageHeight + btnH);
	GetDlgItem(IDC_RADIO_STYPE2)->MoveWindow(rect);
	rect = CRect(5 * iSpace + 4 * btnW, 2 * iSpace + imageHeight, 3 * iSpace + imageWidth + patchW, 2 * iSpace + imageHeight+ btnH);
	GetDlgItem(IDC_EDT_PATH)->MoveWindow(rect);
	//第二行按钮
	rect = CRect(1 * iSpace, 3 * iSpace + btnH + imageHeight, 1 * iSpace + btnW, 3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_BTN_PREVIOUS)->MoveWindow(rect);
	rect = CRect(2 * iSpace + 1 * btnW, 3 * iSpace + btnH + imageHeight, 2 * iSpace + 2 * btnW, 3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_BTN_NEXT)->MoveWindow(rect);
	//帮助
	rect = CRect(3 * iSpace + imageWidth + patchW  - 1 * btnW, 
		3 * iSpace + +btnH + imageHeight, 
		3 * iSpace + imageWidth + patchW, 
		3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_BTN_HELP)->MoveWindow(rect);
	//其它工具
	rect = CRect(3 * iSpace + imageWidth + patchW - 1 * iSpace - 2 * btnW,
		3 * iSpace + +btnH + imageHeight,
		3 * iSpace + imageWidth + patchW - 1 * btnW - 1 * iSpace,
		3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_BTN_TOOL)->MoveWindow(rect);
	//增强
	rect = CRect(3 * iSpace + imageWidth + patchW - 1 * iSpace - 3 * btnW,
		3 * iSpace + +btnH + imageHeight,
		3 * iSpace + imageWidth + patchW - 2 * btnW - 2 * iSpace,
		3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_BTN_ENHANCE)->MoveWindow(rect);
	//信息
	rect = CRect(3 * iSpace + 3 * btnW, 
		3 * iSpace + +btnH + imageHeight,
		3 * iSpace + imageWidth + patchW - 3 * iSpace - 3 * btnW,
		3 * iSpace + imageHeight + 2 * btnH);
	GetDlgItem(IDC_STATIC_INFO)->MoveWindow(rect);
	
}

void CLabelMakerDlg::OnBnClickedRadioStype1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void CLabelMakerDlg::OnBnClickedRadioStype2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


BOOL CLabelMakerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		int currentIdx = m_ctrList.GetCurSel();
		CString labelPath;
		CString filename;
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://屏蔽ESCAPE键
			return TRUE;
		case 'A':
			targetRect = cv::Rect(-1, -1, 0, 0);
			currentIdx -= 1;
			OpenImageOfList(currentIdx);
			return TRUE;
		case 'D':
			targetRect = cv::Rect(-1, -1, 0, 0);
			currentIdx += 1;
			OpenImageOfList(currentIdx);
			return TRUE;
		case VK_SPACE:			
			if (bToSave)
			{
				labelPath = filelist[currentIdx];
				filename = labelPath.Mid(labelPath.ReverseFind('\\') + 1);
				labelPath = labelPath.Left(labelPath.ReverseFind('\\'));
				labelPath = labelPath + _T("\\label.txt");
				SaveLabel(labelPath, filename, targetRect);
				bToSave = FALSE;
				CString strInfo;
				strInfo = filelist[currentIdx] + _T("标注成功！");
				ShowInformation(strInfo);
				targetRect = cv::Rect(-1, -1, 0, 0);
			}			
			currentIdx += 1;
			OpenImageOfList(currentIdx);
			return TRUE;
		default:
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLabelMakerDlg::OpenImageOfList(int index)
{

	if (index < 0)
	{
		MessageBox(_T("文件到头了！"));
	}
	else if (index >= filelist.size())
	{
		MessageBox(_T("文件到尾了！"));
	}
	else
	{
		m_ctrList.SetCurSel(index);
		CString filePath = filelist[index];
		std::string strPath = (LPCSTR)(CStringA)filePath;
		frame = cv::imread(strPath);
		if (frame.data)
		{
			fx = 1.0;
			fy = 1.0;
			//缩放图像
			double MaxWidth = 0.6 * GetSystemMetrics(SM_CXSCREEN) - 210;
			double MaxHeight = 0.6 * GetSystemMetrics(SM_CYSCREEN) - 130;
			int ProperWidth = frame.cols;
			int ProperHeight = frame.rows;
			if (ProperWidth > MaxWidth || ProperHeight > MaxHeight)
			{
				fx = double(frame.cols) / MaxWidth;
				fy = double(frame.rows) / MaxHeight;
				if (fx > fy)fx = fy;
				if (fy > fx)fy = fx;
				ProperWidth = ProperWidth / fx;
				ProperHeight = ProperHeight / fy;
			}
			resize(frame, frame, cv::Size(ProperWidth, ProperHeight));
			ResetDlgItemPosition(ProperWidth, ProperHeight);
			CRect rc;
			GetDlgItem(IDC_PIC)->GetWindowRect(rc);
			Rectangle(GetDlgItem(IDC_PIC)->GetDC()->m_hDC, 0, 0, rc.Width(), rc.Height());
			ShowImage(IDC_PIC, &m_dib, frame);
		}
	}
}

void CLabelMakerDlg::SaveLabel(CString labelPath, CString imagePath, cv::Rect targetRect)
{
	CStdioFile file;
	setlocale(LC_CTYPE, ("chs"));
	CString strLabel;
	int x = double(targetRect.x) * fx;
	int y = double(targetRect.y) * fy;
	int w = double(targetRect.width) * fx;
	int h = double(targetRect.height) * fy;
	strLabel.Format(_T("%s %d %d %d %d"), imagePath.GetBuffer(0), x, y, w, h);
	file.Open(labelPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	file.SeekToEnd();
	file.WriteString(strLabel);
	file.WriteString(CString("\n"));
	file.Close();
}

void CLabelMakerDlg::ShowInformation(CString strInfo)
{
	SetDlgItemText(IDC_STATIC_INFO, strInfo);
}

void CLabelMakerDlg::OnBnClickedBtnPrevious()
{
	// TODO: Add your control notification handler code here
	int currentIdx = m_ctrList.GetCurSel();
	targetRect = cv::Rect(-1, -1, 0, 0);
	currentIdx -= 1;
	OpenImageOfList(currentIdx);
}


void CLabelMakerDlg::OnBnClickedBtnNext()
{
	// TODO: Add your control notification handler code here
	int currentIdx = m_ctrList.GetCurSel();
	targetRect = cv::Rect(-1, -1, 0, 0);
	currentIdx += 1;
	OpenImageOfList(currentIdx);
}


void CLabelMakerDlg::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CHelpDlg helpDlg;
	helpDlg.DoModal();
}



void CLabelMakerDlg::OnBnClickedBtnEnhance()
{
	// TODO: Add your control notification handler code here
	if (enhanceDlg.mImagesPath == _T(""))
		enhanceDlg.mImagesPath = m_strPath;
	if (IDOK == enhanceDlg.DoModal())
	{
		DWORD dwThreadID;
		bRunning = TRUE;
		hThread = CreateThread(NULL, 0, EnhanceProc, (void*)(&enhanceDlg), 0, &dwThreadID);
	}
}


LRESULT CLabelMakerDlg::ShowProgressDlg(WPARAM wParam, LPARAM lParam)
{
	int num = *(int*)lParam;
	progressDlg.FatherWnd = this;
	progressDlg.minVal = 0;
	progressDlg.maxVal = num;
	progressDlg.DoModal();
	return 0;
}


void CLabelMakerDlg::StopGenSample()
{
	EnterCriticalSection(&cs);
	bRunning = FALSE;
	LeaveCriticalSection(&cs);
}

void CLabelMakerDlg::OnBnClickedBtnTool()
{
	// TODO: Add your control notification handler code here
	toolDlg.FatherWnd = this;
	if (toolDlg.DoModal() == IDOK)
	{
		if (toolDlg.iChoice == 0)
		{
			DWORD dwThreadID;
			bRunning = TRUE;
			hResizeThread = CreateThread(NULL, 0, ResizeProc, 
			(void*)(&toolDlg), 0, &dwThreadID);
		}
		else if (toolDlg.iChoice == 1)
		{
			DWORD dwThreadID;
			bRunning = TRUE;
			hRadomCutThread = CreateThread(NULL, 0, RadomCutProc,
				(void*)(&toolDlg), 0, &dwThreadID);
		}
		else if (toolDlg.iChoice == 2)
		{
			DWORD dwThreadID;
			bRunning = TRUE;
			hVideo2ImageThread = CreateThread(NULL, 0, VideoToImagesProc,
				(void*)(&toolDlg), 0, &dwThreadID);
		}

	}
}
