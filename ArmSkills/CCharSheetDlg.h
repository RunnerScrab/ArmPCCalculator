/*
Copyright (c) 2018 RunnerScrab

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include "SkillsDB.h"
#include "CRegistryKey.h"

class CCharSheetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCharSheetDlg)

public:
	CCharSheetDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCharSheetDlg();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeGuildCombo();
	afx_msg void OnCbnSelchangeSubguildCombo();
	afx_msg void OnClose();
	afx_msg void OnOptionsSelectfont();
	afx_msg void OnSize(UINT nType, int cx, int cy);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSHEET_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	int StoreWindowDims();
	int LoadWindowDims();
	int ReadFontRegistrySettings(std::wstring* pFontFace, DWORD* pFontSize);
	int StoreFontRegistrySettings(const wchar_t* szwFontFace, DWORD dwFontSize);
	void UpdateTextEdit();

	std::unique_ptr<SkillsDB> m_spSkillsDB;
	CComboBox* m_pGuildCombobox;
	CComboBox* m_pSubguildCombobox;
	CEdit* m_pEdit;

	CFont   m_editfont;
	LOGFONT m_lfLogFont;

	const wchar_t* m_szwRegKeyPath = L"Software\\ArmCharacterCalculator";
	std::unique_ptr<CRegistryKey> m_pRegKey;

	const int m_dwxborder = 8;
	const int m_dwyborder = 8;
};
