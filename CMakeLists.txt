cmake_minimum_required(VERSION 3.14)
project(accounts-service VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages using vcpkg
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(miniocpp CONFIG REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/grpc-generated
)

# Source files
set(SOURCES
    src/main.cpp
    src/config/env.cpp
    src/db/database.cpp
    src/repository/student_repo.cpp
    src/repository/student_profile_repo.cpp
    src/repository/school_repo.cpp
    src/repository/researcher_repo.cpp
    src/service/school_service.cpp
    src/util/minio.cpp
    src/grpc-service/school_service_impl.cpp
    grpc-generated/grpc/accounts_service.grpc.pb.cc
    grpc-generated/message/accounts_service.pb.cc
)

# Add executable
add_executable(accounts-service ${SOURCES})

# Link libraries in the correct order - protobuf libraries first
target_link_libraries(accounts-service PRIVATE 
    # Protobuf libraries MUST come first
    protobuf::libprotobuf
    
    # gRPC libraries
    gRPC::grpc++
    gRPC::grpc++_reflection
    gRPC::gpr
    
    # Other libraries
    nlohmann_json::nlohmann_json
    libpqxx::pqxx
    Crow::Crow
    jwt-cpp::jwt-cpp
    miniocpp::miniocpp
    
    # System libraries
    pthread
    dl
)

# Install target
install(TARGETS accounts-service
    RUNTIME DESTINATION bin
)