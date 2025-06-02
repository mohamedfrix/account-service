#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include <tuple>
#include <optional>
#include "model/student.h"

class StudentService {
public:
    static std::tuple<bool, Student> createStudent(int user_id, 
                              int school_id, 
                              const std::string& graduation_year, 
                              const std::string& wilaya
                            );

    static std::tuple<bool, Student> updateStudent(
                              int user_id, 
                              std::optional<int> school_id, 
                              const std::optional<std::string>& graduation_year, 
                              const std::optional<std::string>& wilaya,
                              const std::optional<std::vector<uint8_t>> profile_pic_data, 
                              const std::optional<std::string>& profile_pic_type,
                              const std::optional<std::vector<uint8_t>> secondary_pic_data,
                              const std::optional<std::string>& secondary_pic_type
                            );

      static std::optional<Student> getStudentById(int student_id);
      static std::optional<Student> getStudentByUserId(int user_id);
};