#pragma once
#include <tuple>
#include <string>
#include <vector>
#include "model/school.h"
#include "repository/school_repo.h"

class SchoolService {
public:
    
    static std::tuple<bool, School> createSchool(
        const std::string& name,
        const std::string& name_abbr,
        const std::string& location
    );
    static std::optional<School> getSchoolById(int id);
    static std::vector<School> getAllSchools();
    static std::tuple<bool, School> deleteSchool(int id);
};