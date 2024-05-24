FROM ubuntu:24.04

# Copy project files
COPY . /vegetation
WORKDIR /vegetation

# Set the timezone to Europe/Paris
ENV TZ=Europe/Paris

# Install system dependencies
RUN apt-get update && \
	apt-get install -y \
	build-essential \
	cmake \
	git \
	libmpfr-dev \
	libboost-dev \
	libcurl4-openssl-dev \
	libcgal-dev \
	libfmt-dev && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*




