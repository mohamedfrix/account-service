#pragma once
#include <grpcpp/grpcpp.h>
#include "grpc/accounts_service.grpc.pb.h"
#include  <nlohmann/json.hpp>

namespace accounts {

class StudentProfileServiceImpl final : public accounts::StudentProfileService::Service {
public:
    grpc::Status CreateStudentProfile(grpc::ServerContext* context,
                                      const accounts::CreateStudentProfileRequest* request,
                                      accounts::CreateStudentProfileResponse* response) override;

    grpc::Status GetStudentProfile(grpc::ServerContext* context,
                                   const accounts::GetStudentProfileRequest* request,
                                   accounts::GetStudentProfileResponse* response) override;

    grpc::Status UpdateStudentProfile(grpc::ServerContext* context,
                                      const accounts::UpdateStudentProfileRequest* request,
                                      accounts::UpdateStudentProfileResponse* response) override;


};
} // namespace accounts 