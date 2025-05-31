#include "config/env.h"
#include <fstream>
#include <iostream>
#include <sstream>

Env& Env::getInstance() {
    static Env instance;
    return instance;
}

bool Env::load(const std::string& path) {
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Could not open " << path << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Find the position of the equals sign
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        
        // Extract key and value
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        // Remove quotes if present
        if (value.size() >= 2 && 
            ((value.front() == '"' && value.back() == '"') || 
             (value.front() == '\'' && value.back() == '\''))) {
            value = value.substr(1, value.size() - 2);
        }
        
        // Store in map
        vars_[key] = value;
    }
    
    file.close();
    return true;
}

std::string Env::get(const std::string& key, const std::string& default_value) {
    auto it = vars_.find(key);
    if (it != vars_.end()) {
        return it->second;
    }
    return default_value;
}

std::string Env::getDbConnectionString() {
    std::stringstream ss;
    ss << "host=" << get("DB_HOST", "localhost")
       << " port=" << get("DB_PORT", "5432")
       << " dbname=" << get("DB_NAME", "competitive_platform")
       << " user=" << get("DB_USER", "postgres")
       << " password=" << get("DB_PASSWORD", "password");
    return ss.str();
}

std::string Env::getJwtSecret() {
    return get("JWT_SECRET", "default_insecure_secret");
}

int Env::getJwtExpiresIn() {
    try {
        return std::stoi(get("JWT_EXPIRES_IN", "3600"));
    } catch (const std::exception&) {
        return 3600; // Default to 1 hour
    }
}