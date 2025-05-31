#pragma once

#include <string>
#include <nlohmann/json.hpp>

class School {

public:
    School() = default;
    School(int id, const std::string& name, const std::string& name_abbr,
           const std::string& location, const std::string& created_at)
        : id_(id), name_(name), name_abbr_(name_abbr),
          location_(location), created_at_(created_at) {}

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getNameAbbr() const { return name_abbr_; }
    std::string getLocation() const { return location_; }
    std::string getCreatedAt() const { return created_at_; }

    nlohmann::json toJson() const {
        nlohmann::json json;
        json["id"] = id_;
        json["name"] = name_;
        json["name_abbr"] = name_abbr_;
        json["location"] = location_;
        json["created_at"] = created_at_;
        return json;
    }

private:
    int id_;
    std::string name_;
    std::string name_abbr_;
    std::string location_;
    std::string created_at_;
};