#include "CException.h"
#include <sstream>

CException::CException(int line, const char* file) noexcept
    : line(line), file(file)
{
}

const char* CException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* CException::GetType() const noexcept
{
    return "Krispy Exception";
}

int CException::GetLine() const noexcept
{
    return line;
}

const std::string& CException::GetFile() const noexcept
{
    return file;
}

std::string CException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl
        << "[Line] " << line;
    return oss.str();
}
