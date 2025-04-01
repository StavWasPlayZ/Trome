#pragma once

#include <iostream>

// protocol codes
const unsigned int LOGIN_CODE = 1;
const unsigned int SIGNUP_CODE = 2;
const unsigned int ERROR_CODE = 400;

// json keys
const std::string STATUS_KEY = "status";
const std::string MESSAGE_KEY = "message";

// sizes
const unsigned int CODE_SIZE = sizeof(unsigned char);
const unsigned int JSON_LEN_SIZE = 4 * sizeof(unsigned char);