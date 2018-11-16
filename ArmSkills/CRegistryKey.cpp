#include "pch.h"
#include "CRegistryKey.h"

CRegistryKey::CRegistryKey(const wchar_t * subkeypath, HKEY key) :
	m_subkeypath(subkeypath)
{
	if (ERROR_SUCCESS != ::RegOpenKey(key, subkeypath, &m_hKey))
	{
		//Couldn't open key; we need to create it
		if (ERROR_SUCCESS != ::RegCreateKey(key, subkeypath, &m_hKey))
		{
			throw std::exception("Couldn't open or create registry key.");
		}
	}
}

bool CRegistryKey::GetKeyStringValue(const wchar_t * subkey, const wchar_t * valuename, std::wstring * out)
{
	wchar_t szBuf[256] = { 0 };
	DWORD dwBufSize = 256 * sizeof(wchar_t);
	int resultcode = RegGetValue(m_hKey, subkey, valuename, RRF_RT_REG_SZ, 0, &szBuf, &dwBufSize);

	if (ERROR_SUCCESS == resultcode)
	{
		out->assign(szBuf);
		return true;
	}
	else
	{
		return false;
	}
}

bool CRegistryKey::GetKeyDWORDValue(const wchar_t * subkey, const wchar_t * valuename, DWORD * out)
{
	DWORD value;
	DWORD type;
	DWORD size = sizeof(DWORD);

	int resultcode = RegGetValue(m_hKey, subkey, valuename, RRF_RT_DWORD, &type, &value, &size);
	if (ERROR_SUCCESS == resultcode)
	{
		*out = value;
		return true;
	}
	else
	{
		return false;
	}
}

bool CRegistryKey::SetKeyStringValue(const wchar_t * subkey, const wchar_t * valuename, const wchar_t * value)
{
	DWORD valuesize = sizeof(wchar_t) * wcslen(value);
	return ERROR_SUCCESS == RegSetKeyValue(m_hKey, subkey, valuename, RRF_RT_REG_SZ, value, valuesize);
}

bool CRegistryKey::SetKeyDWORDValue(const wchar_t * subkey, const wchar_t * valuename, DWORD value)
{
	DWORD valuesize = sizeof(DWORD);
	return (ERROR_SUCCESS == ::RegSetKeyValue(m_hKey, subkey, valuename, REG_DWORD, &value, valuesize));
}
