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

void KGit::checkError(char buffer[], int error) {
    if (error < 0) {
        const git_error *e = git_error_last();
        sprintf(buffer, "Error %d/%d: %s\n", error, e->klass, e->message);
    }
}

static std::string print_signature(const char *header, const git_signature *sig) {
    char buffer[128];
    if(sig) {
        char sign;
        int offset, hours, minutes;
        offset = sig->when.offset;
        if (offset < 0) {
            sign = '-';
            offset = -offset;
        } else {
            sign = '+';
        }
        hours   = offset / 60;
        minutes = offset % 60;
        sprintf(buffer, "%s %s <%s> %ld %c%02d%02d",
                header, sig->name, sig->email, (long)sig->when.time,
                sign, hours, minutes);
    }
    std::string rv(buffer);
    return rv;
}

static std::string getCommitAuthor(const git_commit *commit) {
    char oidstr[GIT_OID_SHA1_HEXSIZE + 1];
    git_oid_tostr(oidstr, sizeof(oidstr), git_commit_tree_id(commit));
    return print_signature("author: ", git_commit_author(commit));
}

static std::string getCommitCommitter(const git_commit *commit) {
    std::string rv;
    char oidstr[GIT_OID_SHA1_HEXSIZE + 1];
    git_oid_tostr(oidstr, sizeof(oidstr), git_commit_tree_id(commit));
    return print_signature("committer: ", git_commit_committer(commit));
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

std::vector<std::string> KGit::listBranches(git_repository *repository, const char *path, git_branch_t branch_type) {
    std::vector<std::string> rv;
    char error_buffer[128];
    int error = git_repository_open(&repository, path);
    checkError(error_buffer, error);
    git_branch_iterator *it;
    if (!git_branch_iterator_new(&it, repository, branch_type)) {
        git_reference *ref;
        git_branch_t type;
        while (!git_branch_next(&ref, &type, it)) {
            rv.emplace_back(git_reference_name(ref));
            git_reference_free(ref);
        }
        git_branch_iterator_free(it);
    }
    git_repository_free(repository);
    return rv;
}

std::vector<std::string> KGit::revisionWalking(git_repository *repository, const char *path, const char *branch_name) {
    std::vector<std::string> rv;
    char error_buffer[128];
    int error = git_repository_open(&repository, path);
    checkError(error_buffer, error);
    git_revwalk *walker;
    error = git_revwalk_new(&walker, repository);
    checkError(error_buffer, error);
    error = git_revwalk_push_head(walker);
    checkError(error_buffer, error);
    git_oid oid;
    git_commit *commit = nullptr;
    git_tree *tree = nullptr;
    while (!git_revwalk_next(&oid, walker)) {
        git_commit_lookup(&commit, repository, &oid);
        rv.emplace_back(getCommitAuthor(commit) + ";" +getCommitCommitter(commit));
        git_commit_tree(&tree, commit);
        /* size_t cnt = git_tree_entrycount(tree); */
        git_tree_free(tree);
    }
    git_repository_free(repository);
    return rv;
}
