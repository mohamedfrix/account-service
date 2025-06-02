#pragma once
#include <memory>
#include "grpc/accounts_service.grpc.pb.h"
#include <grpcpp/grpcpp.h>

namespace accounts {
class ResearcherServiceImpl final : public accounts::ResearcherService::Service {
public:
    grpc::Status CreateResearcher(grpc::ServerContext* context,
                                  const CreateResearcherRequest* request,
                                  CreateResearcherResponse* response) override;

    grpc::Status GetResearcher(grpc::ServerContext* context,
                               const GetResearcherRequest* request,
                               GetResearcherResponse* response) override;

    grpc::Status GetResearcherByUserId(grpc::ServerContext* context,
                                       const GetResearcherByUserIdRequest* request,
                                       GetResearcherByUserIdResponse* response) override;

    grpc::Status UpdateResearcher(grpc::ServerContext* context,
                                  const UpdateResearcherRequest* request,
                                  UpdateResearcherResponse* response) override;

};
}