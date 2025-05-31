#include "repository/student_profile_repo.h"
#include "model/student_profile.h"
#include <pqxx/pqxx>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

std::shared_ptr<pqxx::connection> StudentProfileRepo::conn_;
void StudentProfileRepo::initialize(std::shared_ptr<pqxx::connection> conn) {
    StudentProfileRepo::conn_ = conn;
}

bool StudentProfileRepo::createStudentProfile(const StudentProfile& student_profile) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "INSERT INTO student_profile (student_id, profile_data) "
            "VALUES ($1, $2)",
            student_profile.getStudentId(), student_profile.getProfileData().dump()
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating student profile: " << e.what() << std::endl;
        return false;
    }
}

std::optional<StudentProfile> StudentProfileRepo::findStudentProfileById(int id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT id, student_id, profile_data FROM student_profile WHERE id = $1",
            id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return StudentProfile(
            row[0].as<int>(),
            row[1].as<int>(),
            nlohmann::json::parse(row[2].as<std::string>())
        );
    } catch (const std::exception& e) {
        std::cerr << "Error finding student profile by ID: " << e.what() << std::endl;
        return std::nullopt;
    }
}
std::optional<StudentProfile> StudentProfileRepo::findStudentProfileByUserId(int user_id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT sp.id, sp.student_id, sp.profile_data FROM student_profile sp "
            "JOIN students s ON sp.student_id = s.id WHERE s.user_id = $1",
            user_id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return StudentProfile(
            row[0].as<int>(),
            row[1].as<int>(),
            nlohmann::json::parse(row[2].as<std::string>())
        );
    } catch (const std::exception& e) {
        std::cerr << "Error finding student profile by user ID: " << e.what() << std::endl;
        return std::nullopt;
    }
}
std::vector<StudentProfile> StudentProfileRepo::getAllStudentProfiles() {
    std::vector<StudentProfile> student_profiles;
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec(
            "SELECT id, student_id, profile_data FROM student_profile"
        );
        txn.commit();

        for (const auto& row : result) {
            student_profiles.emplace_back(
                row[0].as<int>(),
                row[1].as<int>(),
                nlohmann::json::parse(row[2].as<std::string>())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting all student profiles: " << e.what() << std::endl;
    }
    return student_profiles;
}
bool StudentProfileRepo::updateStudentProfile(const StudentProfile& student_profile) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "UPDATE student_profile SET profile_data = $1 WHERE id = $2",
            student_profile.getProfileData().dump(),
            student_profile.getId()
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating student profile: " << e.what() << std::endl;
        return false;
    }
}
bool StudentProfileRepo::deleteStudentProfile(int id) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "DELETE FROM student_profile WHERE id = $1",
            id
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting student profile: " << e.what() << std::endl;
        return false;
    }
}
