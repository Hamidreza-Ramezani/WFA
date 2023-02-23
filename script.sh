#! /bin/bash


l1=100
l2=150
l3=250


dataset_path="/home/hamid/dataset"
pairs_count="5M"
distance="e1"
data1="ERR240727"
data2="SRR826460"
data3="SRR826471"
data_path1="${dataset_path}/${data1}-l${l1}-${distance}-${pairs_count}Pairs" 
data_path2="${dataset_path}/${data2}-l${l2}-${distance}-${pairs_count}Pairs" 
data_path3="${dataset_path}/${data3}-l${l3}-${distance}-${pairs_count}Pairs" 

data_path_temp="${dataset_path}/temp.txt" 

#export OMP_NUM_THREADS=40

#./bin/align_benchmark -i ${data_path1} -a edit-dp
#./bin/align_benchmark -i ${data_path1} -a edit-dp-banded --bandwidth 1
#./bin/align_benchmark -i ${data_path1} -a gap-lineal-nw -p "-1,1,1,1"
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg-banded --bandwidth 1
#program="./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "-1,1,1,1" "
#program="./bin/align_benchmark -i ${data_path_temp} -a gap-affine-wfa -g "-1,1,1,1" "
#./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa-adaptive -g "-1,1,1,1"

#$program

#sudo perf record -e cache-misses $program
#sudo perf report 


prefix="/home/hamid/dataset/temp"
start=`date +%s.%N`


rm $prefix/x*
split -l 250000 -d "$prefix/temp1" "$prefix/x"
end=`date +%s.%N`
echo "$end - $start" | bc -l 

start=`date +%s.%N`
for i in $(seq -w 0 40); do
    filename="$prefix/x$i"
    ./bin/align_benchmark -i "$filename" -a gap-affine-wfa -g "-1,1,1,1" > /dev/null 2>&1 &
done
wait > /dev/null 2>&1
end=`date +%s.%N`
echo "$end - $start" | bc -l 


#rm 0* 1* 2* 3*

