FROM gcc:13

RUN apt-get update && apt-get install -y cmake make

WORKDIR /app

COPY . .

RUN mkdir build

WORKDIR /app/build

RUN cmake ..
RUN cmake --build .

EXPOSE 8080

CMD ["./server"]