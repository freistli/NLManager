#pragma once
#include <netlistmgr.h>
#include <string>
#include <vector>

class CoNetworkEventHandler;


class CNW_Settings
{

public:

	CNW_Settings() :m_bInternet(false),m_strCat(L""),m_strConnectivity(L""), m_strDescription(L""), m_strDomain(L""),m_strName(L"")
	{

	}

	bool GetInternetState () { return m_bInternet;}
	std::wstring GetNWCategory() { return m_strCat;}
	std::wstring GetConnectivityType() { return m_strConnectivity;}
	std::wstring GetNetworkName(){return m_strName;}
	std::wstring GetDescription(){ return m_strDescription;}
	std::wstring GetDomain() {return m_strDomain;}
	std::wstring GetCreationTime( ){return SystemTimeToString(m_stCreated);}
	std::wstring GetConnectionTime() {return SystemTimeToString(m_stConnected);}
	std::wstring GetNetworkID() {return m_strGUID;}


	void SetInternetState (bool bVal) { m_bInternet = bVal;}
	void SetNWCategory(std::wstring strVal) { m_strCat = strVal;}
	void SetConnectivityType(std::wstring strVal) { m_strConnectivity = strVal;}
	void SetNetworkName(std::wstring strVal){m_strName = strVal;}
	void SetDescription(std::wstring strVal){m_strDescription = strVal;}
	void SetDomain(std::wstring strVal) {m_strDomain = strVal;}
	void SetCreationTim(SYSTEMTIME st ){m_stCreated = st;}
	void SetConnectionTime(SYSTEMTIME st) {m_stConnected = st;}
	void SetNWID(std::wstring strVal) {m_strGUID = strVal;}

	std::wstring SystemTimeToString(const SYSTEMTIME& sysTime)
	{
		WCHAR strTime[MAX_PATH] = {0};
		wsprintf (strTime, L"%d / %d / %d\n", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
		std::wstring wstrTime = strTime;
		return wstrTime;

	}
	
private :
	bool m_bInternet;
	std::wstring m_strCat;
	std::wstring m_strConnectivity;
	std::wstring m_strDescription;
	std::wstring m_strDomain;
	std::wstring m_strName;
	SYSTEMTIME m_stCreated, m_stConnected;
	std::wstring m_strGUID;
	GUID m_nwGuid ;

};

typedef std::vector<CNW_Settings> Networks;

class CNLMHelper
{
public:

	CNLMHelper(void);
    ~CNLMHelper(void);
	Networks Get_NW_Info();
	
private:
	void  Dump_NW_Info(CComPtr<INetwork>pNetwork);
	std::wstring Get_NW_Category  (const NLM_NETWORK_CATEGORY& eNlmCat);
	std::wstring Get_NW_Domain_Type (const NLM_DOMAIN_TYPE& eNlmDom);
	static std::wstring Get_NW_Connectivity_Type(const NLM_CONNECTIVITY& conType);
	static std::wstring SystemTimeToString(const SYSTEMTIME& sysTime);

private:
	CComPtr <INetworkListManager> m_pNLM;
	CComPtr <INetworkEvents> m_pNetworkEvents;
	CoNetworkEventHandler * m_pSink;
	LPUNKNOWN m_pUnkSink;
	DWORD m_dwCookie;
private :
	 	Networks m_Networks;
};

