workspace(name = "cloud-cpp-mini-client")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

#load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
#Whttp_archive(
    #name = "cloud",
    #urls = ["https://github.com/testrunner/v1.zip"],
    #sha256 = "...",
#)

git_repository(
    name = "com_github_googleapis_google_cloud_cpp",
    remote = "https://github.com/googleapis/google-cloud-cpp.git",
    branch = "main",
)

load("@com_github_googleapis_google_cloud_cpp//bazel:google_cloud_cpp_deps.bzl", "google_cloud_cpp_deps")
google_cloud_cpp_deps()

load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")
switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,  # C++ support is only "Partially implemented", roll our own.
    grpc = True,
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()

