#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <crow.h>

#include "config/env.h"
#include "db/database.h"

#include "grpc-service/school_service_impl.h"
#include "grpc-service/student_service_impl.h"
#include "grpc-service/student_profile_service_impl.h"
#include "grpc-service/researcher_service_impl.h"
#include <grpcpp/grpcpp.h>


void RunGrpcServer() {
    std::string server_address("0.0.0.0:50050");
    accounts::SchoolServiceImpl school_service;
    accounts::StudentServiceImpl student_service;
    accounts::StudentProfileServiceImpl student_profile_service;
    accounts::ResearcherServiceImpl researcher_service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&school_service);
    builder.RegisterService(&student_service);
    builder.RegisterService(&student_profile_service);
    builder.RegisterService(&researcher_service);

    std::cout << "Server starting on " << server_address << std::endl;
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server started successfully" << std::endl;
    server->Wait();
}

void config() {
    try {
        std::cout << "Starting Competitive Programming Platform..." << std::endl;
        
        // Load environment variables
        auto& env = Env::getInstance();
        if (!env.load(".env")) {
            std::cerr << "Failed to load .env file. Using default values." << std::endl;
            
            // Create .env file from .env.example if not exists
            std::ifstream env_file(".env");
            if (!env_file) {
                std::cout << "Creating .env file from .env.example..." << std::endl;
                std::ifstream example(".env.example");
                std::ofstream output(".env");
                if (example && output) {
                    output << example.rdbuf();
                    std::cout << "Created .env file. Please update with your database credentials." << std::endl;
                } else {
                    std::cerr << "Failed to create .env file." << std::endl;
                }
            }
            env_file.close();
        }

        std::cout << "Connection string: " << env.getDbConnectionString() << std::endl;

        Database::initialize(env.getDbConnectionString());
    } catch (const std::exception& e) {
        std::cerr << "Error during configuration: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Configuration completed successfully." << std::endl;
}

void startCrowServer() {
    
    crow::App<> app;
        
    // Handle OPTIONS requests
    CROW_ROUTE(app, "/<path>")
    .methods("OPTIONS"_method)
    ([](const crow::request& req, const std::string& path) {
        return crow::response(204);
    });
    
    // Add health check endpoint
    CROW_ROUTE(app, "/health")
    ([]() {
        crow::json::wvalue response;
        response["status"] = "ok";
        return crow::response(200, response);
    });


    std::cout << "Crow server starting on port 8080..." << std::endl;
    
    // Start server
    app.port(8080)
        .multithreaded()
        .run();
}

void runServers() {

    std::thread grpc_thread(RunGrpcServer);
    std::thread crow_thread(startCrowServer);

    // Wait for both threads to finish
    grpc_thread.join();
    crow_thread.join();
}

int main() {
    
    config();
    runServers();
    return 0;
}

