#pragma once
#include <string>
#include <optional>
#include <pqxx/pqxx>
#include "model/student.h"
#include "model/school.h"

class StudentRepo {
public:
    static void initialize(std::shared_ptr<pqxx::connection> conn);
    static bool createStudent(const Student& student);
    static std::optional<Student> findStudentById(int id);
    static std::optional<Student> findStudentByUserId(int user_id);
    static std::vector<Student> getAllStudents();
    static bool updateStudent(const Student& student);
    static bool deleteStudent(int id);

    static std::shared_ptr<pqxx::connection> conn_;

private:
    StudentRepo() = delete;  // Prevent instantiation
    StudentRepo(const StudentRepo&) = delete;  // Prevent copying
    StudentRepo& operator=(const StudentRepo&) = delete;  // Prevent assignment
    StudentRepo(StudentRepo&&) = delete;  // Prevent moving
    StudentRepo& operator=(StudentRepo&&) = delete;  // Prevent move assignment
};