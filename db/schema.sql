CREATE TYPE user_type AS ENUM ('student', 'researcher', 'admin');


CREATE TABLE schools(
    id SERIAL PRIMARY KEY,
    name VARCHAR(500) UNIQUE NOT NULL ,
    name_abbr VARCHAR(100) NOT NULL,
    location VARCHAR(255) NOT Null,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    refresh_token VARCHAR(255),
    valid_token BOOLEAN DEFAULT true,
    type user_type NOT NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_users_email ON users(email);


SELECT t.typname as enum_name,
       e.enumlabel as enum_value,
       e.enumsortorder as sort_order
FROM pg_type t
JOIN pg_enum e ON t.oid = e.enumtypid
WHERE t.typname = 'user_type'
ORDER BY e.enumsortorder;


CREATE TABLE students (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) UNIQUE NOT NULL,
    school_id INTEGER REFERENCES schools(id),
    graduation_year VARCHAR(5),
    wilaya VARCHAR(100),
    profile_picture_path VARCHAR(500),
    secondary_profile_picture_path VARCHAR(500),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_student_user_id ON students(user_id);

CREATE TABLE student_profile (
    id SERIAL PRIMARY KEY,
    student_id INTEGER REFERENCES students(id) ON DELETE CASCADE,
    profile_data jsonb NOT NULL
);

CREATE INDEX idx_profile_data_gin
ON student_profile
USING GIN (profile_data);

CREATE INDEX idx_student_profile_student_id ON student_profile(student_id);

CREATE TABLE researchers (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) UNIQUE NOT NULL,
    wilaya VARCHAR(100),
    profile_picture_path VARCHAR(500),
    secondary_profile_picture_path VARCHAR(500),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_researcher_user_id ON researchers(user_id);