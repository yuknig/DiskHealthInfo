#pragma once
#include <atlbase.h>
#include <memory>

struct IWbemClassObject;
class WmiObjectProperties;

class WmiObject
{
public:
    WmiObject(const CComPtr<IWbemClassObject>& instance);
    ~WmiObject();

    const WmiObjectProperties* Properties();

private:
    CComPtr<IWbemClassObject> m_instance;
    std::unique_ptr<WmiObjectProperties> m_properties;
    bool m_propertiesInited = false;
};

