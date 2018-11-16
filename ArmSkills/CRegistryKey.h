#pragma once
#include <afxwin.h>
#include <string>

class CRegistryKey
{
public:
	CRegistryKey(const wchar_t* subkeypath, HKEY key = HKEY_CURRENT_USER);
	~CRegistryKey()
	{
		::RegCloseKey(m_hKey);
	}

	bool GetKeyStringValue(const wchar_t* subkey, const wchar_t* valuename, std::wstring* out);
	bool GetKeyDWORDValue(const wchar_t* subkey, const wchar_t* valuename, DWORD* out);
	bool SetKeyStringValue(const wchar_t* subkey, const wchar_t* valuename, const wchar_t* value);
	bool SetKeyDWORDValue(const wchar_t* subkey, const wchar_t* valuename, DWORD value);

private:
	HKEY m_hKey;
	std::wstring m_subkeypath;
};
