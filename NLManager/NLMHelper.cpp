#include "stdafx.h"
#include "NLMHelper.h"
#include "CoNetworkEventHandler.h"

#include <afxctl.h>

// Constructor & Destructor
	CNLMHelper::CNLMHelper(void) : m_pNLM(NULL), m_pSink(NULL), m_pUnkSink(NULL),m_dwCookie(0)
	{
		 
		m_pNLM.CoCreateInstance(CLSID_NetworkListManager);
		m_pSink = new CoNetworkEventHandler();
		if (SUCCEEDED (m_pSink->QueryInterface(IID_INetworkEvents, (void**) &m_pUnkSink)))
		{
			AfxConnectionAdvise (m_pNLM, __uuidof(INetworkEvents), m_pUnkSink, FALSE, &m_dwCookie); // Advising for Events
		}
		 
	}

	CNLMHelper:: ~CNLMHelper(void)
	{
		AfxConnectionUnadvise (m_pNLM, IID_INetworkListManagerEvents, m_pUnkSink, FALSE, m_dwCookie);
		m_pSink->Release();
				
	}
	
std::wstring CNLMHelper:: Get_NW_Connectivity_Type(const NLM_CONNECTIVITY& conType)
	{
		std::wstring strRet = L"";

		if(conType == NLM_CONNECTIVITY_DISCONNECTED)
		{
			strRet += L"Network Disconnected ";
		}
		if (conType & NLM_CONNECTIVITY_IPV4_NOTRAFFIC)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV4 - No Traffic ";
		}
		if (conType & NLM_CONNECTIVITY_IPV6_NOTRAFFIC)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV6 - No Traffic ";
		}
		if (conType & NLM_CONNECTIVITY_IPV4_SUBNET)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV4 - Subnet ";
			
		}
		if (conType & NLM_CONNECTIVITY_IPV4_LOCALNETWORK)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV4 Local Connectivity ";
		}
		if (conType & NLM_CONNECTIVITY_IPV4_INTERNET)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV4 Internet Connectivity ";
		}
		if (conType & NLM_CONNECTIVITY_IPV6_SUBNET)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV6 - Subnet ";
		}
		if (conType & NLM_CONNECTIVITY_IPV6_LOCALNETWORK)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV6 local Connectivity ";
		}
		if (conType & NLM_CONNECTIVITY_IPV6_INTERNET)
		{
			if(!strRet.empty())
				strRet += L"| ";
			strRet+= L"IPV6 Internet Connectivity ";
		}
		
		return strRet;
	}


std::wstring CNLMHelper :: Get_NW_Category (const NLM_NETWORK_CATEGORY& eNlmCat)
{
	std::wstring strCat = L"";
	if( eNlmCat == NLM_NETWORK_CATEGORY_PUBLIC)
	{
		strCat += L"Public Untrusted Network";
	}
	if( eNlmCat & NLM_NETWORK_CATEGORY_PRIVATE)
	{
		strCat += L"Private trusted Network";
	}
	if( eNlmCat & NLM_NETWORK_CATEGORY_DOMAIN_AUTHENTICATED)
	{
		strCat += L"Domain Network";
	}

	return strCat;
}

std::wstring CNLMHelper :: Get_NW_Domain_Type (const NLM_DOMAIN_TYPE& eNlmDom)
{
	std::wstring strDom = L"";
	if (eNlmDom == NLM_DOMAIN_TYPE_NON_DOMAIN_NETWORK)
	{
		strDom += L"Not a Domain Network";
	}
	if (eNlmDom & NLM_DOMAIN_TYPE_DOMAIN_NETWORK)
	{
		strDom += L"Domain Network but PC Not Authenticated";
	}
	if (eNlmDom & NLM_DOMAIN_TYPE_DOMAIN_AUTHENTICATED)
	{
		strDom += L"Domain Network and PC Authenticated";
	}

	return strDom;

}


// Returns collection of networks
Networks CNLMHelper :: Get_NW_Info()
	{
		HRESULT hr;
		m_Networks.clear();
		if(m_pNLM)
		{			
			CComPtr<IEnumNetworks> pEnumNetworks;
			if(SUCCEEDED(m_pNLM->GetNetworks(NLM_ENUM_NETWORK_CONNECTED, &pEnumNetworks)))
			{
				
				DWORD dwReturn = 0;
				while(true)
				{
					CComPtr<INetwork> pNetwork;
				 	hr = pEnumNetworks->Next(1, &pNetwork, &dwReturn);
					if(hr == S_OK && dwReturn > 0)
					{
						Dump_NW_Info(pNetwork);
					}
					else
					{
						break;
					}
				
				}

			}
			
		}

		return m_Networks;
	}

void CNLMHelper::Dump_NW_Info(CComPtr<INetwork>pNetwork)
{
	
	CNW_Settings network;
	FILETIME ftCreated, ftConnected;
	SYSTEMTIME stCreated, stConnected;
	GUID nwGUID;

	HRESULT hr;

	if(!pNetwork) return;

	NLM_NETWORK_CATEGORY enCat;
	pNetwork->GetCategory(&enCat);
	network.SetNWCategory(Get_NW_Category (enCat));

	NLM_CONNECTIVITY enConnectivity;
	pNetwork->GetConnectivity (&enConnectivity);
	network.SetConnectivityType(Get_NW_Connectivity_Type(enConnectivity));
	
	_bstr_t bstrDesc;
	pNetwork->GetDescription(bstrDesc.GetAddress());
	network.SetDescription(bstrDesc.GetBSTR());

	NLM_DOMAIN_TYPE enDomain;
	pNetwork->GetDomainType (&enDomain);
	network.SetDomain(Get_NW_Domain_Type (enDomain));

	_bstr_t bstrName;
	pNetwork->GetName(bstrName.GetAddress());
	network.SetNetworkName (bstrName.GetBSTR());

	pNetwork->GetTimeCreatedAndConnected (&ftCreated.dwLowDateTime, &ftCreated.dwHighDateTime, &ftConnected.dwLowDateTime, &ftConnected.dwHighDateTime);
	FileTimeToSystemTime(& ftCreated, &stCreated);
	FileTimeToSystemTime(& ftConnected, &stConnected);
	network.SetCreationTim(stCreated);
	network.SetConnectionTime(stConnected);

	pNetwork->GetNetworkId(&nwGUID);
	WCHAR strGuid[40] = {0};
	StringFromGUID2(nwGUID, strGuid, 40 );
	std::wstring wstrGuid = strGuid;
	network.SetNWID (strGuid);

	VARIANT_BOOL vIsInternet;
	pNetwork->get_IsConnectedToInternet(&vIsInternet);
	if (vIsInternet == VARIANT_TRUE)
	{
		network.SetInternetState(true);
	}
	m_Networks.push_back(network);
			
}

std::wstring CNLMHelper:: SystemTimeToString(const SYSTEMTIME& sysTime)
{
	WCHAR strTime[MAX_PATH] = {0};
	wsprintf (strTime, L"%d / %d / %d\n", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	std::wstring wstrTime = strTime;
	return wstrTime;

}

