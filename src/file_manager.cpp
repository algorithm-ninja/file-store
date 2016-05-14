#include "file_manager.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <functional>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

typedef std::function<void(const unsigned char*, size_t)> read_callback_t;
typedef std::function<void(const read_callback_t&)> reader_t;
static const unsigned buf_size = 65536;
static const mode_t file_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
static const mode_t dir_mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

FDWrapper::FDWrapper(const std::string& file, int flags, mode_t mode) {
    if (flags & O_CREAT) fd = open(file.c_str(), flags, mode);
    else fd = open(file.c_str(), flags);
    if (fd == -1) throw OSError("open", errno);
}

FDWrapper::~FDWrapper() {
    if (fd != -1) {
        close(fd); //TODO: close may still fail.
    }
}

void mkdirs(std::string dir, int start_offset = 1) {
    for (unsigned i=start_offset; i<dir.size(); i++) {
        if (dir[i] == '/') {
            dir[i] = 0;
            int ret = mkdir(dir.c_str(), dir_mode);
            dir[i] = '/';
            if (ret == -1 && errno != EEXIST)
                throw OSError("mkdir", errno);
        }
    }
}

static void file_reader(const std::string& file_path, const read_callback_t& callback) {
    unsigned char buf[buf_size];
    FDWrapper fd(file_path, O_RDONLY);
    int size_read = 0;
    do {
        size_read = read(fd, buf, buf_size);
        if (size_read == -1 && errno == EINTR) continue;
        if (size_read == -1) throw OSError("read", errno);
        callback(buf, size_read);
    } while (size_read > 0);
}

static void data_reader(const std::vector<unsigned char>& data, const read_callback_t& callback) {
    for (size_t i=0; i<data.size(); i += buf_size) {
        callback(&data[i], std::min(data.size(), i+buf_size)-i);
    }
}

static void file_writer(const FDWrapper& fd, const reader_t& reader) {
    reader([&](const unsigned char* buf, size_t len) {
        while (len > 0) {
            int sz = write(fd, buf, len);
            if (sz == -1 && errno == EINTR) continue;
            if (sz == -1) throw OSError("write", errno);
            buf += sz;
            len -= sz;
        }
    });
}

static void file_writer(const std::string& file_path, const reader_t& reader) {
    file_writer(FDWrapper(file_path, O_RDWR | O_CREAT | O_TRUNC, file_mode), reader);
}

static std::string hasher(const reader_t& reader) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    reader([&](const unsigned char* buf, size_t len) {
        SHA256_Update(&sha256, buf, len);
    });
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i=0; i<SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::vector<unsigned char> read_file(const std::string& file_root, const std::string& path) {
    std::vector<unsigned char> vec;
    file_reader(file_root + "/files/" + path, [&](const unsigned char* buf, size_t len) {
        vec.insert(vec.end(), buf, buf+len);
    });
    return std::move(vec);
}

void write_file(const std::string& file_root, const std::string& path, const std::vector<unsigned char>& data) {
    using namespace std::placeholders;
    std::string hash = hasher(std::bind(data_reader, data, _1));
    mkdirs(file_root + "/temp/");
    std::string tmp = file_root + "/temp/tmpXXXXXX";
    std::vector<char> temp_file(tmp.begin(), tmp.end());
    temp_file.push_back(0);
    FDWrapper temp_file_fd(mkstemp(&temp_file[0]));
    file_writer(temp_file_fd, std::bind(data_reader, data, _1));
    std::string data_file = file_root + "/data/" + hash[0] + hash[1] + "/" + hash[2] + hash[3] + "/" + hash;
    mkdirs(data_file);
    std::string dest_file = file_root + "/files/" + path;
    mkdirs(dest_file);
    if (link(&temp_file[0], data_file.c_str()) == -1 && errno != EEXIST) {
        int err = errno;
        if (unlink(&temp_file[0]) == -1) throw OSError("unlink", errno);
        throw OSError("link", err);
    }
    if (unlink(&temp_file[0]) == -1) throw OSError("unlink", errno);
    if (link(data_file.c_str(), dest_file.c_str()) == -1) {
        int err = errno;
        if (unlink(data_file.c_str()) == -1) throw OSError("unlink", errno);
        throw OSError("link", err);
    }
}

void copy_file_to_storage(const std::string& file_root, const std::string& path, const std::string& orig_file) {
    using namespace std::placeholders;
    std::string hash = hasher(std::bind(file_reader, orig_file, _1));
    mkdirs(file_root + "/temp/");
    std::string tmp = file_root + "/temp/tmpXXXXXX";
    std::vector<char> temp_file(tmp.begin(), tmp.end());
    temp_file.push_back(0);
    FDWrapper temp_file_fd(mkstemp(&temp_file[0]));
    file_writer(temp_file_fd, std::bind(file_reader, orig_file, _1));
    std::string data_file = file_root + "/data/" + hash[0] + hash[1] + "/" + hash[2] + hash[3] + "/" + hash;
    mkdirs(data_file);
    std::string dest_file = file_root + "/files/" + path;
    mkdirs(dest_file);
    if (link(&temp_file[0], data_file.c_str()) == -1 && errno != EEXIST) {
        int err = errno;
        if (unlink(&temp_file[0]) == -1) throw OSError("unlink", errno);
        throw OSError("link", err);
    }
    if (unlink(&temp_file[0]) == -1) throw OSError("unlink", errno);
    if (link(data_file.c_str(), dest_file.c_str()) == -1) {
        int err = errno;
        if (unlink(data_file.c_str()) == -1) throw OSError("unlink", errno);
        throw OSError("link", err);
    }
}

void copy_file_from_storage(const std::string& file_root, const std::string& path, const std::string& dest_file) {
    file_writer(dest_file, std::bind(file_reader, file_root + "/files/" + path, std::placeholders::_1));
}

void delete_file(const std::string& file_root, const std::string& path) {
    if (unlink((file_root + "/files/" + path).c_str()) == -1)
        throw OSError("unlink", errno);
}

bool exists(const std::string& file_root, const std::string& path) {
    struct stat buf;
    if (stat((file_root + "/files/" + path).c_str(), &buf) == -1) {
        if (errno == ENOENT) return false;
        throw OSError("stat", errno);
    }
    return true;
}

void link(const std::string& file_root, const std::string& orig_path, const std::string& dest_path) {
    if (link((file_root + "/files/" + orig_path).c_str(), (file_root + "/files/" + dest_path).c_str()) == -1)
        throw OSError("link", errno);
}
