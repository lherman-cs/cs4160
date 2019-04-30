FROM golang:1.12 as builder

WORKDIR /go/src/app
COPY ./go.mod ./go.sum ./
RUN go mod download
COPY ./ ./
RUN GO111MODULE=on CGO_ENABLED=0 go build

FROM ubuntu:18.04

RUN apt-get update && apt-get install -y websockify \
  && rm -rf /var/lib/apt/lists/*

COPY --from=builder /go/src/app/server /
EXPOSE 8080
EXPOSE 8081
CMD (websockify 8080 localhost:8081 &) && /server
