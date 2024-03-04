#include "KGit.h"

KGit* KGit::instance{nullptr};
std::mutex KGit::mutex_;

KGit *KGit::GetInstance(){
    if(instance == nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance == nullptr) {
            instance = new KGit;
        }
    }
    return instance;
}

void KGit::cloneRepository(git_repository *repository, const char *url, const char *path) {
    printf("Cloning ...\n");
    int error = git_clone(&repository, url, path, nullptr);
    if (error < 0) {
        const git_error *e = git_error_last();
        printf("Error by cloning %d/%d: %s\n", error, e->klass, e->message);
        /* exit(error); */
    }
}

void KGit::listBranches(git_repository *repository, const char *path, git_branch_t branch_type) {
    int error = git_repository_open(&repository, path);
    if (error < 0) {
        const git_error *e = git_error_last();
        printf("Error by opening %d/%d: %s\n", error, e->klass, e->message);
        /* exit(error); */
    }
    git_branch_iterator *it;
    if (!git_branch_iterator_new(&it, repository, branch_type)) {
        git_reference *ref;
        git_branch_t type;
        while (!git_branch_next(&ref, &type, it)) {
            // Do something with 'ref' and 'type'.
            printf("\t%s\n", git_reference_name(ref));
            git_reference_free(ref);
        }
        git_branch_iterator_free(it);
    }
    git_repository_free(repository);
}

void KGit::revwalking(git_repository *repository, const char *path) {
    printf("Revision walking '%s' ...\n", path);
    int error = git_repository_open(&repository, path);
    if (error < 0) {
        const git_error *e = git_error_last();
        printf("Error by opening %d/%d: %s\n", error, e->klass, e->message);
        /* exit(error); */
    }
    git_revwalk *walk;
    git_commit *wcommit;
    git_oid oid;
    const char *cmsg;
    const git_signature *cauth;

    printf("\t1\n");
    git_revwalk_new(&walk, repository);
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
    git_revwalk_push(walk, &oid);

    printf("\t2\n");
    while ((git_revwalk_next(&oid, walk)) == 0) {
        error = git_commit_lookup(&wcommit, repository, &oid);
        cmsg  = git_commit_message(wcommit);
        cauth = git_commit_author(wcommit);
        printf("\t%s (%s)\n", cmsg, cauth->email);
        git_commit_free(wcommit);
    }
    printf("\t98\n");
    git_revwalk_free(walk);
    printf("\t99\n");
    git_repository_free(repository);
}
