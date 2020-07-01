#pragma once
#include <atlcom.h>

#include "netlistmgr.h"
#include <netlistmgr.h>
//#include <netlistmgr.idl>


class CoNetworkEventHandler :
	public INetworkListManagerEvents,public INetworkEvents
{
public:
	CoNetworkEventHandler(void);
	virtual ~CoNetworkEventHandler(void);
	STDMETHODIMP QueryInterface (REFIID riid, void** pIFace);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP ConnectivityChanged(NLM_CONNECTIVITY NewConnectivity);
	STDMETHODIMP NetworkAdded(GUID networkId);
	STDMETHODIMP NetworkDeleted(GUID networkId);
	STDMETHODIMP NetworkConnectivityChanged(GUID networkId,	NLM_CONNECTIVITY newConnectivity);
	STDMETHODIMP NetworkPropertyChanged(GUID networkId,NLM_NETWORK_PROPERTY_CHANGE flags);
private:
	long m_lRefCnt;
	DWORD m_dwCookie;

};

