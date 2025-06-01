#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include <vector>
#include <tuple>
#include "model/student_profile.h"

class StudentProfileService {
public:
    static std::tuple<bool, StudentProfile> createStudentProfile(int student_id, const std::optional<nlohmann::json>& profile_data);
    static std::tuple<bool, StudentProfile> updateStudentProfile(int student_id, const nlohmann::json& profile_data);
    static std::optional<StudentProfile> getStudentProfileById(int student_id);
    static std::optional<StudentProfile> getStudentProfileByUserId(int user_id);

    static nlohmann::json defaultStudentProfileData;
};