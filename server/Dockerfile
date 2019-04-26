FROM golang as builder

WORKDIR /go/src/app
COPY . .
RUN go get -v ./...
RUN CGO_ENABLED=0 go build

FROM python:alpine

RUN apk add make automake gcc g++ subversion python3-dev git && \
    git clone --depth=1 https://github.com/novnc/websockify.git && \
    cd websockify && \
    python setup.py install

COPY --from=builder /go/src/app/app /
EXPOSE 8080
EXPOSE 8081
CMD (websockify 8080 localhost:8081 &) && /app