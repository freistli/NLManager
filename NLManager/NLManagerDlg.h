
// NLManagerDlg.h : header file
//

#pragma once
#include "NLMHelper.h"
#include "afxwin.h"

// CNLManagerDlg dialog
class CNLManagerDlg : public CDialogEx
{
// Construction
public:
	CNLManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NLMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnNetworkChanged(WPARAM wParam, LPARAM lParam);
	void Init_Controls();
	void Fill_NW_Info();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNw();

private:
	CNLMHelper m_helper;
	CEdit m_edConType;
	CEdit m_edCategory;
	CEdit m_edDomain;
	CEdit m_edNID;
	CEdit m_edName;
	CEdit m_edCreation;
	CEdit m_edConTime;
	CEdit m_edInternet;
	CEdit m_edHeader;
	int m_nIndex;
public:
	afx_msg void OnBnClickedBtnNext();
private:
	CButton m_btnNext;
};
