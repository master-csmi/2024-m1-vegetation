FROM ubuntu:24.04

# Set the working directory
WORKDIR /workspace

# Set the timezone
ENV TZ=Europe/Paris

# Install Git LFS
RUN apt-get update && apt-get install -y git-lfs \
	&& git lfs install

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
RUN git clone https://github.com/CGAL/cgal.git /workspace/cgal \
	&& mkdir cgal.build \
	&& cd cgal.build \
	&& cmake -DCMAKE_INSTALL_PREFIX=/workspace/cgal/cgal.install ../cgal \
	&& make install

# Copy the project files into the container
COPY . /workspace/2024-m1-vegetation

# Set the working directory to the project directory
WORKDIR /workspace/2024-m1-vegetation

