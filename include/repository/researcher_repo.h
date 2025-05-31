#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "model/researcher.h"
#include "db/database.h"
#include <vector>

class ResearcherRepo {
public:
    static void initialize(std::shared_ptr<pqxx::connection> conn);
    static std::optional<Researcher> getResearcherById(int id);
    static std::optional<Researcher> getResearcherByUserId(int user_id);
    static std::vector<Researcher> getAllResearchers();
    static bool addResearcher(const Researcher& researcher);
    static bool updateResearcher(const Researcher& researcher);
    static bool deleteResearcher(int id);

    static std::shared_ptr<pqxx::connection> conn_;

private:
    ResearcherRepo() = delete;  // Prevent instantiation
    ResearcherRepo(const ResearcherRepo&) = delete;  // Prevent copying
    ResearcherRepo& operator=(const ResearcherRepo&) = delete;  // Prevent assignment
    ResearcherRepo(ResearcherRepo&&) = delete;  // Prevent moving
    ResearcherRepo& operator=(ResearcherRepo&&) = delete;  // Prevent move assignment
};