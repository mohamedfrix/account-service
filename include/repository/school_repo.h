#pragma once
#include <string>
#include <vector>
#include <optional>
#include <pqxx/pqxx>
#include "model/school.h"

class SchoolRepo {
public:
    static void initialize(std::shared_ptr<pqxx::connection> conn);
    static bool createSchool(const School& school);
    static std::optional<School> findSchoolByName(const std::string& name);
    static std::optional<School> getSchoolById(int id);
    static std::vector<School> getAllSchools();
    static bool updateSchool(const School& school);
    static bool deleteSchool(int id);

    static std::shared_ptr<pqxx::connection> conn_;

private:
    SchoolRepo() = delete;  // Prevent instantiation
    SchoolRepo(const SchoolRepo&) = delete;  // Prevent copying
    SchoolRepo& operator=(const SchoolRepo&) = delete;  // Prevent assignment
    SchoolRepo(SchoolRepo&&) = delete;  // Prevent moving
    SchoolRepo& operator=(SchoolRepo&&) = delete;  // Prevent move assignment
};