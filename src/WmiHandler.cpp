#include "WmiHandler.h"

#include <combaseapi.h>
#include <wbemidl.h>

#include <iostream>
#include "StringUtils.h"

#pragma comment(lib, "wbemuuid.lib")

WmiHandler::WmiHandler(const std::wstring& wmiNamespace)
{
    HRESULT hret;
    hret = CoCreateInstance(
        CLSID_WbemLocator, // or CLSID_WbemAdministrativeLocator?
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&m_locator);
    if (FAILED(hret))
    {
        auto msg = __FUNCTION__ " failed to instantiate IWbemLocator";
        throw std::runtime_error(msg);
    }

    hret = m_locator->ConnectServer(
        ToComBStr(wmiNamespace),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &m_services);
    if (FAILED(hret))
    {
        auto msg = __FUNCTION__ " failed IWbemLocator::ConnectServer call";
        std::cerr << msg << std::endl;
        throw std::runtime_error(msg);
    }

    hret = CoSetProxyBlanket(
        m_services,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hret)) {
        auto msg = __FUNCTION__ " failed CoSetProxyBlanket call";
        std::cerr << msg << std::endl;
        throw std::runtime_error(msg);
    }
}

std::unique_ptr<WmiQuery> WmiHandler::ExecQuery(const std::wstring& query)
{
    CComPtr<IEnumWbemClassObject> enumerator;
    HRESULT hret = m_services->ExecQuery(
        ToComBStr("WQL"),
        ToComBStr(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &enumerator);
    if (FAILED(hret)) {
        std::cerr << __FUNCTION__ " failed query: " << ToStringA(query) << std::endl;
        return {};
    }

    return std::make_unique<WmiQuery>(enumerator);
}