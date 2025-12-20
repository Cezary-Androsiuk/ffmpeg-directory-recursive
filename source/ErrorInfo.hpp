#ifndef ERROR_INFO_HPP
#define ERROR_INFO_HPP

#include <string>
#include <vector>

typedef std::string str;
typedef const std::string &cstr;
typedef std::wstring wstr;
typedef const std::wstring &cwstr;
typedef std::vector<wstr> vwstr;
typedef const std::vector<wstr> &cvwstr;

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_GREEN   "\033[32m"

#define ADD_ERROR_INFO(x) ErrorInfo::addError(x, __PRETTY_FUNCTION__)

class ErrorInfo
{
    static vwstr m_errors;
public:
    static void addError(cwstr errorDesc, cstr from);
    static void printErrors();
    static cvwstr getErrors();
};

#endif // ERROR_INFO_HPP