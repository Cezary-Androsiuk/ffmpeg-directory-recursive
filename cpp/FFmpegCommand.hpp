#ifndef _FFMPEG_COMMAND_HPP
#define _FFMPEG_COMMAND_HPP

#include <string>


#define _CMDT(text) L"\"" + (text) + L"\" " /* command text */

#define _FFMPEG_COMMAND_PREFIX L"ffmpeg -i "
#define _FFMPEG_COMMAND_DEFAULT_CORE L"-c:v libx265 -vtag hvc1"

/// allows to print simpler output - usefull while reading ffmpeg pipe
#if __linux__
#define _FFMPEG_COMMAND_PROGRESS_FLAG L"-progress - "
#elif _WIN32
#define _FFMPEG_COMMAND_PROGRESS_FLAG L"-progress pipe:1 "
#endif

#define _FFMPEG_COMMAND_POSTFIX L"2>&1"; // move stderr to stdout (connect them)


class FFmpegCommand{
public:
    static inline const wchar_t *getCore() noexcept { return m_core.c_str(); }
    static inline void setCore(const std::wstring &core) noexcept { m_core = core; };

    static inline std::wstring get(const std::wstring &input, const std::wstring &output) noexcept
    {
        return 
            _FFMPEG_COMMAND_PREFIX + 
            (L"\"" + input + L"\" ") +
            m_core + L" " +
            _FFMPEG_COMMAND_PROGRESS_FLAG + 
            (L"\"" + output + L"\" ") +
            _FFMPEG_COMMAND_POSTFIX;
    }

private:
    static std::wstring m_core;
};

#endif // _FFMPEG_COMMAND_HPP