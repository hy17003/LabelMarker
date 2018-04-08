// OtherToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LabelMaker.h"
#include "OtherToolDlg.h"
#include "afxdialogex.h"


// COtherToolDlg dialog

IMPLEMENT_DYNAMIC(COtherToolDlg, CDialogEx)

COtherToolDlg::COtherToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COtherToolDlg::IDD, pParent)
	, strSrcPath(_T(""))
	, strDstPath(_T(""))
	, iNormalWidth(30)
	, iNormalHeight(30)
	, iChoice(0)
	, strSrcPath2(_T(""))
	, strDstPath2(_T(""))
	, iMaxSize(100)
	, iMinSize(30)
	, iNormalSize(60)
	, bNormalSize(FALSE)
	, iCutNum(10)
	, strVideoFilePath(_T(""))
	, strImageOutPutFolder(_T(""))
	, iFormatIdx(0)
	, iSampleInterval(5)
{

}

COtherToolDlg::~COtherToolDlg()
{
}

void COtherToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC_PATH, strSrcPath);
	DDX_Text(pDX, IDC_EDIT_DST_PATH, strDstPath);
	DDX_Text(pDX, IDC_EDIT_WIDTH, iNormalWidth);
	DDV_MinMaxInt(pDX, iNormalWidth, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, iNormalHeight);
	DDV_MinMaxInt(pDX, iNormalHeight, 0, 1000);
	DDX_Radio(pDX, IDC_CHOICE_RESIZE, iChoice);
	DDX_Text(pDX, IDC_EDIT_SRC_PATH2, strSrcPath2);
	DDX_Text(pDX, IDC_EDIT_DST_PATH2, strDstPath2);

	DDX_Text(pDX, IDC_EDIT_MAX_SIZE, iMaxSize);
	DDV_MinMaxInt(pDX, iMaxSize, 10, 100);
	DDX_Text(pDX, IDC_EDIT_MIN_SIZE, iMinSize);
	DDV_MinMaxInt(pDX, iMinSize, 10, 100);
	DDX_Text(pDX, IDC_EDIT_NORMAL_SIZE, iNormalSize);
	DDV_MinMaxInt(pDX, iNormalSize, 10, 100);
	DDX_Check(pDX, IDC_CHECK_NORMALSIZE, bNormalSize);
	DDX_Text(pDX, IDC_EDIT_CUTNUM, iCutNum);
	DDV_MinMaxInt(pDX, iCutNum, 1, 100);
	DDX_Text(pDX, IDC_EDIT_VIDEO_PATH, strVideoFilePath);
	DDX_Text(pDX, IDC_EDIT_IMAGE_OUTPUT, strImageOutPutFolder);
	DDX_Radio(pDX, IDC_RADIO_FORMAT, iFormatIdx);
	DDX_Text(pDX, IDC_EDIT_INTERRVAL, iSampleInterval);
	DDV_MinMaxInt(pDX, iSampleInterval, 0, 10000);
}


BEGIN_MESSAGE_MAP(COtherToolDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BROUSE1, &COtherToolDlg::OnBnClickedBtnBrouse1)
	ON_BN_CLICKED(IDC_BTN_BROUSE2, &COtherToolDlg::OnBnClickedBtnBrouse2)
	ON_BN_CLICKED(IDOK, &COtherToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROUSE3, &COtherToolDlg::OnBnClickedBtnBrouse3)
	ON_BN_CLICKED(IDC_BTN_BROUSE4, &COtherToolDlg::OnBnClickedBtnBrouse4)
	ON_BN_CLICKED(IDC_CHOICE_RESIZE, &COtherToolDlg::OnBnClickedChoiceResize)
	ON_BN_CLICKED(IDC_CHOICE_CUT, &COtherToolDlg::OnBnClickedChoiceCut)
	ON_BN_CLICKED(IDC_CHOICE_VIDEO2IMAGE, &COtherToolDlg::OnBnClickedChoiceVideo2image)
	ON_BN_CLICKED(IDC_BTN_BROUSE5, &COtherToolDlg::OnBnClickedBtnBrouse5)
	ON_BN_CLICKED(IDC_BTN_OPEN_VIDEO, &COtherToolDlg::OnBnClickedBtnOpenVideo)
END_MESSAGE_MAP()


// COtherToolDlg message handlers

void COtherToolDlg::RecursiveFindFile(std::vector<CString>& filelist, CString& strPath)
{
	RecursiveFindFile(filelist, strPath, _T("jpg"));
	RecursiveFindFile(filelist, strPath, _T("bmp"));
}

void COtherToolDlg::RecursiveFindFile(std::vector<CString>& filelist, CString& strPath, CString ext)
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
				RecursiveFindFile(filelist, ff.GetFilePath(), ext);
			}
		}
		else
		{
			CString strFileName = ff.GetFilePath();
			filelist.push_back(strFileName);
		}
	}
}


void COtherToolDlg::OnBnClickedBtnBrouse1()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		strSrcPath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
	srcList.clear();
	RecursiveFindFile(srcList, strSrcPath);
}


void COtherToolDlg::OnBnClickedBtnBrouse2()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		strDstPath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
}


void COtherToolDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void COtherToolDlg::OnBnClickedBtnBrouse3()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		strSrcPath2.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
	srcList.clear();
	RecursiveFindFile(srcList, strSrcPath2);
}


void COtherToolDlg::OnBnClickedBtnBrouse4()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		strDstPath2.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
}

void COtherToolDlg::DisplayItems()
{
	if (iChoice == 0)
	{
		GetDlgItem(IDC_EDIT_DST_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROUSE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SRC_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROUSE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_BROUSE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SRC_PATH2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DST_PATH2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NORMAL_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NORMALSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CUTNUM)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_VIDEO_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMAGE_OUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_OPEN_VIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FORMAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FORMAT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INTERRVAL)->EnableWindow(FALSE);
	}
	else if (iChoice == 1)
	{
		GetDlgItem(IDC_EDIT_DST_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SRC_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_BROUSE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROUSE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SRC_PATH2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DST_PATH2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAX_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NORMAL_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_NORMALSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CUTNUM)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_VIDEO_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMAGE_OUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_OPEN_VIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FORMAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FORMAT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INTERRVAL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_DST_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SRC_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_BROUSE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROUSE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SRC_PATH2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DST_PATH2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NORMAL_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NORMALSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CUTNUM)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_VIDEO_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IMAGE_OUTPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROUSE5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_OPEN_VIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FORMAT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FORMAT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INTERRVAL)->EnableWindow(TRUE);
	}
}

BOOL COtherToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	DisplayItems();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void COtherToolDlg::OnBnClickedChoiceResize()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DisplayItems();
}


void COtherToolDlg::OnBnClickedChoiceCut()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DisplayItems();
}


void COtherToolDlg::OnBnClickedChoiceVideo2image()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DisplayItems();
}


void COtherToolDlg::OnBnClickedBtnBrouse5()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		strImageOutPutFolder.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
}


void COtherToolDlg::OnBnClickedBtnOpenVideo()
{
	// TODO: Add your control notification handler code here
	CString gReadFilePathName;
	CFileDialog fileDlg(true, _T("mp4"), _T("*.mp4"), OFN_HIDEREADONLY | 
		OFN_OVERWRITEPROMPT, _T("mp4 Files (*.mp4)|*.mp4|mov File(*.mov)|*.mov \
		|avi File(*.avi)|*.avi|All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)    //弹出对话框  
	{
		strVideoFilePath = fileDlg.GetPathName();//得到完整的文件名和目录名拓展名  
		UpdateData(FALSE);
	}
}
