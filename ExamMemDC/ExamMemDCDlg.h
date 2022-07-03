#pragma once
class CExamMemDCDlg : public CDialogEx
{
public:
	CExamMemDCDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMMEMDC_DIALOG };
#endif

public:
	afx_msg void OnDestroy();
	void SetBitmap(CString a_file_path);
	HBITMAP GetBitmap();
	void CopyBitmap(HBITMAP ah_bitmap);
	void DrawImage(HDC ah_dc_pic);
	afx_msg void OnBnClickedOpenBtn();
	void WriteDefaultData(int cx, int cy);
	CString GetSectionName(UINT a_ctrl_id);
	void WriteNullSectionIni();
	void RenewalDisplay();
	CSize GetTotalSize();
	BOOL WriteStringIni(CString sectionName, CString keyName, CString strDefault, CString strFilePath);
	void CExamMemDCDlg::GetFracImgCBrushVers(HDC ah_dc, int a_src_x, int a_src_y, int a_src_width, int a_src_height);
	POINT GetPicCtrlPt();
	DWORD ReadIntIni(CString strSection, CString strKey, CString strDefault, CString strFilePath);
	void DrawClipImagesCBrushVers(HDC ah_dc, int cx, int cy);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CDC m_mem_dc;
	CBitmap m_mem_bmp;
	HBITMAP mh_mem_bmp, p_old_bmp;
	BITMAP m_bmp_info;
	CImage dest_image;
	int m_width, m_height;
	CString m_ini_filepath;
	CRect m_rect_pic_client;
	CString m_file_path;
	CImage m_mem_view;
};
