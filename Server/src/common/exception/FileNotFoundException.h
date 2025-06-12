#pragma once

#include <exception>
#include <string>

class FileNotFoundException : public std::exception
{
public:
	explicit FileNotFoundException(const std::string &path);
	const char *what() const noexcept override;

private:
	const std::string message;
};