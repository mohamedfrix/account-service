# Accounts Service API Documentation

This document describes the gRPC services available in the Accounts Service.

## Table of Contents
- [File Handling](#file-handling)
- [Common Messages](#common-messages)
- [Student Service](#student-service)
- [School Service](#school-service)
- [Researcher Service](#researcher-service)
- [Student Profile Service](#student-profile-service)
- [Student Service](#student-service)
- [School Service](#school-service)
- [Researcher Service](#researcher-service)
- [Student Profile Service](#student-profile-service)

## File Handling

The service handles file uploads (such as profile pictures) using the `FileData` message type:

```protobuf
message FileData {
    bytes content = 1;              // The actual file content
    string content_type = 2;        // MIME type of the file
}
```

Files are stored in MinIO storage, and the service returns paths to the stored files in responses.

## Common Messages

### School
```protobuf
message School {
    int32 id = 1;                    // Unique identifier for the school
    string name = 2;                 // Full name of the school
    string name_abbr = 3;           // Abbreviated name of the school
    string location = 4;            // Physical location of the school
    string created_at = 5;          // Timestamp when the record was created
}
```

### Student
```protobuf
message Student {
    int32 id = 1;
    int32 user_id = 2;
    School school = 3;
    string graduation_year = 4;
    string wilaya = 5;
    string profile_picture_path = 6;           // Path to stored image in MinIO
    string secondary_profile_picture_path = 7;  // Path to stored image in MinIO
    string created_at = 8;
}
```

## Student Service

Service for managing student accounts and information. Handles profile picture uploads and storage.

### Methods

#### CreateStudent
Creates a new student account.

**Request** (`CreateStudentRequest`):
```protobuf
message CreateStudentRequest {
    int32 user_id = 1;
    int32 school_id = 2;
    string graduation_year = 3;
    string wilaya = 4;
    FileData profile_picture = 5;              // Optional profile picture upload
    FileData secondary_profile_picture = 6;     // Optional secondary profile picture upload
}
```

**Response** (`CreateStudentResponse`):
```protobuf
message CreateStudentResponse {
    bool success = 1;
    Student student = 2;
    string error_message = 3;
}
```

#### GetStudent
Retrieves student information by ID.

**Request** (`GetStudentRequest`):
```protobuf
message GetStudentRequest {
    int32 id = 1;
}
```

**Response** (`GetStudentResponse`):
```protobuf
message GetStudentResponse {
    Student student = 1;
    string error_message = 2;
}
```

#### GetStudentByUserId
Retrieves student information by user ID.

**Request** (`GetStudentByUserIdRequest`):
```protobuf
message GetStudentByUserIdRequest {
    int32 user_id = 1;
}
```

**Response** (`GetStudentByUserIdResponse`):
```protobuf
message GetStudentByUserIdResponse {
    Student student = 1;
    string error_message = 2;
}
```

#### UpdateStudent
Updates student information. All fields except id are optional, allowing partial updates.

**Request** (`UpdateStudentRequest`):
```protobuf
message UpdateStudentRequest {
    int32 id = 1;
    optional int32 school_id = 2;
    optional string graduation_year = 3;
    optional string wilaya = 4;
    optional FileData profile_picture = 5;      // Optional new profile picture
    optional FileData secondary_profile_picture = 6;  // Optional new secondary profile picture
}
```

**Response** (`UpdateStudentResponse`):
```protobuf
message UpdateStudentResponse {
    bool success = 1;
    Student student = 2;                       // Includes updated image paths
    string error_message = 3;
}
```

#### DeleteStudent
Deletes a student account.

**Request** (`DeleteStudentRequest`):
```protobuf
message DeleteStudentRequest {
    int32 id = 1;
}
```

**Response** (`DeleteStudentResponse`):
```protobuf
message DeleteStudentResponse {
    bool success = 1;
    string error_message = 2;
}
```

## School Service

Service for managing school information.

### Methods

#### CreateSchool
Creates a new school entry.

**Request** (`CreateSchoolRequest`):
```protobuf
message CreateSchoolRequest {
    string name = 1;
    string name_abbr = 2;
    string location = 3;
}
```

**Response** (`CreateSchoolResponse`):
```protobuf
message CreateSchoolResponse {
    bool success = 1;
    School school = 2;
    string error_message = 3;
}
```

#### GetSchool
Retrieves school information by ID.

**Request** (`GetSchoolRequest`):
```protobuf
message GetSchoolRequest {
    int32 id = 1;
}
```

**Response** (`GetSchoolResponse`):
```protobuf
message GetSchoolResponse {
    School school = 1;
    string error_message = 2;
}
```

#### GetSchoolByName
Retrieves school information by name.

**Request** (`GetSchoolByNameRequest`):
```protobuf
message GetSchoolByNameRequest {
    string name = 1;
}
```

**Response** (`GetSchoolByNameResponse`):
```protobuf
message GetSchoolByNameResponse {
    School school = 1;
    string error_message = 2;
}
```

#### UpdateSchool
Updates school information.

**Request** (`UpdateSchoolRequest`):
```protobuf
message UpdateSchoolRequest {
    School school = 1;
}
```

**Response** (`UpdateSchoolResponse`):
```protobuf
message UpdateSchoolResponse {
    bool success = 1;
    string error_message = 2;
}
```

## Researcher Service

Service for managing researcher accounts and information.

### Methods

#### CreateResearcher
Creates a new researcher account.

**Request** (`CreateResearcherRequest`):
```protobuf
message CreateResearcherRequest {
    int32 user_id = 1;
    string wilaya = 2;
    string profile_picture_path = 3;
    string secondary_profile_picture_path = 4;
}
```

**Response** (`CreateResearcherResponse`):
```protobuf
message CreateResearcherResponse {
    bool success = 1;
    Researcher researcher = 2;
    string error_message = 3;
}
```

#### GetResearcher
Retrieves researcher information by ID.

**Request** (`GetResearcherRequest`):
```protobuf
message GetResearcherRequest {
    int32 id = 1;
}
```

**Response** (`GetResearcherResponse`):
```protobuf
message GetResearcherResponse {
    Researcher researcher = 1;
    string error_message = 2;
}
```

#### GetResearcherByUserId
Retrieves researcher information by user ID.

**Request** (`GetResearcherByUserIdRequest`):
```protobuf
message GetResearcherByUserIdRequest {
    int32 user_id = 1;
}
```

**Response** (`GetResearcherByUserIdResponse`):
```protobuf
message GetResearcherByUserIdResponse {
    Researcher researcher = 1;
    string error_message = 2;
}
```

## Student Profile Service

Service for managing student profiles with additional information.

### Methods

#### CreateStudentProfile
Creates a new student profile.

**Request** (`CreateStudentProfileRequest`):
```protobuf
message CreateStudentProfileRequest {
    int32 student_id = 1;
    google.protobuf.Struct profile_data = 2;
}
```

**Response** (`CreateStudentProfileResponse`):
```protobuf
message CreateStudentProfileResponse {
    bool success = 1;
    StudentProfile profile = 2;
    string error_message = 3;
}
```

#### GetStudentProfile
Retrieves student profile by ID.

**Request** (`GetStudentProfileRequest`):
```protobuf
message GetStudentProfileRequest {
    int32 id = 1;
}
```

**Response** (`GetStudentProfileResponse`):
```protobuf
message GetStudentProfileResponse {
    StudentProfile profile = 1;
    string error_message = 2;
}
```

#### UpdateStudentProfile
Updates student profile information.

**Request** (`UpdateStudentProfileRequest`):
```protobuf
message UpdateStudentProfileRequest {
    StudentProfile profile = 1;
}
```

**Response** (`UpdateStudentProfileResponse`):
```protobuf
message UpdateStudentProfileResponse {
    bool success = 1;
    string error_message = 2;
}
```

## Common Messages

### Student
```protobuf
message Student {
    int32 id = 1;
    int32 user_id = 2;
    School school = 3;
    string graduation_year = 4;
    string wilaya = 5;
    string profile_picture_path = 6;
    string secondary_profile_picture_path = 7;
    string created_at = 8;
}
```

### School
```protobuf
message School {
    int32 id = 1;
    string name = 2;
    string name_abbr = 3;
    string location = 4;
    string created_at = 5;
}
```

### Researcher
```protobuf
message Researcher {
    int32 id = 1;
    int32 user_id = 2;
    string wilaya = 3;
    string profile_picture_path = 4;
    string secondary_profile_picture_path = 5;
    string created_at = 6;
}
```

### StudentProfile
```protobuf
message StudentProfile {
    int32 id = 1;
    int32 student_id = 2;
    google.protobuf.Struct profile_data = 3;
}
```