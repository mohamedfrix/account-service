#include "grpc-service/student_service_impl.h"
#include "model/student.h"
#include <string>
#include <optional>
#include "service/student_service.h"

using grpc::Status;

namespace accounts {

    grpc::Status StudentServiceImpl::CreateStudent(
        grpc::ServerContext* context,
        const CreateStudentRequest* request,
        CreateStudentResponse* response) {
        
        auto [success, student] = ::StudentService::createStudent(
            request->user_id(),
            request->school_id(),
            request->graduation_year(),
            request->wilaya()
        );

        if (success) {
            response->mutable_student()->set_id(student.getId());
            response->mutable_student()->set_user_id(student.getUserId());
            response->mutable_student()->mutable_school()->set_id(student.getSchool().getId());
            response->mutable_student()->mutable_school()->set_name(student.getSchool().getName());
            response->mutable_student()->mutable_school()->set_name_abbr(student.getSchool().getNameAbbr());
            response->mutable_student()->mutable_school()->set_location(student.getSchool().getLocation());
            response->mutable_student()->set_graduation_year(student.getGraduationYear());
            response->mutable_student()->set_wilaya(student.getWilaya());
            response->mutable_student()->set_profile_picture_path(student.getProfilePicturePath());
            response->mutable_student()->set_secondary_profile_picture_path(student.getSecondaryProfilePicturePath());
            response->mutable_student()->set_created_at(student.getCreatedAt());

            response->set_success(true);
            return Status::OK;
        }

        response->set_success(false);
        return Status(grpc::StatusCode::ALREADY_EXISTS, "Student with this user ID already exists");
    }

    grpc::Status StudentServiceImpl::GetStudent(
        grpc::ServerContext* context,
        const GetStudentRequest* request,
        GetStudentResponse* response) {
        
        auto student = ::StudentService::getStudentById(request->id());
        
        if (student.has_value()) {
            response->mutable_student()->set_id(student->getId());
            response->mutable_student()->set_user_id(student->getUserId());
            response->mutable_student()->mutable_school()->set_id(student->getSchool().getId());
            response->mutable_student()->mutable_school()->set_name(student->getSchool().getName());
            response->mutable_student()->mutable_school()->set_name_abbr(student->getSchool().getNameAbbr());
            response->mutable_student()->mutable_school()->set_location(student->getSchool().getLocation());
            response->mutable_student()->set_graduation_year(student->getGraduationYear());
            response->mutable_student()->set_wilaya(student->getWilaya());
            response->mutable_student()->set_profile_picture_path(student->getProfilePicturePath());
            response->mutable_student()->set_secondary_profile_picture_path(student->getSecondaryProfilePicturePath());
            response->mutable_student()->set_created_at(student->getCreatedAt());

            response->set_success(true);
            return Status::OK;
        }
        
        response->set_success(false);
        return Status(grpc::StatusCode::NOT_FOUND, "Student not found");
    }

    grpc::Status StudentServiceImpl::GetStudentByUserId(
        grpc::ServerContext* context,
        const GetStudentByUserIdRequest* request,
        GetStudentByUserIdResponse* response) {
        
        auto student = ::StudentService::getStudentByUserId(request->user_id());
        
        if (student.has_value()) {
            response->mutable_student()->set_id(student->getId());
            response->mutable_student()->set_user_id(student->getUserId());
            response->mutable_student()->mutable_school()->set_id(student->getSchool().getId());
            response->mutable_student()->mutable_school()->set_name(student->getSchool().getName());
            response->mutable_student()->mutable_school()->set_name_abbr(student->getSchool().getNameAbbr());
            response->mutable_student()->mutable_school()->set_location(student->getSchool().getLocation());
            response->mutable_student()->set_graduation_year(student->getGraduationYear());
            response->mutable_student()->set_wilaya(student->getWilaya());
            response->mutable_student()->set_profile_picture_path(student->getProfilePicturePath());
            response->mutable_student()->set_secondary_profile_picture_path(student->getSecondaryProfilePicturePath());
            response->mutable_student()->set_created_at(student->getCreatedAt());

            response->set_success(true);
            return Status::OK;
        }
        
        response->set_success(false);
        return Status(grpc::StatusCode::NOT_FOUND, "Student not found");
    }

    grpc::Status StudentServiceImpl::UpdateStudent(
        grpc::ServerContext* context,
        const UpdateStudentRequest* request,
        UpdateStudentResponse* response) {

        // Convert FileData to vector<uint8_t> and string
        std::optional<std::vector<uint8_t>> profile_pic_data;
        std::optional<std::string> profile_pic_type;

        std::optional<std::vector<uint8_t>> secondary_profile_pic_data;
        std::optional<std::string> secondary_profile_pic_type;

        std::optional<int> school_id;
        std::optional<std::string> graduation_year;
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
        if (request->has_school_id()) {
            school_id = request->school_id();
        }
        if (request->has_graduation_year()) {
            graduation_year = request->graduation_year();
        }
        if (request->has_wilaya()) {
            wilaya = request->wilaya();
        }
        
        auto [success, student] = ::StudentService::updateStudent(
            request->id(),
            school_id,
            graduation_year,
            wilaya,
            profile_pic_data,
            profile_pic_type,
            secondary_profile_pic_data,
            secondary_profile_pic_type
        );

        if (success) {
            response->mutable_student()->set_id(student.getId());
            response->mutable_student()->set_user_id(student.getUserId());
            response->mutable_student()->mutable_school()->set_id(student.getSchool().getId());
            response->mutable_student()->mutable_school()->set_name(student.getSchool().getName());
            response->mutable_student()->mutable_school()->set_name_abbr(student.getSchool().getNameAbbr());
            response->mutable_student()->mutable_school()->set_location(student.getSchool().getLocation());
            response->mutable_student()->set_graduation_year(student.getGraduationYear());
            response->mutable_student()->set_wilaya(student.getWilaya());
            response->mutable_student()->set_profile_picture_path(student.getProfilePicturePath());
            response->mutable_student()->set_secondary_profile_picture_path(student.getSecondaryProfilePicturePath());
            response->mutable_student()->set_created_at(student.getCreatedAt());

            response->set_success(true);
            return Status::OK;
        }

        response->set_success(false);
        return Status(grpc::StatusCode::NOT_FOUND, "Student not found");
    }

} // namespace accounts