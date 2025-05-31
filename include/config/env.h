#pragma once

#include <string>
#include <unordered_map>

class Env {
public:
    // Singleton instance
    static Env& getInstance();
    
    // Load environment variables from .env file
    bool load(const std::string& path = ".env");
    
    // Get environment variable
    std::string get(const std::string& key, const std::string& default_value = "");
    
    // Get database connection string
    std::string getDbConnectionString();
    
    // Get JWT secret
    std::string getJwtSecret();
    
    // Get JWT expiration time in seconds
    int getJwtExpiresIn();
    
private:
    // Private constructor for singleton
    Env() = default;
    
    // Disable copy/move operations
    Env(const Env&) = delete;
    Env& operator=(const Env&) = delete;
    Env(Env&&) = delete;
    Env& operator=(Env&&) = delete;
    
    // Map of environment variables
    std::unordered_map<std::string, std::string> vars_;
};