// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mini_client.hpp"
#include "google/cloud/storage/client.h"
#include "google/cloud/storage/grpc_plugin.h"
#include <iostream>
#include <memory>
#include <string>

// Create aliases to make the code easier to read.
namespace gc = ::google::cloud;
namespace gcs = gc::storage;
namespace gcs_experimental = gc::storage_experimental;

const std::string kExceptionThrown = "EXCEPTION_THROWN";

void SetErrorCode(gc::StatusCode code, char *dest) {
    auto str = gc::StatusCodeToString(code);
    dest[str.copy(dest, 24)] = '\0';
}

extern "C" {

    struct GoogleStorageClient {
        gcs::Client client;
    };

    GoogleStorageClient* CreateGCSClient(enum ClientAPI client_api, const char* project) {
      gc::Options options{};
      switch(client_api) {
      case GRPC_DIRECTPATH:
         options.set<gcs_experimental::GrpcPluginOption>("media")
                .set<gc::EndpointOption>("google-c2p-experimental:///storage.googleapis.com");
	 break;
      case GRPC_NO_DIRECTPATH:
         options.set<gcs_experimental::GrpcPluginOption>("media")
                .set<gc::EndpointOption>("storage.googleapis.com");
         break;
      case JSON:
	  options.set<gcs_experimental::GrpcPluginOption>("none")
	         .set<gc::EndpointOption>("google-c2p-experimental:///storage.googleapis.com"); // ignored
	  break;
      }

        auto client_wrapper = new GoogleStorageClient;
        client_wrapper->client = google::cloud::storage_experimental::DefaultGrpcClient(options);
        return client_wrapper;
    }

    void DestroyGCSClient(GoogleStorageClient* client) {
        delete client;
    }

    static const uint64_t READ_BUFFER_SIZE = 262144;

    CallResult ReadObject_Internal(GoogleStorageClient* client, const char* bucket, const char* obj) {
        CallResult result;
        result.success = false;

        std::int64_t bytes_received{0};
        // Allocating this elsewhere would probably be faster, but that's hard
        // and we'll probably still be within 1 ms of the right answer.
        std::vector<char> buffer(READ_BUFFER_SIZE);

        auto stream = client->client.ReadObject(bucket, obj);
        if (stream.bad()) {
            std::cerr << "Error reading object: " << stream.status() << "\n";
	    SetErrorCode(stream.status().code(), result.error_code);
	    return result;
        }
        do {
            stream.read(buffer.data(), READ_BUFFER_SIZE);
            bytes_received += stream.gcount();
        } while(stream);

        stream.Close();
	SetErrorCode(stream.status().code(), result.error_code);
        if (stream.status().ok()) {
            result.success = true;
            result.bytes_received = bytes_received;
        } else {
            std::cerr << "Failure reading object: " << stream.status() << std::endl;
        }

        return result;
    }

    // Client seems to sometimes throw C++ exceptions, so we must wrap them here because
    // Rust (and any other C calling convention tool) does not know how to handle those.
    CallResult ReadObject(GoogleStorageClient* client, const char* bucket, const char* obj) {
	try {
          return ReadObject_Internal(client, bucket, obj);
	} catch(std::exception const& e) {
           CallResult rr;
	   rr.success = false;
           std::cerr << "Encountered exception: " << e.what() << std::endl;
	   kExceptionThrown.copy(rr.error_code, 24);
	   return rr;
	} catch(...) {
           CallResult rr;
	   rr.success = false;
           std::cerr << "Unknown C++ exception occurred" << std::endl;
	   kExceptionThrown.copy(rr.error_code, 24);
	   return rr;
	}
    }

}
