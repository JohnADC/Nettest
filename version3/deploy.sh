#!bash /bin/bash
make
client=$1
server=$2
hcp client $client:
hcp ctest.sh $client:
hcp server $server:
hcp stest.sh $server:

echo "deployed, hopefully"
