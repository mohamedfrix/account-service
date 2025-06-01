#include "service/researcher_service.h"
#include "repository/researcher_repo.h"
#include "model/researcher.h"
#include <tuple>
#include <vector>
#include <tuple>
#include <iostream>
#include <optional>
#include "util/minio.h"


std::tuple<bool, Researcher> createResearcher(
    int user_id, 
    const std::string& wilaya, 
    const std::optional<std::vector<uint8_t>>& profile_pic_data,
    const std::optional<std::string>& profile_pic_type,  
    const std::optional<std::vector<uint8_t>>& secondary_pic_data,
    const std::optional<std::string>& secondary_pic_type) {

    std::optional<Researcher> researcherIfExists = ResearcherRepo::getResearcherByUserId(user_id);
    if (researcherIfExists.has_value()) {
        std::cerr << "Researcher with user_id: " << user_id << " already exists." << std::endl;
        return {false, Researcher()};
    }
    Researcher researcher = Researcher(0, user_id, wilaya, "", "", ""); // Initialize with empty paths
    if (profile_pic_data.has_value() && profile_pic_type.has_value()) {
        std::string profile_pic_path = MinioService::uploadFile(
            "researcher-profile-pics", 
            profile_pic_data.value(), 
            profile_pic_type.value()
        );
        researcher.setProfilePicturePath(profile_pic_path);
    }

    if (secondary_pic_data.has_value() && secondary_pic_type.has_value()) {
        std::string secondary_pic_path = MinioService::uploadFile(
            "researcher-secondary-profile-pics", 
            secondary_pic_data.value(), 
            secondary_pic_type.value()
        );
        researcher.setSecondaryProfilePicturePath(secondary_pic_path);
    }

    bool success = ResearcherRepo::addResearcher(researcher);
    if (!success) {
        std::cerr << "Failed to create researcher" << std::endl;
        return {false, Researcher()};
    }
    auto researcher_from_db = ResearcherRepo::getResearcherByUserId(user_id);
    if (!researcher_from_db.has_value()) {
        std::cerr << "Failed to create researcher: can not get the researcher from db" << std::endl;
        return {false, Researcher()};
    }
    Researcher researcher_result = researcher_from_db.value();
    return {true, researcher_result};
}

std::tuple<bool, Researcher> ResearcherService::updateResearcher(
    int researcher_id,
    const std::optional<std::string>& wilaya,
    const std::optional<std::vector<uint8_t>>& profile_pic_data,
    const std::optional<std::string>& profile_pic_type,
    const std::optional<std::vector<uint8_t>>& secondary_pic_data,
    const std::optional<std::string>& secondary_pic_type
) {
    std::optional<Researcher> researcherOpt = ResearcherRepo::getResearcherById(researcher_id);
    if (!researcherOpt.has_value()) {
        std::cerr << "Researcher with id: " << researcher_id << " not found." << std::endl;
        return {false, Researcher()};
    }

    Researcher researcher = researcherOpt.value();
    
    if (wilaya.has_value()) {
        researcher.setWilaya(wilaya.value());
    }

    if (profile_pic_data.has_value() && profile_pic_type.has_value()) {
        std::string profile_pic_path = MinioService::uploadFile(
            "researcher-profile-pics", 
            profile_pic_data.value(), 
            profile_pic_type.value()
        );
        researcher.setProfilePicturePath(profile_pic_path);
    }

    if (secondary_pic_data.has_value() && secondary_pic_type.has_value()) {
        std::string secondary_pic_path = MinioService::uploadFile(
            "researcher-secondary-profile-pics", 
            secondary_pic_data.value(), 
            secondary_pic_type.value()
        );
        researcher.setSecondaryProfilePicturePath(secondary_pic_path);
    }

    bool success = ResearcherRepo::updateResearcher(researcher);
    if (!success) {
        std::cerr << "Failed to update researcher" << std::endl;
        return {false, Researcher()};
    }
    
    auto updated_researcher = ResearcherRepo::getResearcherById(researcher_id);
    if (!updated_researcher.has_value()) {
        std::cerr << "Failed to update researcher: can not get the updated researcher from db" << std::endl;
        return {false, Researcher()};
    }
    
    return {true, updated_researcher.value()};
}

std::optional<Researcher> ResearcherService::getResearcherById(int researcher_id) {
    return ResearcherRepo::getResearcherById(researcher_id);
}
std::optional<Researcher> ResearcherService::getResearcherByUserId(int user_id) {
    return ResearcherRepo::getResearcherByUserId(user_id);
}