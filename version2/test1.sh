file="./tests.txt"

if [[ ! -f $file ]]; then
  touch $file
fi


for pps in 50 100 200 500 1000 2000 4000 6000 8000 10000 12000 14000 
do
	./client 10.0.1.20 $pps
	./client 10.0.1.20 $pps -s16376 -t1
	./client 10.0.1.20 $pps -s16376 -t10
	./client 10.0.1.20 $pps -s1 -t10
	./client 10.0.1.20 $pps -s1 -t1
done >> tests.txt

echo "-------------------------" >> tests.txt

echo "done testing"