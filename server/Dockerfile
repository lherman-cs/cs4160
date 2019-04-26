FROM golang as builder

WORKDIR /go/src/app
COPY . .
RUN go get -v ./...
RUN go build

FROM ubuntu

RUN apt update && \
    apt install -y python3-websockify

COPY --from=builder /go/src/app/app /
EXPOSE 8080
EXPOSE 8081
CMD (websockify 8080 localhost:8081 &) && /app