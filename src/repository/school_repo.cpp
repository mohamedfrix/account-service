#include "repository/school_repo.h"
#include <iostream>
#include "model/school.h"
#include "pqxx/pqxx"


std::shared_ptr<pqxx::connection> SchoolRepo::conn_;
void SchoolRepo::initialize(std::shared_ptr<pqxx::connection> conn) {
    SchoolRepo::conn_ = conn;
}

bool SchoolRepo::createSchool(const School& school) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "INSERT INTO schools (name, name_abbr, location) "
            "VALUES ($1, $2, $3)",
            school.getName(), school.getNameAbbr(), school.getLocation());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating school: " << e.what() << std::endl;
        return false;
    }
}

std::optional<School> SchoolRepo::findSchoolByName(const std::string& name) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT id, name, name_abbr, location, created_at "
            "FROM schools WHERE name = $1",
            name
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return School(
            row[0].as<int>(),
            row[1].as<std::string>(),
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>()
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in findSchoolByName: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<School> SchoolRepo::getSchoolById(int id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT id, name, name_abbr, location, created_at "
            "FROM schools WHERE id = $1",
            id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return School(
            row[0].as<int>(),
            row[1].as<std::string>(),
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>()
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in getSchoolById: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<School> SchoolRepo::getAllSchools() {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec(
            "SELECT id, name, name_abbr, location, created_at "
            "FROM schools"
        );
        txn.commit();

        std::vector<School> schools;
        for (const auto& row : result) {
            schools.emplace_back(
                row[0].as<int>(),
                row[1].as<std::string>(),
                row[2].as<std::string>(),
                row[3].as<std::string>(),
                row[4].as<std::string>()
            );
        }
        return schools;
    } catch (const std::exception& e) {
        std::cerr << "Error in getAllSchools: " << e.what() << std::endl;
        return {};
    }
}

bool SchoolRepo::updateSchool(const School& school) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "UPDATE schools SET name = $1, name_abbr = $2, location = $3 "
            "WHERE id = $4",
            school.getName(), school.getNameAbbr(), school.getLocation(), school.getId());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating school: " << e.what() << std::endl;
        return false;
    }
}

bool SchoolRepo::deleteSchool(int id) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "DELETE FROM schools WHERE id = $1",
            id
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting school: " << e.what() << std::endl;
        return false;
    }
}