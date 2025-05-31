#pragma once
#include <string>
#include <optional>
#include <pqxx/pqxx>
#include "model/student_profile.h"



class StudentProfileRepo {
public:
    static void initialize(std::shared_ptr<pqxx::connection> conn);
    static bool createStudentProfile(const StudentProfile& student_profile);
    static std::optional<StudentProfile> findStudentProfileById(int id);
    static std::optional<StudentProfile> findStudentProfileByUserId(int user_id);
    static std::vector<StudentProfile> getAllStudentProfiles();
    static bool updateStudentProfile(const StudentProfile& student_profile);
    static bool deleteStudentProfile(int id);

    static std::shared_ptr<pqxx::connection> conn_;

private:
    StudentProfileRepo() = delete;  // Prevent instantiation
    StudentProfileRepo(const StudentProfileRepo&) = delete;  // Prevent copying
    StudentProfileRepo& operator=(const StudentProfileRepo&) = delete;  // Prevent assignment
    StudentProfileRepo(StudentProfileRepo&&) = delete;  // Prevent moving
    StudentProfileRepo& operator=(StudentProfileRepo&&) = delete;  // Prevent move assignment
};