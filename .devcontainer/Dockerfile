FROM debian:bookworm

RUN apt-get update
RUN apt-get install -y \
    build-essential git checkinstall libssl-dev openssl \
    cmake golang-go \
    python3 python3-pip python3-venv libsass-dev libcairo2 libpango-1.0-0 libpangoft2-1.0-0 pangocairo-1.0 pngquant \
    sudo

## dev user
RUN useradd -ms /bin/bash dev \
    && apt-get update \
    && apt-get install -y sudo \
    && echo dev ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/dev \
    && chmod 0440 /etc/sudoers.d/dev

USER dev
WORKDIR /home/dev

## Python will run in venv
RUN python3 -m venv /home/dev/venv
ENV PATH="/home/dev/venv/bin:$PATH"
