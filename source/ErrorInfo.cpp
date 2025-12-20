#include "ErrorInfo.hpp"

#include <cstdio>
#include <string>
#include <vector>

#include "BetterConversion.hpp"

vwstr ErrorInfo::m_errors;

void ErrorInfo::addError(cwstr errorDesc, cstr from)
{
    wstr error = errorDesc + L", from: " + BetterConversion::toWideString(from);
    m_errors.push_back(error);
}

void ErrorInfo::printErrors()
{
    if(m_errors.empty())
    {
        printf("In meantime," COLOR_GREEN " no other errors occured" COLOR_RESET ".\n");
        return;
    }

    printf("In meantime, " COLOR_RED "%d other errors occured" COLOR_RESET 
        ":\n", m_errors.size());
    int index = 0;
    for(const auto &error : m_errors)
    {
        printf("  " COLOR_RED "% 5d" COLOR_RESET " - %ls\n", ++index, error.c_str());
    }
}

cvwstr ErrorInfo::getErrors()
{
    return m_errors;
}

