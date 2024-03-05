#include <cstdio>
#include <string>

#include "KGit.h"

/*
 * cd .. && rmdir /q /s build temp && mkdir build temp && cd build && cmake -S .. -B . && cmake --build . && copy "C:\Program
 Files (x86)\libgit2\bin\git2.dll" . && Debug\KGit.exe
 */

int main(int argc, char *argv[]) {
    printf("---------------------\n");
    git_libgit2_init();
    git_repository *repository = nullptr;
    std::string path = "C:\\KGit\\.git";
    std::string branch = "master";

    KGit *kendoGit = KGit::GetInstance();
    std::vector<std::string> listOfBranchNames = KGit::listBranches(
            repository, (argc < 2 ? path.c_str() : argv[1]), GIT_BRANCH_ALL);
    for (auto &it : listOfBranchNames) {
        printf("-----> %s\n", it.c_str());
    }
    std::vector<std::string> listOfCommits = KGit::revisionWalking(
        repository,(argc < 2 ? path.c_str() : argv[1]),(argc < 3 ? branch.c_str() : argv[2])
    );
    for (auto &it : listOfCommits) {
        printf("=====> %s\n", it.c_str());
    }
    git_libgit2_shutdown();
    printf("---------------------\n");
    return 0;
}
