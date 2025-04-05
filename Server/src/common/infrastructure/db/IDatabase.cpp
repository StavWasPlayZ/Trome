#include "IDatabase.h"

IDatabase::IDatabase(const std::string& dbName) :
    _dbName(dbName)
{}

IDatabase::~IDatabase() = default;
