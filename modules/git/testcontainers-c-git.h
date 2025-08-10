#ifndef TESTCONTAINERS_GIT_H
#define TESTCONTAINERS_GIT_H

/// @brief Creates a container request with a default image, exposed port and init logic
/// @return Container request ID
int tc_git_container_create_default();

/// @brief Creates a container request with a default image, exposed port and init logic
/// @param image Full image name
/// @return Container request ID
int tc_git_container_create(const char* image);

void tc_git_container_with_repository(int request_id, const char* repository);

void tc_git_container_with_password(int request_id, const char* password);

void tc_git_container_with_ssh_key_auth(int request_id);

void tc_git_container_with_existing_local_repository(int request_id, const char* local_repository);

#endif
