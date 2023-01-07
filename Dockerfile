FROM ubuntu:16.04

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install g++ php valgrind curl massif-visualizer -y
RUN apt-get update && apt-get install make
EXPOSE 8081
EXPOSE 8080
EXPOSE 444

# docker build .
# docker image ls
# docker run -p 127.0.0.1:8081:8081 -ti -v $(PWD):/test 2e8bddf1900b bash -c "cd /test/; make rebug && valgrind --tool=massif --stacks=yes ./webserv conf/webserv.conf; ls; ms_print massif.out.* > memory.profiling ; rm -f ./webserv"
			# other valgrind flags: --leak-check=full --show-leak-kinds=all