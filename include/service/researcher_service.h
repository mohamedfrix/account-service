#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "model/researcher.h"

class ResearcherService {
public:
    static std::tuple<bool, Researcher> createResearcher(int user_id, 
                               const std::string& wilaya, 
                               const std::optional<std::vector<uint8_t>>& profile_pic_data = std::nullopt, 
                               const std::optional<std::string>& profile_pic_type = std::nullopt,
                               const std::optional<std::vector<uint8_t>>& secondary_pic_data = std::nullopt,
                               const std::optional<std::string>& secondary_pic_type = std::nullopt
                             );

    static std::tuple<bool, Researcher> updateResearcher(int researcher_id, 
                               const std::optional<std::string>& wilaya = std::nullopt,
                               const std::optional<std::vector<uint8_t>>& profile_pic_data = std::nullopt, 
                               const std::optional<std::string>& profile_pic_type = std::nullopt,
                               const std::optional<std::vector<uint8_t>>& secondary_pic_data = std::nullopt,
                               const std::optional<std::string>& secondary_pic_type = std::nullopt
                             );

    static std::optional<Researcher> getResearcherById(int researcher_id);
    static std::optional<Researcher> getResearcherByUserId(int user_id);

};