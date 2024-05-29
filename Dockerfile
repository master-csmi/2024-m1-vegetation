FROM ubuntu:24.04

# Set the working directory
WORKDIR /workspace

# Set the timezone
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
RUN git clone https://github.com/CGAL/cgal.git /workspace/cgal \
	&& mkdir cgal.install \
	&& cd cgal.install \
	&& cmake -DCMAKE_INSTALL_PREFIX=/workspace/cgal/cgal.install ../cgal \
	&& make install

# Clone the project
RUN git clone https://github.com/master-csmi/2024-m1-vegetation.git /workspace/2024-m1-vegetation

# Build the project
RUN cd /workspace/2024-m1-vegetation \
	&& mkdir build \
	&& cd build \
	&& cmake -DCMAKE_PREFIX_PATH=/workspace/cgal/cgal.install .. \
	&& make

