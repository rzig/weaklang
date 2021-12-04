FROM ubuntu:20.04

RUN apt clean
RUN apt-get update
RUN apt-get install -y libblas-dev
RUN apt-get install -y libatlas-base-dev
RUN apt-get install -y clang
RUN apt-get install -y make
RUN apt-get install -y gfortran
RUN adduser --gecos "" --disabled-password weak
USER weak
WORKDIR /home/weak
RUN mkdir /home/weak/bin