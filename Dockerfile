# Build stage
FROM mohamedfrix/cpp-base-image:latest AS builder

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Ensure gRPC source is available
RUN test -d /opt/grpc || (echo "gRPC source not found in base image" && exit 1)

# Build application
WORKDIR /app
COPY . .
RUN mkdir -p build  
WORKDIR /app/build
RUN cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DGRPC_AS_SUBMODULE=ON && \
    cmake --build . --config Release

# Runtime stage
FROM ubuntu:22.04

# Install minimal runtime dependencies
RUN apt-get update
RUN apt-get install -y libstdc++6 \
    libssl3 \
    libpq5

RUN rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy necessary files from builder
COPY --from=builder /app/build/accounts-service .
COPY --from=builder /usr/local/lib/*.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libproto* /usr/local/lib/
COPY --from=builder /usr/local/lib/libgrpc* /usr/local/lib/
COPY .env .

# Configure container
EXPOSE 8081
ENV LD_LIBRARY_PATH=/usr/local/lib:/usr/lib
ENTRYPOINT ["./accounts-service"]
CMD ["--host", "0.0.0.0", "--port", "8081"]