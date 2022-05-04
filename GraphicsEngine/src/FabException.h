#pragma once
#include <exception>
#include <string>
class FabException : std::exception
{
public:
	FabException(int aLine, const char* aFile) noexcept;
	const char* what()const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int myLine;
	std::string myFile;
protected:
	mutable std::string myWhatBuffer;
};

