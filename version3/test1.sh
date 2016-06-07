file="./tests.txt"
border="-------------------------"
time=`date`

if [[ ! -f $file ]]; then
  touch $file
fi

echo "$border" >> tests.txt
echo "$time" >> tests.txt
echo "$border" >> tests.txt

for pps in 100 500 1000 2000 4000 6000 8000 10000 12000
do 
	echo "Packets per second $pps" >> tests.txt
	./client 10.0.1.20 $pps
	./client 10.0.1.20 $pps -s16376 -t1
	./client 10.0.1.20 $pps -s16376 -t10
	./client 10.0.1.20 $pps -s4 -t10
	./client 10.0.1.20 $pps -s4 -t1
done >> tests.txt

echo "$border" >> tests.txt

echo "done testing"