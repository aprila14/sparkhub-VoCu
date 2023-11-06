ARG UBUNTU_VERSION=18.04
FROM ubuntu:${UBUNTU_VERSION}

LABEL maintainer="Radoslaw Golebiewski <radoslaw.golebiewski@wizzdev.pl>"
LABEL version="1.0"

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

RUN apt-get update && apt-get --yes install apt-utils

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo "Europe/London" > /etc/timezone

RUN apt-get update && apt-get --yes install sudo && echo "%sudo ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

RUN apt-get update && apt-get install --yes ca-certificates && /usr/sbin/update-ca-certificates

# RUN build/clean_repo_and_patch.sh
