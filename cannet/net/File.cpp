#include "File.h"

namespace cannet {

    Status File::getContent(const std::string &filename, std::string &cont) {
        int fd = open(filename.c_str(), O_RDONLY);
        if (fd < 0) {
            return Status::ioError("open", filename);
        }

        ExitCaller ec1([=] { close(fd); });
        char buf[4096];
        for (;;) {
            int r = read(fd, buf, sizeof(buf));
            if (r < 0) {
                return Status::ioError("read", filename);
            } else if (r == 0) {
                break;
            }
            cont.append(buf, r);
        }
        return {};
    }

    Status File::writeContent(const std::string &filename, const std::string &cont) {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd < 0) {
            return Status::ioError("open", filename);
        }
        ExitCaller ec1([=] { close(fd); });
        int r = write(fd, cont.data(), cont.size());
        if (r < 0) {
            return Status::ioError("write", filename);
        }
        return {};
    }

    Status File::renameSave(const std::string &name, const std::string &tmpName, const std::string &cont) {
        Status s = writeContent(tmpName, cont);
        if (s.ok()) {
            unlink(name.c_str());
            s = renameFile(tmpName, name);
        }
        return s;
    }

    Status File::getChildren(const std::string &dir, std::vector<std::string> *result) {
        result->clear();
        DIR *d = opendir(dir.c_str());
        if (d == nullptr) {
            return Status::ioError("opendir", dir);
        }
        struct dirent *entry;
        while ((entry = readdir(d)) != nullptr) {
            result->push_back(entry->d_name);
        }
        closedir(d);
        return {};
    }

    Status File::deleteFile(const std::string &filename) {
        if (unlink(filename.c_str()) != 0) {
            return Status::ioError("unlink", filename);
        }
        return {};
    }

    Status File::createDir(const std::string &name) {
        if (mkdir(name.c_str(), 0755) != 0) {
            return Status::ioError("mkdir", name);
        }
        return {};
    }

    Status File::deleteDir(const std::string &name) {
        if (rmdir(name.c_str()) != 0) {
            return Status::ioError("rmdir", name);
        }
        return {};
    }

    Status File::getFileSize(const std::string &filename, uint64_t *size) {
        struct stat sbuf;
        if (stat(filename.c_str(), &sbuf) != 0) {
            *size = 0;
            return Status::ioError("stat", filename);
        } else {
            *size = sbuf.st_size;
        }
        return {};
    }

    Status File::renameFile(const std::string &src, const std::string &target) {
        if (rename(src.c_str(), target.c_str()) != 0) {
            return Status::ioError("raname", src + " " + target);
        }
        return {};
    }

    bool File::fileExists(const std::string &filename) {
        return access(filename.c_str(), F_OK) == 0;
    }
};