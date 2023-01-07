FROM ubuntu:16.04

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install g++ php valgrind curl -y
RUN apt-get update && apt-get install make
EXPOSE 8081
EXPOSE 8080
EXPOSE 444