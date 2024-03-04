#ifndef KGIT_KGIT_H
#define KGIT_KGIT_H

#include <cstdio>
#include <git2.h>
#include <mutex>

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

    static void cloneRepository(git_repository *repository, const char *url, const char *path);
    static void listBranches(git_repository *repository, const char *path, git_branch_t branch_type);
    static void revwalking(git_repository *repository, const char *path);
};

#endif //KGIT_KGIT_H
