# checksums linux client
Application to verify checksums of files.

## Install .deb file

1. Download .deb file from either of these web pages.   
   a). https://checksums.rammini.com/   
   b). https://github.com/chamalsl/checksum_linux_client/releases
2. Run this command to install.
   `sudo apt install ./checksums_{VERSION}_amd64.deb`   

   apt will display this error message.
   > N: Download is performed unsandboxed as root as file 'checksums_{VERSION}_amd64.deb' 
   > couldn't be accessed by user '_apt'. - pkgAcquire::Run (13: Permission denied)

   **Please ignore above error message.**
3. Run application.   
   `checksums`
   

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

Built executable will be saved in build/{BUILD}/checksums.

For debug builds, executable will be saved in build/debug/checksums.
For release builds, executable will be saved in build/release/checksums.
