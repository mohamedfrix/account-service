# Build stage
FROM mohamedfrix/cpp-base-image:latest AS builder

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Set environment variables
ENV MY_INSTALL_DIR=/usr/local
ENV PATH="$MY_INSTALL_DIR/bin:$PATH"
ENV LD_LIBRARY_PATH="$MY_INSTALL_DIR/lib:$MY_INSTALL_DIR/lib64"
ENV PKG_CONFIG_PATH="$MY_INSTALL_DIR/lib/pkgconfig"

# Ensure gRPC source is available
RUN test -d /opt/grpc || (echo "gRPC source not found in base image" && exit 1)

# Build application
WORKDIR /app
COPY . .
RUN mkdir -p build
WORKDIR /app/build

# Configure and build with detailed output
RUN cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    cmake --build . --config Release --verbose

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && \
    apt-get install -y \
    libstdc++6 \
    libssl3 \
    libpq5 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy binary and libraries
COPY --from=builder /app/build/accounts-service .
COPY --from=builder /usr/local/lib/*.so* /usr/local/lib/
COPY .env .

# Configure runtime
ENV LD_LIBRARY_PATH=/usr/local/lib:/usr/lib
RUN ldconfig

EXPOSE 8081
ENTRYPOINT ["./accounts-service"]
CMD ["--host", "0.0.0.0", "--port", "8081"]