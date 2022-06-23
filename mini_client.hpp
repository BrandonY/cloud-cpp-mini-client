// Header for mini client

#ifndef __GOOGLE_CLOUD_STORAGE_CLIENT_H__
#define __GOOGLE_CLOUD_STORAGE_CLIENT_H__

extern "C" {

enum ClientAPI {
	GRPC_DIRECTPATH,
	GRPC_NO_DIRECTPATH,
	JSON
}; 

// Opaque client holder for Rust.
    struct GoogleStorageClient;

    typedef struct CallResult {
        bool success;
        unsigned long bytes_received;  // would be uint64_t but stdint adds a bunch of stuff to bindings.
	char error_code[25];
    } CallResult;

    GoogleStorageClient* CreateGCSClient(enum ClientAPI client_api, const char* project);
    void DestroyGCSClient(GoogleStorageClient* client);

    CallResult ReadObject(GoogleStorageClient* client, const char* bucket, const char* obj);

}

#endif // __GOOGLE_CLOUD_STORAGE_CLIENT_H__
