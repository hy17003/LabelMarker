// HelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LabelMaker.h"
#include "HelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg �Ի���

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGHELP, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelpDlg ��Ϣ�������


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strHelpTex = _T("\t\t\t  ʹ��˵��\r\n \
1.�������\r\n \
  1������򿪰�ť��ѡ����������Ŀ¼\r\n \
  2��ʹ������ģʽʱ������Ϊ���Σ�ʹ�ù̶�ģʽʱ������Ϊ����\r\n \
  3�������һ�š���һ�Ż��¼���A��D�������������л�ͼ��\r\n \
  4��һ��ͼ��ÿ��ֻ�ܱ��һ����ǩ�����ж��Ŀ�꣬���Ա�Ǳ�������л��������������\r\n \
  5��������־�󣬻���ͼ��Ŀ¼������label.txt�ļ�\r\n \
  6�����¿ո�����棬ÿ���һ��Ŀ��󣬶�Ҫ���ո��\r\n \
2.������ǿ\r\n \
  1��ѡ��ͼ������Ŀ¼����Ŀ¼�±�����label.txt�ļ�\r\n \
  2��ѡ����������Ŀ¼\r\n \
  3����ѡ�൱�����ӷ�ʽ\r\n \
  4������ѡ��ߴ��һ���������óߴ�\r\n \
  5�����ȷ�� \r\n \
  6�������ɹ����У����ֹͣ��ť����ֹͣ�������ɳ���\r\n \
	\t\t\t\t\t2018.03.20");
	SetDlgItemText(IDC_HELP_TEXT, strHelpTex);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
