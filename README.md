# Cloud C++ Mini Client

This is a small wrapper around the [google-cloud-cpp](https://github.com/googleapis/google-cloud-cpp)
storage C++ client library.

It is intended for use driving small load tests for languages that need a C ABI.

# Instructions

To build with Bazel, run `bazel build :mini_client`. You will be left with a
.so file wrapping the C++ client that can be used with the `mini_client.hpp`
C header.

There is a small `mini_client_test` binary designed for quickly verifying that
the client works.
