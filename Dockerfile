FROM ubuntu:24.04
RUN apt-get update && apt-get install -y build-essential libgtest-dev cmake
ENV PATH="/opt/venv/bin:$PATH"
WORKDIR /root/app/
COPY . .
CMD ["./build.sh"]
