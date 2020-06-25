#pragma once
#include <string>
#include <vector>

class IPropertyCollection
{
public:
    virtual ~IPropertyCollection() = default;

    virtual bool PropertyExists(const std::wstring& name) const = 0;
    virtual std::wstring GetPropertyText(const std::wstring& name) const = 0;
};
