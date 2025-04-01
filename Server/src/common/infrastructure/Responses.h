#pragma once

#include <iostream>

typedef struct LoginRespone
{
	unsigned int status;
} LoginRespone;

typedef struct SignupRespone
{
	unsigned int status;
} SignupRespone;

typedef struct ErrorRespone
{
	std::string message;
} ErrorRespone;