#include "util/minio.h"
#include <miniocpp/args.h>
#include <miniocpp/providers.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib> // for system()


// Initialize static members
minio::s3::Client* MinioService::client = nullptr;
std::string MinioService::endpoint;
std::string MinioService::port;
std::string MinioService::access_key;
std::string MinioService::secret_key;

void MinioService::initialize(const std::string& ep, 
                            const std::string& p,
                            const std::string& ak,
                            const std::string& sk) {
    endpoint = ep;
    port = p;
    access_key = ak;
    secret_key = sk;

    auto full_endpoint = "http://" + endpoint + ":" + port;
    
    // Create S3 base URL
    minio::s3::BaseUrl base_url(full_endpoint);
    
    // Create credential provider
    minio::creds::StaticProvider* provider = new minio::creds::StaticProvider(access_key, secret_key);
    
    // Create S3 client
    client = new minio::s3::Client(base_url, provider);
}

void MinioService::cleanup() {
    if (client) {
        delete client;
        client = nullptr;
    }
}

std::string MinioService::getFileExtensionFromContentType(const std::string& contentType) {
    size_t pos = contentType.find("/");
    if (pos != std::string::npos) {
        return contentType.substr(pos + 1);
    }
    return "bin"; // Default extension if content type is not recognized
}

std::string MinioService::createTempFile(const std::vector<uint8_t>& fileData, 
                                       const std::string& extension) {
    // Generate unique filename using timestamp
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    std::string tempFileName = "/tmp/minio_upload_" + 
                             std::to_string(now_ms) + "_" + 
                             std::to_string(rand()) + "." + extension;

    // Write data to temporary file
    std::ofstream tempFile(tempFileName, std::ios::binary);
    if (!tempFile) {
        std::cerr << "Failed to create temporary file: " << tempFileName << std::endl;
        return "";
    }

    tempFile.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
    tempFile.close();

    return tempFileName;
}

std::string MinioService::uploadFile(const std::string& bucket_name,
                                   const std::vector<uint8_t>& fileData,
                                   const std::string& contentType) {
    try {
        // Get file extension from content type
        std::string extension = getFileExtensionFromContentType(contentType);
        
        // Create temporary file
        std::string tempFilePath = createTempFile(fileData, extension);
        if (tempFilePath.empty()) {
            return "";
        }

        // Generate final filename for MinIO
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
        std::string filename = std::to_string(now_ms) + "_" + 
                             std::to_string(rand()) + "." + extension;

        // Open file stream for reading
        // std::ifstream fileStream(tempFilePath, std::ios::binary);
        // std::ifstream fileStream(tempFilePath);

        // if (!fileStream) {
        //     std::cerr << "Failed to open temporary file for reading" << std::endl;
        //     std::remove(tempFilePath.c_str());
        //     return "";
        // }

        // std::cout << "Uploading file to MinIO: " << filename << " Of size " << fileData.size() << std::endl;

        // // Create put object arguments
        // minio::s3::PutObjectArgs args(fileStream, fileData.size(), 5 * 1024 * 1024); // 5MB part size
        // args.bucket = bucket_name;
        // args.object = filename;
        // // args.content_type = contentType;

        // // Upload file
        // auto resp = client->PutObject(args);
        
        // // Close and remove temporary file
        // fileStream.close();
        // // std::remove(tempFilePath.c_str());

        // if (!resp) {
        //     std::cerr << "Failed to upload file: " << resp.Error().String() << std::endl;
        //     return "";
        // }

        int result = system("mc alias set minio http://localhost:9000 frix 07vk640xz");
        if (result != 0) {
            std::cerr << "Failed to set MinIO alias" << std::endl;
            std::remove(tempFilePath.c_str());
            return "";
        }
        result = system(("mc cp " + tempFilePath + " minio/" + bucket_name + "/" + filename).c_str());
        if (result != 0) {
            std::cerr << "Failed to upload file to MinIO: " << filename << std::endl;
            std::remove(tempFilePath.c_str());
            return "";
        }
        std::remove(tempFilePath.c_str());

        return filename;
    } catch (const std::exception& e) {
        std::cerr << "Error in uploadFile: " << e.what() << std::endl;
        return "";
    }
}

std::string MinioService::getFileUrl(const std::string& fileName, std::string bucket_name) {
    try {
        // minio::s3::GetPresignedObjectUrlArgs args;
        // args.bucket = bucket_name;
        // args.object = fileName;
        // args.expiry_seconds = 24 * 60 * 60; // 24 hours

        // auto resp = client->GetPresignedObjectUrl(args);
        // if (!resp) {
        //     std::cerr << "Failed to get file URL: " << resp.Error().String() << std::endl;
        //     return "";
        // }

        // return resp.url;
        int result = system("mc alias set minio http://localhost:9000 frix 07vk640xz");
        if (result != 0) {
            std::cerr << "Failed to set MinIO alias" << std::endl;
            return "";
        }
        result = system(("mc share download minio/" + bucket_name + "/" + fileName).c_str());
    } catch (const std::exception& e) {
        std::cerr << "Error in getFileUrl: " << e.what() << std::endl;
        return "";
    }
}
