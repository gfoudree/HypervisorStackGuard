FROM joshwyant/gcc-cross:latest
MAINTAINER Grant Foudree

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \ 
	apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" install nasm grub2-common xorriso grub-pc --assume-yes -y

