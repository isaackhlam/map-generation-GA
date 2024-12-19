FROM debian:12.5

# Install dependency
RUN apt-get update
RUN apt-get install -y g++ wget cmake
RUN apt-get install -y libopengl-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libxrandr-dev libxcursor-dev libfreetype-dev libgl1-mesa-dev

# Build SFML
WORKDIR /root
RUN wget https://github.com/SFML/SFML/archive/refs/tags/2.6.1.tar.gz
RUN tar xvf 2.6.1.tar.gz
WORKDIR /root/SFML-2.6.1
RUN cmake -DBUILD_SHARED_LIBS=false .
RUN make -j 8 && make install

# Compile game
COPY ./src /root/src
WORKDIR /root/src
RUN g++ -c main.cpp
RUN g++ main.o -o sfml-app -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -ludev -lX11 -lXcursor -lXrandr
