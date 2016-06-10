size=$1
time=$2


file="./ctests.txt"

if [[ ! -f $file ]]; then
  touch $file
fi

for pps in 1000 50000 100000 200000 400000 600000 800000 1000000
do 
	./client 10.0.1.20 $pps -t $time -s $size

done > tests.txt

echo "size is $size"
echo "time is $time"

echo "done testing"