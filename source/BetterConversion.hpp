#ifndef BETTER_CONVERSION_HPP
#define BETTER_CONVERSION_HPP

#include <string>

class BetterConversion{
public:
    static std::wstring toWideString(const std::string &string);
    static std::string toString(const std::wstring &wstring);
};

#endif // BETTER_CONVERSION_HPP