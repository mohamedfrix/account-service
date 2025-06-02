#include "grpc-service/student_profile_service_impl.h"
#include "model/student_profile.h"
#include "service/student_profile_service.h"
#include <string>
#include <optional>
#include <tuple>
#include <iostream>

using grpc::Status;

namespace accounts {



    grpc::Status StudentProfileServiceImpl::CreateStudentProfile(
        grpc::ServerContext* context,
        const CreateStudentProfileRequest* request,
        CreateStudentProfileResponse* response) {

        nlohmann::json profile_data = nlohmann::json::parse(request->profile_data());

        auto [success, student_profile] = ::StudentProfileService::createStudentProfile(request->student_id(), profile_data);

        if (!success) {
            response->set_success(false);
            response->set_error_message("Failed to create student profile");
            return Status(grpc::StatusCode::INTERNAL, "Failed to create student profile");
        }

        response->set_success(true);
        response->mutable_profile()->set_id(student_profile.getId());
        response->mutable_profile()->set_student_id(student_profile.getStudentId());
        response->mutable_profile()->set_profile_data(student_profile.getProfileData().dump());

        return Status(grpc::StatusCode::OK, "Student Profile Created");
    }

    grpc::Status StudentProfileServiceImpl::GetStudentProfile(
        grpc::ServerContext* context,
        const GetStudentProfileRequest* request,
        GetStudentProfileResponse* response) {

        
        std::optional<::StudentProfile> student_profile_opt = ::StudentProfileService::getStudentProfileByStudentId(request->id());

        if (!student_profile_opt.has_value()) {
            response->set_error_message("Student Profile Not Found For This Student ID");
            return Status(grpc::StatusCode::NOT_FOUND, "Student Profile Not Found For This Student ID");
        }

        ::StudentProfile  student_profile = student_profile_opt.value();
        response->mutable_profile()->set_id(student_profile.getId());
        response->mutable_profile()->set_student_id(student_profile.getStudentId());
        response->mutable_profile()->set_profile_data(student_profile.getProfileData().dump());

        return Status(grpc::StatusCode::OK, "Student Profile Loaded");
    }

    grpc::Status StudentProfileServiceImpl::UpdateStudentProfile(
        grpc::ServerContext* context,
        const UpdateStudentProfileRequest* request,
        UpdateStudentProfileResponse* response) {
        
        auto [success, student_profile] = ::StudentProfileService::updateStudentProfile(request->profile().student_id(), ::nlohmann::json::parse(request->profile().profile_data()));
        
        if (!success) {
            response->set_success(false);
            response->set_error_message("Failed to update student profile");
            return Status(grpc::StatusCode::INTERNAL, "Failed to update student profile");
        }

        response->set_success(true);        
    }




} // namespace accounts 