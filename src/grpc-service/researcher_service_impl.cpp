#include "grpc-service/researcher_service_impl.h"
#include "model/researcher.h"
#include "service/researcher_service.h"
#include <string>
#include <optional>
#include <iostream>
using grpc::Status;

namespace accounts {

    grpc::Status ResearcherServiceImpl::CreateResearcher(
        grpc::ServerContext* context,
        const CreateResearcherRequest* request,
        CreateResearcherResponse* response) {
        
        auto [success, researcher] = ::ResearcherService::createResearcher(
            request->user_id(),
            request->wilaya(),
            std::nullopt,
            std::nullopt,
            std::nullopt,
            std::nullopt
        );

        if (success) {
            response->mutable_researcher()->set_id(researcher.getId());
            response->mutable_researcher()->set_user_id(researcher.getUserId());
            response->mutable_researcher()->set_wilaya(researcher.getWilaya());
            response->mutable_researcher()->set_profile_picture_path(researcher.getProfilePicturePath());
            response->mutable_researcher()->set_secondary_profile_picture_path(researcher.getSecondaryProfilePicturePath());
            response->set_success(true);
            return Status::OK;
        }

        response->set_success(false);
        return Status(grpc::StatusCode::INTERNAL, "Failed to create researcher");
    }

    grpc::Status ResearcherServiceImpl::GetResearcher(
        grpc::ServerContext* context,
        const GetResearcherRequest* request,
        GetResearcherResponse* response) {
        
        auto researcher = ::ResearcherService::getResearcherById(request->id());
        
        if (researcher.has_value()) {
            response->mutable_researcher()->set_id(researcher->getId());
            response->mutable_researcher()->set_user_id(researcher->getUserId());
            response->mutable_researcher()->set_wilaya(researcher->getWilaya());
            response->mutable_researcher()->set_profile_picture_path(researcher->getProfilePicturePath());
            response->mutable_researcher()->set_secondary_profile_picture_path(researcher->getSecondaryProfilePicturePath());
            return Status::OK;
        }

        return Status(grpc::StatusCode::NOT_FOUND, "Researcher not found");
    }

    grpc::Status ResearcherServiceImpl::UpdateResearcher(
        grpc::ServerContext* context,
        const UpdateResearcherRequest* request,
        UpdateResearcherResponse* response) {

        std::optional<std::vector<uint8_t>> profile_pic_data;
        std::optional<std::string> profile_pic_type;

        std::optional<std::vector<uint8_t>> secondary_profile_pic_data;
        std::optional<std::string> secondary_profile_pic_type;

        std::optional<std::string> wilaya;



        if (request->has_profile_picture()) {
            profile_pic_data = std::vector<uint8_t>(
                request->profile_picture().content().begin(),
                request->profile_picture().content().end()
            );
            profile_pic_type = request->profile_picture().content_type();

        }

        if (request->has_secondary_profile_picture()) {
            secondary_profile_pic_data = std::vector<uint8_t>(
                request->secondary_profile_picture().content().begin(),
                request->secondary_profile_picture().content().end()
            );
            secondary_profile_pic_type = request->secondary_profile_picture().content_type();
        }

        if (request->has_wilaya()){
            wilaya = request->wilaya();
        }
        
        auto [success, researcher] = ::ResearcherService::updateResearcher(
            request->id(),
            wilaya,
            profile_pic_data,
            profile_pic_type,
            secondary_profile_pic_data,
            secondary_profile_pic_type
        );

        if (success) {
            response->mutable_researcher()->set_id(researcher.getId());
            response->mutable_researcher()->set_user_id(researcher.getUserId());
            response->mutable_researcher()->set_wilaya(researcher.getWilaya());
            response->mutable_researcher()->set_profile_picture_path(researcher.getProfilePicturePath());
            response->mutable_researcher()->set_secondary_profile_picture_path(researcher.getSecondaryProfilePicturePath());
            return Status::OK;
        }

        return Status(grpc::StatusCode::INTERNAL, "Failed to update researcher");
    }

    grpc::Status ResearcherServiceImpl::GetResearcherByUserId(
        grpc::ServerContext* context,
        const GetResearcherByUserIdRequest* request,
        GetResearcherByUserIdResponse* response) {
        
        auto researcher = ::ResearcherService::getResearcherByUserId(request->user_id());
        
        if (researcher.has_value()) {
            response->mutable_researcher()->set_id(researcher->getId());
            response->mutable_researcher()->set_user_id(researcher->getUserId());
            response->mutable_researcher()->set_wilaya(researcher->getWilaya());
            response->mutable_researcher()->set_profile_picture_path(researcher->getProfilePicturePath());
            response->mutable_researcher()->set_secondary_profile_picture_path(researcher->getSecondaryProfilePicturePath());
            return Status::OK;
        }

        return Status(grpc::StatusCode::NOT_FOUND, "Researcher not found");
    }
}