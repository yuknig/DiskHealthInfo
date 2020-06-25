#pragma once
#include <atlbase.h>

#include <memory>
#include <vector>

struct IEnumWbemClassObject;
class WmiObject;

class WmiQuery
{
public:
    WmiQuery(const CComPtr<IEnumWbemClassObject>& enumerator);
    ~WmiQuery();

    using ObjectCollection = std::vector<std::shared_ptr<WmiObject>>;
    const ObjectCollection& GetObjects() const;

private:
    static ObjectCollection ReadObjects(const CComPtr<IEnumWbemClassObject>& enumerator);

private:
    ObjectCollection m_queriedInstances;
};
