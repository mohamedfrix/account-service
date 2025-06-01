#pragma once

#include <nlohmann/json.hpp>


class StudentProfile {
public:

    StudentProfile() = default;
    StudentProfile(const int id, const int student_id, nlohmann::json profile_data) : 
        id(id), 
        student_id(student_id), 
        profile_data(std::move(profile_data)) {}
        

    int getId() const{ return id; }
    int getStudentId() const{ return student_id; }
    nlohmann::json getProfileData() const{ return profile_data; }

    void setProfileData(nlohmann::json new_profile_data) { profile_data = new_profile_data; }

    nlohmann::json toJson() const {
        nlohmann::json j;
        j["id"] = id;
        j["student_id"] = student_id;
        j["profile_data"] = profile_data;
        return j;
    }
private:
    int id;
    int student_id;
    nlohmann::json profile_data;
};