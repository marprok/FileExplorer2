#include "../inc/log.h"

namespace utils
{
    std::string Log::m_log_path = "";

    Log::Log()
    {
        if (m_log_path == "")
            m_log_path = ".felog";
        m_log_stream.open(m_log_path, std::ios::out);
    }

    Log::~Log()
    {

    }

    Log& Log::the()
    {
        static Log m_instance;

        return m_instance;
    }

    void Log::set_output(const std::string &path)
    {
        m_log_path = path;
    }

    Log& Log::operator <<(const std::string& text)
    {
        m_log_stream << text;
        return *this;
    }
}
