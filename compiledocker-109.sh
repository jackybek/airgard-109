sudo ./killdocker-109.sh
sudo docker build -f dockerfile-109 -t opcuaserver109 --tag 'open62541' .
sudo docker run -it -d --name opcuaserver109 opcuaserver109 bash
sudo docker exec -it opcuaserver109 /bin/bash
