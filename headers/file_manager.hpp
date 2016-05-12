#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include <stdexcept>
#include <string>
#include <vector>
#include <string.h>
#include <sys/stat.h>

#if ! (defined(__unix__) || defined(__APPLE__))
#error "This won't work on a non-unix OS!"
#endif

class OSError: public std::runtime_error {
    int err;
public:
    OSError(const char* str, int err): std::runtime_error(std::string(str)+ ": " + strerror(err)), err(err) {}
    int get_err() {return err;}
};

class FDWrapper {
    int fd;
public:
    FDWrapper(const std::string& file, int flags, mode_t mode = 0);
    explicit FDWrapper(int fd): fd(fd) {}
    ~FDWrapper();
    operator int() const {return fd;}
};

std::vector<unsigned char> read_file(const std::string& file_root, const std::string& path);
void write_file(const std::string& file_root, const std::string& path, const std::vector<unsigned char>& data);
void copy_file_to_storage(const std::string& file_root, const std::string& path, const std::string& orig_file);
void copy_file_from_storage(const std::string& file_root, const std::string& path, const std::string& dest_file);
void delete_file(const std::string& file_root, const std::string& path);
bool exists(const std::string& file_root, const std::string& path);
void link(const std::string& file_root, const std::string& orig_path, const std::string& dest_path);
#endif
