#pragma once
#include <exception>
#include <string>

// Manage errors and collect data about it to present it
class CException : public std::exception
{
public:
	CException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

// TODO:
// Apparently, throw has been deprecated in C++17 and removed in C++20.
// For the future, I would like to rewrite my error-log process.
// Perhaps, to a seperate console application to "throw" messages in.
// In that way, I could clearly see what has happened during the whole runtime to detect issues.