
#include "mini_client.hpp"
#include <iostream>

#include "google/cloud/storage/client.h"
#include "google/cloud/storage/grpc_plugin.h"
#include <iostream>
#include <memory>
#include <string>

namespace gc = ::google::cloud;
namespace gcs = gc::storage;
namespace gcs_experimental = gc::storage_experimental;

const char BUCKET_NAME[] = "gcs-grpc-team-perf-testing-us-central1";
const char OBJECT_NAME[] = "writes/will_never_finalize";

int main() {

    // Works
    // GoogleStorageClient* client = CreateGCSClient(GRPC_NO_DIRECTPATH, "gcs-grpc-team-testing");

    // Does not work
    GoogleStorageClient* client = CreateGCSClient(JSON, "gcs-grpc-team-testing");

    CallResult start_write_result = StartResumableWrite(client, BUCKET_NAME, OBJECT_NAME);
    if (!start_write_result.success) {
      return 1;
    }

    const char* upload_id = start_write_result.upload_id;
    std::cerr << "Upload ID: '"<< upload_id << "'" << std::endl;

    CallResult query_write_result = QueryWriteStatus(client, upload_id);
    if (query_write_result.success) {
      std::cerr << "Query succeeded";
    } else {
      std::cerr << "QueryWriteStatus failed: " << query_write_result.error_code << std::endl;
    }
    DestroyGCSClient(client);

    return 0;
}
