
# Use the official Ubuntu base image
FROM ubuntu:24.04

# Set the working directory
WORKDIR /workspace

ENV TZ=Europe/Paris

# Install dependencies
RUN apt-get update && apt-get install -y \
	build-essential \
	cmake \
	git \
	libmpfr-dev \
	libboost-dev \
	libcurl4-openssl-dev \
	libfmt-dev

# Clone CGAL master branch and install it
RUN git clone --branch master https://github.com/CGAL/cgal.git \
	&& cd cgal \
	&& mkdir cgal.install \
	&& cd cgal.install \
	&& cmake -DCMAKE_INSTALL_PREFIX=/workspace/cgal.install .. \
	&& make install

RUN git clone https://github.com/master-csmi/2024-m1-vegetation.git /workspace/2024-m1-vegetation

# Set the working directory
WORKDIR /workspace/2024-m1-vegetation

# Build the project
RUN mkdir build \
	&& cd build \
	&& cmake -DCMAKE_PREFIX_PATH=/workspace/cgal.install .. \
	&& make

# Set the entry point
ENTRYPOINT ["/workspace/2024-m1-vegetation/build/vegetation"]