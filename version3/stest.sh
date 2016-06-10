file="stests.txt"

if [[ ! -f $file ]]; then
  touch $file
fi

./server > stests.txt
