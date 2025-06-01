#include "service/student_profile_service.h"
#include "repository/student_profile_repo.h"
#include <tuple>
#include <vector>
#include <tuple>
#include <nlohmann/json.hpp>
#include "model/student.h"
#include "repository/student_repo.h"
#include "model/student_profile.h"
#include <iostream>
#include <optional>


nlohmann::json StudentProfileService::defaultStudentProfileData = {
    {"bio", ""},
    {"interests", nlohmann::json::array()},
    {"skills", nlohmann::json::array()},
    {"projects", nlohmann::json::array()},
    {"languages", nlohmann::json::array()}
};

std::tuple<bool, StudentProfile> StudentProfileService::createStudentProfile(int student_id, const std::optional<nlohmann::json>& profile_data) {
    nlohmann::json profile_data_to_insert = profile_data.value_or(StudentProfileService::defaultStudentProfileData);
    
    auto studentOpt = StudentRepo::findStudentById(student_id);
    if (!studentOpt.has_value()) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Student with id: " << student_id << " not found." << std::endl;
        return result;
    }

    Student student = studentOpt.value();

    StudentProfile student_profile(0, student.getId(), profile_data.value());
    bool success = StudentProfileRepo::createStudentProfile(student_profile);
    if (!success) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Failed to create student profile" << std::endl;
        return result;
    }

    auto student_profile_from_db = StudentProfileRepo::findStudentProfileByUserId(student.getUserId());
    if (!student_profile_from_db.has_value()) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Failed to create student profile: " << "can not get the student profile from db" << std::endl;
        return result;
    }
    StudentProfile student_profile_result = student_profile_from_db.value();
    return {true, student_profile_result};
}

std::tuple<bool, StudentProfile> StudentProfileService::updateStudentProfile(int student_id, const nlohmann::json& profile_data) {
    auto studentOpt = StudentRepo::findStudentById(student_id);
    if (!studentOpt.has_value()) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Student with id: " << student_id << " not found." << std::endl;
        return result;
    }

    Student student = studentOpt.value();

    auto student_profile_opt = StudentProfileRepo::findStudentProfileByUserId(student.getUserId());
    if (!student_profile_opt.has_value()) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Student profile for user id: " << student.getUserId() << " not found." << std::endl;
        return result;
    }

    StudentProfile student_profile = student_profile_opt.value();
    student_profile.setProfileData(profile_data);

    bool success = StudentProfileRepo::updateStudentProfile(student_profile);
    if (!success) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Failed to update student profile" << std::endl;
        return result;
    }

    auto updated_student_profile_from_db = StudentProfileRepo::findStudentProfileByUserId(student.getUserId());
    if (!updated_student_profile_from_db.has_value()) {
        std::tuple<bool, StudentProfile> result = {false, StudentProfile()};
        std::cerr << "Failed to update student profile: " << "can not get the updated student profile from db" << std::endl;
        return result;
    }
    
    StudentProfile updated_student_profile_result = updated_student_profile_from_db.value();
    return {true, updated_student_profile_result};
}

std::optional<StudentProfile> StudentProfileService::getStudentProfileById(int student_id) {
    auto student_profile_opt = StudentProfileRepo::findStudentProfileById(student_id);
    if (!student_profile_opt.has_value()) {
        std::cerr << "Student profile with id: " << student_id << " not found." << std::endl;
        return std::nullopt;
    }
    return student_profile_opt.value();
}

std::optional<StudentProfile> StudentProfileService::getStudentProfileByUserId(int user_id) {
    auto student_profile_opt = StudentProfileRepo::findStudentProfileByUserId(user_id);
    if (!student_profile_opt.has_value()) {
        std::cerr << "Student profile with user id: " << user_id << " not found." << std::endl;
        return std::nullopt;
    }
    return student_profile_opt.value();
}

