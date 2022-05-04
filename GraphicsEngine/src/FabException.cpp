#include "GraphicsEngine.pch.h"
#include "FabException.h"
#include <sstream>

FabException::FabException(int aLine, const char* aFile) noexcept
    : myLine(aLine), myFile(aFile)
{
}

const char* FabException::what() const noexcept
{
    std::stringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    myWhatBuffer = oss.str();
    return myWhatBuffer.c_str();
}

const char* FabException::GetType() const noexcept
{
    return "Fab Exception";
}

int FabException::GetLine() const noexcept
{
    return myLine;
}

const std::string& FabException::GetFile() const noexcept
{
    return myFile;
}

std::string FabException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << myFile << std::endl
        << "[Line] " << myLine;
    return oss.str();
}
