#include "repository/student_repo.h"
#include "model/student.h"
#include "model/school.h"
#include <iostream>
#include <pqxx/pqxx>


std::shared_ptr<pqxx::connection> StudentRepo::conn_;
void StudentRepo::initialize(std::shared_ptr<pqxx::connection> conn) {
    StudentRepo::conn_ = conn;
}


bool StudentRepo::createStudent(const Student& student) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "INSERT INTO students (user_id, school_id, graduation_year, wilaya, profile_picture_path, secondary_profile_picture_path) "
            "VALUES ($1, $2, $3, $4, $5, $6)",
            student.getUserId(), student.getSchool().getId(), student.getGraduationYear(),
            student.getWilaya(), student.getProfilePicturePath(), student.getSecondaryProfilePicturePath());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating student: " << e.what() << std::endl;
        return false;
    }
}

std::optional<Student> StudentRepo::findStudentById(int id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT s.id, s.user_id, s.graduation_year, s.wilaya, s.profile_picture_path, "
            "s.secondary_profile_picture_path, sc.id, sc.name, sc.name_abbr, sc.location "
            "FROM students s "
            "JOIN schools sc ON s.school_id = sc.id "
            "WHERE s.id = $1",
            id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        School school(
            row[6].as<int>(),
            row[7].as<std::string>(),
            row[8].as<std::string>(),
            row[9].as<std::string>(),
            ""
        );

        return Student(
            row[0].as<int>(),
            row[1].as<int>(),
            school,
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>(),
            row[5].as<std::string>(),
            ""
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in findStudentById: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<Student> StudentRepo::findStudentByUserId(int user_id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT s.id, s.user_id, s.graduation_year, s.wilaya, s.profile_picture_path, "
            "s.secondary_profile_picture_path, sc.id, sc.name, sc.name_abbr, sc.location "
            "FROM students s "
            "JOIN schools sc ON s.school_id = sc.id "
            "WHERE s.user_id = $1",
            user_id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        School school(
            row[6].as<int>(),
            row[7].as<std::string>(),
            row[8].as<std::string>(),
            row[9].as<std::string>(),
            ""
        );

        return Student(
            row[0].as<int>(),
            row[1].as<int>(),
            school,
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>(),
            row[5].as<std::string>(),
            ""
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in findStudentByUserId: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<Student> StudentRepo::getAllStudents() {
    std::vector<Student> students;
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec(
            "SELECT s.id, s.user_id, s.graduation_year, s.wilaya, s.profile_picture_path, "
            "s.secondary_profile_picture_path, sc.id, sc.name, sc.name_abbr, sc.location "
            "FROM students s "
            "JOIN schools sc ON s.school_id = sc.id"
        );
        txn.commit();

        for (const auto& row : result) {
            School school(
                row[6].as<int>(),
                row[7].as<std::string>(),
                row[8].as<std::string>(),
                row[9].as<std::string>(),
                ""
            );

            students.emplace_back(
                row[0].as<int>(),
                row[1].as<int>(),
                school,
                row[2].as<std::string>(),
                row[3].as<std::string>(),
                row[4].as<std::string>(),
                row[5].as<std::string>(),
                ""
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in getAllStudents: " << e.what() << std::endl;
    }
    return students;
}

bool StudentRepo::updateStudent(const Student& student) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            // "UPDATE students SET user_id = $1, school_id = $2, graduation_year = $3, "
            // "wilaya = $4, profile_picture_path = $5, secondary_profile_picture_path = $6 "
            // "WHERE id = $7",
            "UPDATE students SET graduation_year = $1, "
            "wilaya = $2, profile_picture_path = $3, secondary_profile_picture_path = $4, school_id = $5 "
            "WHERE id = $6",
            student.getGraduationYear(),
            student.getWilaya(), student.getProfilePicturePath(), student.getSecondaryProfilePicturePath(), student.getSchool().getId(),
            student.getId()
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating student: " << e.what() << std::endl;
        return false;
    }
}

bool StudentRepo::deleteStudent(int id) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "DELETE FROM students WHERE id = $1",
            id
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting student: " << e.what() << std::endl;
        return false;
    }
}
