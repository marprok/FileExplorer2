#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>

namespace utils
{
    class Log
    {
    private:
        static std::string m_log_path;

        std::fstream m_log_stream;

    public:
        static Log& the();

        static void set_output(const std::string &path);

        Log& operator <<(const std::string& text);

    private:
        Log();

        ~Log();

        Log(const Log&) = delete;

        Log& operator=(const Log&) = delete;

        Log(Log&&) = delete;

        Log& operator=(Log&&) = delete;
    };
}
#endif // LOG_H
