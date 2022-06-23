
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

int main() {

    for(int i=0; i<2; i++) {
      GoogleStorageClient* client = CreateGCSClient(GRPC_NO_DIRECTPATH, "gcs-grpc-team-testing");
      CallResult rr = ReadObject(client, "gcs-grpc-team-yarbrough-test-1", "test.txt");
      std::cerr << "Bytes read: " << rr.bytes_received << std::endl;
      std::cerr << "Status: " << rr.error_code << std::endl;
      DestroyGCSClient(client);
    }
    return 0;
}
