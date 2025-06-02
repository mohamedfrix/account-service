#include "service/student_service.h"
#include "repository/student_repo.h"
#include "repository/school_repo.h"
#include "util/minio.h"
#include <nlohmann/json.hpp>

std::tuple<bool, Student> StudentService::createStudent(int user_id, 
    int school_id, 
    const std::string& graduation_year, 
    const std::string& wilaya
) {

        auto school = SchoolRepo::getSchoolById(school_id);
        if (!school.has_value()){
            std::tuple result = {false, Student()};
            std::cerr << "School with id: " << school_id << " not found." << std::endl;
            return result;
        }


        auto studentIfExist = StudentRepo::findStudentByUserId(user_id);
        if (studentIfExist.has_value()){
            std::tuple<bool, Student> result = {false, studentIfExist.value()};
            std::cerr << "Student with same user id: " << user_id << " already exists" << std::endl;
            return result;
        }

        const Student student(
            0,
            user_id,
            school.value(),
            graduation_year,
            wilaya,
            // profile_picture_uploaded,
            // secondary_picture_uploaded,
            "",
            "",
            ""
        );

        bool creatingStudentResult = StudentRepo::createStudent(student);
        if (!creatingStudentResult) {
            std::tuple<bool, Student> result = {false, Student()};
            std::cerr << "Failed to create a student" << std::endl;
            return result;
        }

        auto studentFormDb = StudentRepo::findStudentByUserId(user_id);
        if (!studentFormDb.has_value()){
            std::tuple<bool, Student> result = {false, Student()};
            std::cerr << "Failed to create student: " << "can not get the student from db" << std::endl;
            return result;
        }

        Student studentResult = studentFormDb.value();

        std::tuple<bool, Student> result = {true, studentResult};
        return result;
    }


std::tuple<bool, Student> StudentService::updateStudent(
    int user_id, 
    std::optional<int> school_id, 
    const std::optional<std::string>& graduation_year, 
    const std::optional<std::string>& wilaya,
    const std::optional<std::vector<uint8_t>> profile_pic_data, 
    const std::optional<std::string>& profile_pic_type,
    const std::optional<std::vector<uint8_t>> secondary_pic_data,
    const std::optional<std::string>& secondary_pic_type
) {
   auto studentOpt = StudentRepo::findStudentByUserId(user_id);
   if (!studentOpt.has_value()){
       std::tuple<bool, Student> result = {false, Student()};
       std::cerr << "Student with User Id: " << user_id << " not found." << std::endl;
       return result;
   }

   Student student = studentOpt.value();

    if (school_id.has_value()){
        auto schoolOpt = SchoolRepo::getSchoolById(school_id.value());
        if (!schoolOpt.has_value()){
            std::tuple<bool, Student> result = {false, Student()};
            std::cerr << "School with id: " << school_id.value() << " not found." << std::endl;
            return result;
        }
        student.setSchool(schoolOpt.value());
    }
    if (graduation_year.has_value()){
        student.setGraduationYear(graduation_year.value());
    }

    if (wilaya.has_value()){
        student.setWilaya(wilaya.value());
    }

    if (profile_pic_data.has_value()){
        std::string newProfilePicPath = MinioService::uploadFile("profile-pictures", 
            profile_pic_data.value(), 
            profile_pic_type.value());
        if (newProfilePicPath.empty()) {
            std::tuple<bool, Student> result = {false, Student()};
            std::cerr << "Failed to upload profile picture." << std::endl;
            return result;
        }
        student.setProfilePicturePath(newProfilePicPath);
    }

    if (secondary_pic_data.has_value()){
        std::string newSecondaryPicPath = MinioService::uploadFile("secondary-profile-pictures", 
            secondary_pic_data.value(), 
            secondary_pic_type.value());
        if (newSecondaryPicPath.empty()) {
            std::tuple<bool, Student> result = {false, Student()};
            std::cerr << "Failed to upload secondary picture." << std::endl;
            return result;
        }
        student.setSecondaryProfilePicturePath(newSecondaryPicPath);
    }


    // Update the student in the database
    bool updateResult = StudentRepo::updateStudent(student);
    if (!updateResult) {
        std::tuple<bool, Student> result = {false, Student()};
        std::cerr << "Failed to update student" << std::endl;
        return result;
    }
    auto studentFormDb = StudentRepo::findStudentById(student.getId());
    if (!studentFormDb.has_value()){
        std::tuple<bool, Student> result = {false, Student()};
        std::cerr << "Failed to update student: " << "can not get the student from db" << std::endl;
        return result;
    }
    Student studentResult = studentFormDb.value();
    return {true, studentResult};
}

std::optional<Student> StudentService::getStudentById(int student_id) {
    auto studentOpt = StudentRepo::findStudentById(student_id);
    if (!studentOpt.has_value()){
        std::cerr << "Student with id: " << student_id << " not found." << std::endl;
        return std::nullopt;
    }
    Student student = studentOpt.value();
    return student;
}

std::optional<Student> StudentService::getStudentByUserId(int user_id) {
    auto studentOpt = StudentRepo::findStudentByUserId(user_id);
    if (!studentOpt.has_value()){
        std::cerr << "Student with id: " << user_id << " not found." << std::endl;
        return std::nullopt;
    }
    Student student = studentOpt.value();
    return student;
}