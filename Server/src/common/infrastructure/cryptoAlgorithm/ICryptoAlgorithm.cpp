#include "ICryptoAlgorithm.h"

#include "exception/FileNotFoundException.h"

#include <fstream>

const std::string ICryptoAlgorithm::KEYS_PATH = "./resources/keys/";

ICryptoAlgorithm::~ICryptoAlgorithm() = default;
