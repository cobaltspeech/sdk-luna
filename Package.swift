// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.
//
// Copyright (2019) Cobalt Speech and Language Inc.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import PackageDescription

let package = Package(
    name: "Luna",
    platforms: [
        .macOS(.v10_12),
        .iOS(.v10)
    ],
    products: [
        .library(
            name: "Luna",
            targets: ["Luna"]),
    ],
    dependencies: [
        .package(url: "git@github.com:grpc/grpc-swift.git", .exact("1.6.1"))
    ],
    targets: [
        .target(
            name: "Luna",
            dependencies: [
                .product(name: "GRPC", package: "grpc-swift"),
            ],
            path: "grpc/swift-cubic")
    ]
)
