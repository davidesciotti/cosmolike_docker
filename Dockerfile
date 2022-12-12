FROM ubuntu:latest

LABEL mantainers="marco.bonici@ge.infn.it"
LABEL version="0.0.1"
LABEL description="This Dockerfile create a Docker image where is possible to run Cobaya"

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y
RUN apt-get install apt-utils -y
RUN apt-get upgrade -y
RUN apt-get autoremove -y
RUN apt-get install python3 -y
RUN apt-get install python3-distutils -y
RUN apt-get install python3-pip -y
RUN apt-get update
RUN apt-get install git -y
RUN apt-get install gfortran -y
RUN apt-get install libblas-dev liblapack-dev liblapacke-dev -y
RUN apt-get install libpq-dev python3-dev -y
RUN apt-get install -y python3-mpi4py
#RUN pip3 install utils
RUN pip3 install numpy numba scipy cython PyYAML pytest flaky pandas matplotlib quadpy simpy getdist

# clone Robin's repo
RUN git clone https://github.com/robinupham/CosmoCov_ClCov.git

# install gsl
RUN apt-get install -y libgsl-dev

# install fftw
# RUN cd cosmolike_installation_files/fftw-3.3.4/
# RUN configure; make; make install
# alternative fftw install?
RUN apt-get install -y libfftw3-dev libfftw3-doc

