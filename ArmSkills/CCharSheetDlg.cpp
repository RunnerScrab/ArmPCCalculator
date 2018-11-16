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

#include "pch.h"
#include "resource.h"
#include <vector>

#include <atlconv.h>

#include "CCharSheetDlg.h"
#include "afxdialogex.h"

#include "Character.h"

// CCharSheetDlg dialog

IMPLEMENT_DYNAMIC(CCharSheetDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CCharSheetDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_GUILD_COMBO, &CCharSheetDlg::OnCbnSelchangeGuildCombo)
	ON_CBN_SELCHANGE(IDC_SUBGUILD_COMBO, &CCharSheetDlg::OnCbnSelchangeSubguildCombo)
	ON_COMMAND(ID_OPTIONS_SELECTFONT, &CCharSheetDlg::OnOptionsSelectfont)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CCharSheetDlg::CCharSheetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARSHEET_DIALOG, pParent)
{
	try
	{
		m_spSkillsDB = std::make_unique<SkillsDB>();
	}
	catch (std::exception ex)
	{
		::MessageBoxA(GetSafeHwnd(), ex.what(), "Error", MB_ICONERROR);
		::ExitProcess(0);
	}
}

CCharSheetDlg::~CCharSheetDlg()
{
}

void CCharSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


static unsigned int GetDPI()
{
	HDC hScreen = ::GetDC(0);
	unsigned int dpi = ::GetDeviceCaps(hScreen, LOGPIXELSX);
	ReleaseDC(0, hScreen);
	return dpi;
}

static unsigned int PixToHeight(unsigned int pix)
{
	int a = pix * GetDPI();
	a = a < 0 ? a * -1 : a;
	return a / 72;
}

BOOL CCharSheetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	try
	{
		m_pRegKey = std::make_unique<CRegistryKey>(m_szwRegKeyPath);
	}
	catch (std::exception ex)
	{
		MessageBox(L"Failed to open registry key.");
	}

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	m_pGuildCombobox = static_cast<CComboBox*>(GetDlgItem(IDC_GUILD_COMBO));
	m_pSubguildCombobox = static_cast<CComboBox*>(GetDlgItem(IDC_SUBGUILD_COMBO));
	m_pEdit = static_cast<CEdit*>(GetDlgItem(IDC_TEXTEDIT));

	std::wstring fontface;
	DWORD fontsize = 0;
	memset(&m_lfLogFont, 0, sizeof(m_lfLogFont));
	if (0 > ReadFontRegistrySettings(&fontface, &fontsize))
	{
		//Couldn't find a previous font setting; use defaults
		m_lfLogFont.lfHeight = PixToHeight(12);
		m_lfLogFont.lfWeight = FW_NORMAL;
		m_lfLogFont.lfUnderline = FALSE;
		wcscpy_s(m_lfLogFont.lfFaceName, 32, L"Courier New");
	}
	else
	{
		// Use registry settings
		wcscpy_s(m_lfLogFont.lfFaceName, fontface.size() + 1, fontface.c_str());
		m_lfLogFont.lfHeight = fontsize;
		m_lfLogFont.lfWeight = FW_NORMAL;
		m_lfLogFont.lfUnderline = FALSE;
	}
	

	m_editfont.CreateFontIndirect(&m_lfLogFont);
	m_pEdit->SetFont(&m_editfont);

	const auto& rClassMap = m_spSkillsDB->GetClassMap();
	for (auto& clpair : rClassMap)
	{
		if (!clpair.second.IsSubclass())
		{
			m_pGuildCombobox->AddString(CA2T(clpair.first.c_str()));
		}
		else
		{
			m_pSubguildCombobox->AddString(CA2T(clpair.first.c_str()));
		}
	}

	LoadWindowDims();

	return TRUE;
}

void CCharSheetDlg::UpdateTextEdit()
{
	CString guildstr;
	if (CB_ERR != m_pGuildCombobox->GetCurSel())
		m_pGuildCombobox->GetLBText(m_pGuildCombobox->GetCurSel(), guildstr);

	CString subguildstr;
	if (CB_ERR != m_pSubguildCombobox->GetCurSel())
		m_pSubguildCombobox->GetLBText(m_pSubguildCombobox->GetCurSel(), subguildstr);

	std::shared_ptr<Character> pChar = m_spSkillsDB->CreateCharacter(CT2A(guildstr.GetString()), CT2A(subguildstr.GetString()));
	if (pChar)
	{
		std::string report = pChar->GetCombinedReport();
		std::vector<wchar_t> wreport;
		wreport.resize(report.size() + 1, 0);
		::MultiByteToWideChar(CP_UTF8, 0, report.c_str(), report.length(),
			&wreport[0], wreport.size());

		m_pEdit->SetWindowTextW(&wreport[0]);
	}
}

void CCharSheetDlg::OnCbnSelchangeGuildCombo()
{
	UpdateTextEdit();
}


void CCharSheetDlg::OnCbnSelchangeSubguildCombo()
{
	UpdateTextEdit();
}

void CCharSheetDlg::OnClose()
{
	StoreWindowDims();
	CDialog::OnClose();
}

void CCharSheetDlg::OnOptionsSelectfont()
{
	CFontDialog font_dialog(&m_lfLogFont);
	if (IDOK == font_dialog.DoModal())
	{
		memset(&m_lfLogFont, 0, sizeof(LOGFONT));
		font_dialog.GetCurrentFont(&m_lfLogFont);
		m_editfont.CreateFontIndirect(&m_lfLogFont);

		if (StoreFontRegistrySettings(m_lfLogFont.lfFaceName, static_cast<DWORD>(m_lfLogFont.lfHeight)) < 0)
		{
			MessageBox(L"Error storing font settings in registry.");
		}

		m_pEdit->SetFont(&m_editfont);
	}
}

int CCharSheetDlg::StoreWindowDims()
{
	RECT rc;
	HWND hWnd = GetSafeHwnd();
	::GetClientRect(hWnd, &rc);

	DWORD width = rc.right - rc.left;
	DWORD height = rc.bottom - rc.top;

	if (m_pRegKey->SetKeyDWORDValue(L"Dim", L"width", width) &&
		m_pRegKey->SetKeyDWORDValue(L"Dim", L"height", height))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CCharSheetDlg::LoadWindowDims()
{
	DWORD height, width;

	if (m_pRegKey->GetKeyDWORDValue(L"Dim", L"height", &height) &&
		m_pRegKey->GetKeyDWORDValue(L"Dim", L"width", &width))
	{
		RECT rc;
		::GetWindowRect(GetSafeHwnd(), &rc);
		::SetWindowPos(GetSafeHwnd(), HWND_TOP, rc.left, rc.top, width, height, 0);
		return 0;
	}
	else
	{
		return -1;
	}
}

int CCharSheetDlg::ReadFontRegistrySettings(std::wstring* pFontFace, DWORD* pFontSize)
{
	std::wstring fontfacestr;
	DWORD dwFontSize = 0;

	if (m_pRegKey->GetKeyStringValue(L"Font", L"Face", &fontfacestr) &&
		m_pRegKey->GetKeyDWORDValue(L"Font", L"Size", &dwFontSize))
	{
		pFontFace->assign(fontfacestr);
		*pFontSize = dwFontSize;
		return 0;
	}
	else
	{
		return -1;
	}
}

int CCharSheetDlg::StoreFontRegistrySettings(const wchar_t * szwFontFace, DWORD dwFontSize)
{
	if (m_pRegKey->SetKeyStringValue(L"Font", L"Face", szwFontFace) &&
		m_pRegKey->SetKeyDWORDValue(L"Font", L"Size", dwFontSize))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}