shutdown() {
    docker kill weak-lang
    docker rm weak-lang
}

trap shutdown EXIT

docker build . -t weak-lang
docker run -td --name weak-lang --mount type=bind,source="$(pwd)",target=/home/weak weak-lang
docker exec -it -w /home/weak weak-lang /bin/bash