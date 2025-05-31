cmake_minimum_required(VERSION 3.14)
project(accounts-service VERSION 0.1.0)

include(./common.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GRPC_AS_SUBMODULE ON)

# Exclude OpenSSL from vcpkg auto-linking
set(VCPKG_TARGET_TRIPLET "x64-linux")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Set gRPC variables before add_subdirectory
set(gRPC_BUILD_TESTS OFF)
set(gRPC_INSTALL OFF)
set(gRPC_SSL_PROVIDER package)

# Point to your local gRPC clone
add_subdirectory(
    "/opt/grpc" 
    "${CMAKE_CURRENT_BINARY_DIR}/grpc-build"
    EXCLUDE_FROM_ALL
)

# Find packages (avoid OpenSSL from vcpkg)
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

# Prevent automatic linking of OpenSSL from vcpkg
set_target_properties(accounts-service PROPERTIES
    LINK_INTERFACE_MULTIPLICITY 2
)

# Link libraries in specific order
target_link_libraries(accounts-service PRIVATE 
    # gRPC libraries (include BoringSSL internally)
    gRPC::gpr
    gRPC::grpc
    gRPC::grpc++
    gRPC::grpc++_alts
    gRPC::grpc++_reflection

    protobuf::libprotoc
    protobuf::libprotobuf
    protobuf::libprotobuf-lite

    absl::flags_parse
    absl::absl_log
    absl::log_initialize
    absl::log_globals

    nlohmann_json::nlohmann_json
    libpqxx::pqxx
    Crow::Crow
    jwt-cpp::jwt-cpp
    miniocpp::miniocpp
    -pthread
    -ldl
)

# Force linker to prefer gRPC's symbols
target_link_options(accounts-service PRIVATE
    -Wl,--allow-multiple-definition
    -Wl,--as-needed
)

# Install target
install(TARGETS accounts-service
    RUNTIME DESTINATION bin
)