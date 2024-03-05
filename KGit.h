#ifndef KGIT_KGIT_H
#define KGIT_KGIT_H

#include <cstdio>
#include <cstring>
#include <git2.h>
#include <mutex>
#include <string>
#include <vector>

class KGit {
private:
    static KGit *instance;
    static std::mutex mutex_;
    KGit(){};

protected:
    ~KGit() = default;

public:
    KGit(KGit &other) = delete;
    void operator=(const KGit &) = delete;
    static KGit *GetInstance();

    static void checkError(char buffer[], int error);
    static void cloneRepository(git_repository *repository, const char *url, const char *path);
    static std::vector<std::string> listBranches(git_repository *repository, const char *path, git_branch_t branch_type);
    static std::vector<std::string> revisionWalking(git_repository *repository, const char *path, const char *branch_name);
};

#endif //KGIT_KGIT_H
