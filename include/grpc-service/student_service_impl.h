#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>
#include "grpc/accounts_service.grpc.pb.h"
#include "service/student_service.h"

namespace accounts{

class StudentServiceImpl final : public StudentService::Service {
public:
    grpc::Status CreateStudent(grpc::ServerContext* context,
                               const CreateStudentRequest* request,
                               CreateStudentResponse* response) override;

    grpc::Status GetStudent(grpc::ServerContext* context,
                            const GetStudentRequest* request,
                            GetStudentResponse* response) override;

    grpc::Status GetStudentByUserId(grpc::ServerContext* context,
                                    const GetStudentByUserIdRequest* request,
                                    GetStudentByUserIdResponse* response) override;

    grpc::Status UpdateStudent(grpc::ServerContext* context,
                               const UpdateStudentRequest* request,
                               UpdateStudentResponse* response) override;

};
} // namespace accounts