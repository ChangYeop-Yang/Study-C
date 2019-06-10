
// ChatMFCApplicationDlg.h: 헤더 파일
//

#pragma once

#include <memory>

#include "WinTime.h"
#include "WinSocket.h"

#define MWM_SOCK (WM_USER+1)

// CChatMFCApplicationDlg 대화 상자
class CChatMFCApplicationDlg : public CDialogEx
{
	// 생성입니다.
	public:
		CChatMFCApplicationDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CHATMFCAPPLICATION_DIALOG };
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

	// MARK: - Outlet Variables
	private:
		CEdit		IDC_INPUT_PORT_EDIT;
		CEdit		IDC_INPUT_MESSAGE_EDIT;

		CListBox	IDC_EVENT_MESSAGE_LIST;

		CMFCButton	IDC_SERVER_CLOSE_BUTTON;
		CMFCButton	IDC_MESSAGE_SEND_BUTTON;
		CMFCButton	IDC_SERVER_OPEN_BUTTON;

	// MARK: - Object Variables
	private:
		std::unique_ptr<WinSocket>	socket;

	// MARK: - Event Methods
	private:
		afx_msg void OnClickedTCPbuttons(const UINT id);
		afx_msg void OnClickedRadioButtons(const UINT id);
		afx_msg void OnSendMessage();

	// MARK: - User Methods
	private:
		void OpenTCPServer();
		void CloseTCPServer();

	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
