#include <cstdio>

#include "KGit.h"

/*
 * cd .. && rmdir /q /s build temp && mkdir build temp && cd build && cmake -S .. -B . && cmake --build . && copy "C:\Program
 Files (x86)\libgit2\bin\git2.dll" . && Debug\KGit.exe
 */

int main(int argc, char *argv[]) {
    printf("---------------------\n");
    git_libgit2_init();
    git_repository *repository = nullptr;

    KGit *kendoGit = KGit::GetInstance();
    KGit::listBranches(repository, "C:\\src\\KGit\\.git");

    git_libgit2_shutdown();
    printf("---------------------\n");
    return 0;
}
