#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>

class Database {
public:
    // Initialize database connection
    static void initialize(const std::string& connection_string);

    // Get connection
    static std::shared_ptr<pqxx::connection> getConnection();

    static std::shared_ptr<pqxx::connection> conn_;

private:
    // Private constructor for singleton
    Database() = default;
    
    // Disable copy/move operations
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

    // Database connection
};