# Luna C++ SDK
Unlike other languages, C++ has more strict requirements to
integrate with gRPC. The easiest way to accomodate these
requirements is to build gRPC as part of the C++ project
and use it to generate the required C++ files from the 
luna.proto file.

## CMake Build
To help simplify the build process, this project uses
[CMake](www.cmake.org). CMake will automatically download
the gRPC source code and include it as a subproject, giving
access to the gRPC libraries and the protoc compiler.

To build as a standalone library execute the following
commands:
```bash
# Create a build directory. It can be named anything and
# can exist outside of the source code directory.
mkdir build-luna-client && cd build-luna-client

# Run CMake to download gRPC and generate makefiles.
# The final path specifies the directory that contains the
# CMakeLists.txt file for the luna_client C++ project. By
# default this will create static libraries. To make a shared
# library instead, add -DBUILD_SHARED_LIBS=TRUE
cmake -DCMAKE_BUILD_TYPE=Release <path/to/sdk-luna/grpc/cpp-luna>
# OR
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE <path/to/sdk-luna/grpc/cpp-luna>

# Build the library
make luna_client
```

To include this CMake project in another one, simply
copy this repository into your project and add the line

```cmake
add_subdirectory(sdk-luna/grpc/cpp-luna)
```

to your project's CMakeLists.txt.

## Build without CMake
When building without CMake, you must manually build and install 
gRPC as [described here](https://grpc.io/docs/quickstart/cpp/).
Once that is done, generate the gRPC/protobuf files by running
protoc:

```bash
LUNA_GRPC_DIR=<path/to/sdk-luna/grpc>
GRPC_INSTALL_DIR=<path/to/grpc/install>
OUTDIR=<path/to/generated/output>
mkdir -p $OUTDIR

PROTO_INCLUDE_PATH=$GRPC_INSTALL_DIR/include/google/protobuf

# Generate files for luna
$GRPC_INSTALL_DIR/bin/protoc \
  -I $PROTO_INCLUDE_PATH:$LUNA_GRPC_DIR \
  --grpc_out=$OUTDIR \
  --cpp_out=$OUTDIR \
  --plugin=protoc-gen-grpc=$GRPC_INSTALL_DIR/bin/grpc_cpp_plugin \
  $LUNA_GRPC_DIR/luna.proto
```

Once the files are generated, include them with the wrapper code
from this directory in your project. Then be sure to link your
binaries with libgrpc, libgrpc++, and libprotobuf (found in
the gRPC installation's `lib` directory).
