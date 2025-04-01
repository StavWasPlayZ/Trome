#pragma once

#include <iostream>

typedef struct LoginResponse
{
	unsigned int status;
} LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
} SignupResponse;

typedef struct ErrorRespone
{
	std::string message;
} ErrorResponse;