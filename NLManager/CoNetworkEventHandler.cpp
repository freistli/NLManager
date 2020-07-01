#include "stdafx.h"
#include "CoNetworkEventHandler.h"

//#include <netlistmgr.idl>

CoNetworkEventHandler::CoNetworkEventHandler(void) : m_lRefCnt(0), m_dwCookie(0)
{
}


CoNetworkEventHandler::~CoNetworkEventHandler(void)
{

}

STDMETHODIMP CoNetworkEventHandler::QueryInterface(REFIID refIID, void** pIFace)
{
	*pIFace = NULL;
	if(refIID == IID_IUnknown || refIID == __uuidof(INetworkListManagerEvents))
	{
		*pIFace =  (IUnknown*)(INetworkListManagerEvents*)(this);
	}
	else if (refIID == __uuidof(INetworkEvents))
	{		
		*pIFace = (IUnknown*)(INetworkEvents*)(this);
	}
	if (*pIFace == NULL)
	{
		return E_NOINTERFACE;
	}
	((IUnknown*)*pIFace)->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CoNetworkEventHandler::AddRef()
{
	m_lRefCnt++;
	return m_lRefCnt;
}

STDMETHODIMP_(ULONG) CoNetworkEventHandler::Release()
{
	m_lRefCnt--;
	if(m_lRefCnt == 0) 
		{
			delete this;
			return (0);
	}
	return m_lRefCnt;
}
STDMETHODIMP  CoNetworkEventHandler :: ConnectivityChanged( NLM_CONNECTIVITY NewConnectivity)
{
	bool bInternet = false;
	if((NewConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET) || (NewConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET))
	{
		bInternet = true;
	}
	PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, UM_NETWORK_NOTIFY, (WPARAM) bInternet, 0);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CoNetworkEventHandler::NetworkAdded(GUID networkId)
{
	bool bInternet = false;
	
	PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, UM_NETWORK_NOTIFY, (WPARAM)bInternet, 0);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CoNetworkEventHandler::NetworkDeleted(GUID networkId)
{
	bool bInternet = false;

	PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, UM_NETWORK_NOTIFY, (WPARAM)bInternet, 0);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CoNetworkEventHandler::NetworkConnectivityChanged(GUID networkId, NLM_CONNECTIVITY newConnectivity)
{
	bool bInternet = false;
	if ((newConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET) || (newConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET))
	{
		bInternet = true;
	}
	PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, UM_NETWORK_NOTIFY, (WPARAM)bInternet, 0);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CoNetworkEventHandler::NetworkPropertyChanged(GUID networkId, NLM_NETWORK_PROPERTY_CHANGE flags)
{
	bool bInternet = false;
	
	PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, UM_NETWORK_NOTIFY, (WPARAM)bInternet, 0);
	return S_OK;
}
 
