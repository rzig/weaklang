# This file is part of weak-lang.
# weak-lang is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# weak-lang is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# You should have received a copy of the GNU Affero General Public License
# along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

FROM ubuntu:20.04

RUN apt clean
RUN apt-get update
RUN apt-get install -y libblas-dev
RUN apt-get install -y libatlas-base-dev
RUN apt-get install -y clang
RUN apt-get install -y make
RUN apt-get install -y gfortran
RUN adduser --gecos "" --disabled-password weak
USER weak
WORKDIR /home/weak
RUN mkdir /home/weak/bin