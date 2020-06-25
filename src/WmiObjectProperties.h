#pragma once
#include "IPropertyCollection.h"
#include <atlcomcli.h>
#include <map>

struct IWbemClassObject;

class WmiObjectProperties : public IPropertyCollection
{
public:
    WmiObjectProperties(const CComPtr<IWbemClassObject>& object);

    bool PropertyExists(const std::wstring& name) const override;
    std::wstring GetPropertyText(const std::wstring& name) const override;

private:
    std::map<std::wstring, CComVariant> m_properties;
};
