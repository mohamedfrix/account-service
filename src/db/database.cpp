#include "db/database.h"
#include <iostream>
#include <stdexcept>
#include <pqxx/pqxx>
#include "config/env.h"
#include "repository/school_repo.h"
#include "repository/student_repo.h"
#include "repository/student_profile_repo.h"
#include "repository/researcher_repo.h"


// Define the static member variable
std::shared_ptr<pqxx::connection> Database::conn_;

void Database::initialize(const std::string& connection_string) {
    try {
        conn_ = std::make_shared<pqxx::connection>(connection_string);
        
        if (!conn_->is_open()) {
            throw std::runtime_error("Failed to open database connection");
        }
        
        std::cout << "Connected to database: " << conn_->dbname() << std::endl;

        // Initialize repositories
        SchoolRepo::initialize(conn_);
        StudentRepo::initialize(conn_);
        StudentProfileRepo::initialize(conn_);
        ResearcherRepo::initialize(conn_);

    } catch (const std::exception& e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        throw;
    }
}

std::shared_ptr<pqxx::connection> Database::getConnection() {
    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Database connection is not initialized or closed");
    }
    return conn_;
}


