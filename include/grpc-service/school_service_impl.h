#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>
#include "grpc/accounts_service.grpc.pb.h"
#include "service/school_service.h"

namespace accounts {

class SchoolServiceImpl final : public SchoolService::Service {
public:
    grpc::Status CreateSchool(grpc::ServerContext* context,
                            const CreateSchoolRequest* request,
                            CreateSchoolResponse* response) override;

    grpc::Status GetSchool(grpc::ServerContext* context,
                          const GetSchoolRequest* request,
                          GetSchoolResponse* response) override;

    grpc::Status ListSchools(grpc::ServerContext* context,
                           const ListSchoolsRequest* request,
                           ListSchoolsResponse* response) override;

    grpc::Status DeleteSchool(grpc::ServerContext* context,
                            const DeleteSchoolRequest* request,
                            DeleteSchoolResponse* response) override;
};

// Helper function to create a new instance of the service
std::unique_ptr<grpc::Service> CreateSchoolService();

} // namespace accounts