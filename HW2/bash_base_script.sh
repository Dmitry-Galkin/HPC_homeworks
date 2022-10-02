#!/bin/bash

# 1. even number print
for i in `seq 100 2 1000`
do
    echo $i
done


# 2. print array elements
array=(1 2 3 4 5 apple 7 8 melon made)

for el in ${array[@]}
do
    echo $el
done


# 3. sum with float number
echo "100 + 0.5" | bc


# 4. check if directory exists
if [[ -d "./Linux" ]]
then
    echo "course"
else
    echo "very easy"
    echo "course is easy" > Linux
fi

