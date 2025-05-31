#include "service/school_service.h"
#include "repository/school_repo.h"
#include "model/school.h"
#include <optional>
#include <tuple>
#include <vector>
#include <string>

std::tuple<bool, School> SchoolService::createSchool(
    const std::string& name,
    const std::string& name_abbr,
    const std::string& location
) {
   std::optional<School> existingSchool = SchoolRepo::findSchoolByName(name);
        if (existingSchool.has_value()) {
            return {false, existingSchool.value()};
        }

        School schoolArg = School(0, name, name_abbr, location, "");
        bool created = SchoolRepo::createSchool(schoolArg);
        if (!created) {
            return {false, School()};
        }
        std::optional<School> createdSchool = SchoolRepo::findSchoolByName(name);
        if (!createdSchool.has_value()) {
            return {false, School()};
        }
        return {true, createdSchool.value()};
}

std::optional<School> SchoolService::getSchoolById(int id) {
    return SchoolRepo::getSchoolById(id);
}

std::vector<School> SchoolService::getAllSchools() {
    return SchoolRepo::getAllSchools();
}

std::tuple<bool, School> SchoolService::deleteSchool(int id) {
    std::optional<School> existingSchool = SchoolRepo::getSchoolById(id);
    if (!existingSchool.has_value()) {
        return {false, School()};
    }

    bool deleted = SchoolRepo::deleteSchool(id);
    if (!deleted) {
        return {false, School()};
    }

    return {true, existingSchool.value()};
}
