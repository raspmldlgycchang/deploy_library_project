
// ExamMemDCDlg.h: 헤더 파일
//

#pragma once


// CExamMemDCDlg 대화 상자
class CExamMemDCDlg : public CDialogEx
{
private:
	CDC m_mem_dc;
	CBitmap m_mem_bmp;
	HBITMAP mh_mem_bmp, p_old_bmp;
	BITMAP m_bmp_info;
	CImage dest_image;
	int m_width, m_height;
public:
	CExamMemDCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMMEMDC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	void SetBitmap(CString a_file_path);
	HBITMAP GetBitmap();
	void CopyBitmap(HBITMAP ah_bitmap);
	void DrawImage(HDC ah_dc_pic);
	afx_msg void OnBnClickedOpenBtn();
};
