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

shutdown() {
    docker kill weak-lang
    docker rm weak-lang
}

trap shutdown EXIT

docker build . -t weak-lang
docker run -td --name weak-lang --mount type=bind,source="$(pwd)",target=/home/weak weak-lang
docker exec -w /home/weak weak-lang make
docker exec -it -w /home/weak weak-lang /bin/bash
