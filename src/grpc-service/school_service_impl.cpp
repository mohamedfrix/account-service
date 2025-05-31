#include "grpc-service/school_service_impl.h"
#include "model/school.h"
#include <string>

using grpc::Status;

namespace accounts {

grpc::Status SchoolServiceImpl::CreateSchool(grpc::ServerContext* context,
                                           const CreateSchoolRequest* request,
                                           CreateSchoolResponse* response) {
    auto [success, school] = ::SchoolService::createSchool(
        request->name(),
        request->name_abbr(),
        request->location()
    );

    if (success) {
        response->mutable_school()->set_id(school.getId());
        response->mutable_school()->set_name(school.getName());
        response->mutable_school()->set_name_abbr(school.getNameAbbr());
        response->mutable_school()->set_location(school.getLocation());
        response->mutable_school()->set_created_at(school.getCreatedAt());
        response->set_success(true);
        return Status::OK;
    }

    response->set_success(false);
    return Status(grpc::StatusCode::ALREADY_EXISTS, "School with this name already exists");
}

grpc::Status SchoolServiceImpl::GetSchool(grpc::ServerContext* context,
                                        const GetSchoolRequest* request,
                                        GetSchoolResponse* response) {
    auto school = ::SchoolService::getSchoolById(request->id());
    
    if (school.has_value()) {
        response->mutable_school()->set_id(school->getId());
        response->mutable_school()->set_name(school->getName());
        response->mutable_school()->set_name_abbr(school->getNameAbbr());
        response->mutable_school()->set_location(school->getLocation());
        response->mutable_school()->set_created_at(school->getCreatedAt());

        response->set_success(true);
        return Status::OK;
    }
    response->set_success(false);

    return Status(grpc::StatusCode::NOT_FOUND, "School not found");
}

grpc::Status SchoolServiceImpl::ListSchools(grpc::ServerContext* context,
                                          const ListSchoolsRequest* request,
                                          ListSchoolsResponse* response) {
    auto schools = ::SchoolService::getAllSchools();
    
    for (const auto& school : schools) {
        auto* schoolProto = response->add_schools();
        schoolProto->set_id(school.getId());
        schoolProto->set_name(school.getName());
        schoolProto->set_name_abbr(school.getNameAbbr());
        schoolProto->set_location(school.getLocation());
        schoolProto->set_created_at(school.getCreatedAt());
    }
    response->set_total_count(schools.size());
    response->set_success(true);


    return Status::OK;
}

grpc::Status SchoolServiceImpl::DeleteSchool(grpc::ServerContext* context,
                                           const DeleteSchoolRequest* request,
                                           DeleteSchoolResponse* response) {
    auto [success, school] = ::SchoolService::deleteSchool(request->id());
    
    if (success) {
        response->set_success(true);
        return Status::OK;
    }

    response->set_success(false);
    return Status(grpc::StatusCode::NOT_FOUND, "School not found");
}

std::unique_ptr<grpc::Service> CreateSchoolService() {
    return std::make_unique<SchoolServiceImpl>();
}

} // namespace accounts