# checksums linux client
Application to verify checksums of files.

## Build Requirements
1. gtkmm-3.0
2. openssl
3. libcurl
4. libsecret

* Special thanks to maintainers of above projects

## Build

1. `make` - Build a debug build..
2. `make BUILD=release` - Build a release build.
3. `make BUILD=sanitize_address` - Build with address sanitizer.