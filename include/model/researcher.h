#pragma once

#include <string>
#include <nlohmann/json.hpp>


class Researcher {
public:
    Researcher() = default;
    Researcher(int id, int user_id, const std::string& wilaya,
               const std::string& profile_picture_path,
               const std::string& secondary_profile_picture_path,
               const std::string& created_at)
        : id_(id), user_id_(user_id), wilaya_(wilaya),
          profile_picture_path_(profile_picture_path),
          secondary_profile_picture_path_(secondary_profile_picture_path),
          created_at_(created_at) {}

    int getId() const { return id_; }
    int getUserId() const { return user_id_; }
    std::string getWilaya() const { return wilaya_; }
    std::string getProfilePicturePath() const { return profile_picture_path_; }
    std::string getSecondaryProfilePicturePath() const { return secondary_profile_picture_path_; }
    std::string getCreatedAt() const { return created_at_; }

    void setWilaya(const std::string& wilaya) { wilaya_ = wilaya; }
    void setProfilePicturePath(const std::string& profile_picture_path) { profile_picture_path_ = profile_picture_path; }
    void setSecondaryProfilePicturePath(const std::string& secondary_profile_picture_path) { secondary_profile_picture_path_ = secondary_profile_picture_path; }
    nlohmann::json toJson() const {
        nlohmann::json json;
        json["id"] = id_;
        json["user_id"] = user_id_;
        json["wilaya"] = wilaya_;
        json["profile_picture_path"] = profile_picture_path_;
        json["secondary_profile_picture_path"] = secondary_profile_picture_path_;
        json["created_at"] = created_at_;
        return json;
    }

private:
    int id_;
    int user_id_;
    std::string wilaya_;
    std::string profile_picture_path_;
    std::string secondary_profile_picture_path_;
    std::string created_at_;
};