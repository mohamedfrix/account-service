#include "repository/researcher_repo.h"
#include "db/database.h"
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include "model/researcher.h"
#include <iostream>

std::shared_ptr<pqxx::connection> ResearcherRepo::conn_;
void ResearcherRepo::initialize(std::shared_ptr<pqxx::connection> conn) {
    ResearcherRepo::conn_ = conn;
}

std::optional<Researcher> ResearcherRepo::getResearcherById(int id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT id, user_id, wilaya, profile_picture_path, secondary_profile_picture_path, created_at "
            "FROM researchers WHERE id = $1",
            id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return Researcher(
            row[0].as<int>(),
            row[1].as<int>(),
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>(),
            row[5].as<std::string>()
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in getResearcherById: " << e.what() << std::endl;
        return std::nullopt;
    }
}
std::optional<Researcher> ResearcherRepo::getResearcherByUserId(int user_id) {
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec_params(
            "SELECT id, user_id, wilaya, profile_picture_path, secondary_profile_picture_path, created_at "
            "FROM researchers WHERE user_id = $1",
            user_id
        );
        txn.commit();

        if (result.empty()) {
            return std::nullopt;
        }

        const auto& row = result[0];
        return Researcher(
            row[0].as<int>(),
            row[1].as<int>(),
            row[2].as<std::string>(),
            row[3].as<std::string>(),
            row[4].as<std::string>(),
            row[5].as<std::string>()
        );
    } catch (const std::exception& e) {
        std::cerr << "Error in getResearcherByUserId: " << e.what() << std::endl;
        return std::nullopt;
    }
}
std::vector<Researcher> ResearcherRepo::getAllResearchers() {
    std::vector<Researcher> researchers;
    try {
        pqxx::work txn{*conn_};
        auto result = txn.exec(
            "SELECT id, user_id, wilaya, profile_picture_path, secondary_profile_picture_path, created_at "
            "FROM researchers"
        );
        txn.commit();

        for (const auto& row : result) {
            researchers.emplace_back(
                row[0].as<int>(),
                row[1].as<int>(),
                row[2].as<std::string>(),
                row[3].as<std::string>(),
                row[4].as<std::string>(),
                row[5].as<std::string>()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in getAllResearchers: " << e.what() << std::endl;
    }
    return researchers;
}
bool ResearcherRepo::addResearcher(const Researcher& researcher) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "INSERT INTO researchers (user_id, wilaya, profile_picture_path, secondary_profile_picture_path, created_at) "
            "VALUES ($1, $2, $3, $4, $5)",
            researcher.getUserId(),
            researcher.getWilaya(),
            researcher.getProfilePicturePath(),
            researcher.getSecondaryProfilePicturePath(),
            researcher.getCreatedAt()
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding researcher: " << e.what() << std::endl;
        return false;
    }
}
bool ResearcherRepo::updateResearcher(const Researcher& researcher) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "UPDATE researchers SET wilaya = $1, profile_picture_path = $2, secondary_profile_picture_path = $3 "
            "WHERE id = $4",
            researcher.getWilaya(),
            researcher.getProfilePicturePath(),
            researcher.getSecondaryProfilePicturePath(),
            researcher.getId()
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating researcher: " << e.what() << std::endl;
        return false;
    }
}
bool ResearcherRepo::deleteResearcher(int id) {
    try {
        pqxx::work txn{*conn_};
        txn.exec_params(
            "DELETE FROM researchers WHERE id = $1",
            id
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting researcher: " << e.what() << std::endl;
        return false;
    }
}
