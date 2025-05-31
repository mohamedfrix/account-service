#pragma once
#include <miniocpp/client.h>
#include <string>
#include <vector>

class MinioService {
public:
    static void initialize(const std::string& endpoint, 
                         const std::string& port,
                         const std::string& access_key,
                         const std::string& secret_key);
    static void cleanup();
    
    static std::string uploadFile(const std::string& bucket_name, 
                                const std::vector<uint8_t>& fileData, 
                                const std::string& contentType);
    static std::string getFileUrl(const std::string& fileName, std::string bucket_name);

private:
    static minio::s3::Client* client;
    static std::string endpoint;
    static std::string port;
    static std::string access_key;
    static std::string secret_key;

    static std::string getFileExtensionFromContentType(const std::string& contentType);
    static std::string createTempFile(const std::vector<uint8_t>& fileData, 
                                    const std::string& extension);
};
