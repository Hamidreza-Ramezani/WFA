#! /bin/bash


l1=100
l2=150
l3=250
l4=500
l5=1000
l6=5000
l7=10000

user="hamidkeb"
dataset_path="/data/hamid"
pairs_count="5M"
distance="e1"
data1="ERR240727"
data2="SRR826460"
data3="SRR826471"
synthetic="synthetic"


data_path1="${dataset_path}/${data1}-l${l1}-${distance}-${pairs_count}Pairs" 
data_path2="${dataset_path}/${data2}-l${l2}-${distance}-${pairs_count}Pairs" 
data_path3="${dataset_path}/${data3}-l${l3}-${distance}-${pairs_count}Pairs" 
data_path4="${dataset_path}/${synthetic}-l${l4}-${distance}-${pairs_count}Pairs" 
data_path5="${dataset_path}/${synthetic}-l${l5}-${distance}-${pairs_count}Pairs" 
data_path6="${dataset_path}/${synthetic}-l${l6}-${distance}-${pairs_count}Pairs" 
data_path7="${dataset_path}/${synthetic}-l${l7}-${distance}-${pairs_count}Pairs" 

#./bin/align_benchmark -i ${data_path1} -a edit-dp
#./bin/align_benchmark -i ${data_path1} -a edit-dp-banded --bandwidth 1
#./bin/align_benchmark -i ${data_path1} -a gap-lineal-nw -p "-1,1,1,1"
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg-banded --bandwidth 1
program="./bin/align_benchmark -i ${data_path5} -a gap-affine-swg -g "-1,1,1,1"" 
#program="./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "-1,1,1,1"" 
#program="./bin/align_benchmark -i ${data_path2} -a gap-affine-wfa -g "-1,1,1,1""
#program="./bin/align_benchmark -i ${data_path3} -a gap-affine-wfa -g "-1,1,1,1""
#program="./bin/align_benchmark -i ${data_path4} -a gap-affine-wfa -g "-1,1,1,1""
#program="./bin/align_benchmark -i ${data_path5} -a gap-affine-wfa -g "-1,1,1,1""
#program="./bin/align_benchmark -i ${data_path6} -a gap-affine-wfa-adaptive -g "-1,1,1,1""
#program="./bin/align_benchmark -i ${data_path7} -a gap-affine-wfa-adaptive -g "-1,1,1,1""


start=`date +%s.%N`
$program
end=`date +%s.%N`
echo "$end - $start" | bc -l 




#prefix="/home/hamid/dataset/temp"
#rm $prefix/x*
#start=`date +%s.%N`
#split -l 25000 -d "$prefix/synthetic-l5000-e1-1MPairs" "$prefix/x"
#end=`date +%s.%N`
##echo "$end - $start" | bc -l 
##start=`date +%s.%N`
#for i in $(seq -w 0 39); do
#    filename="$prefix/x$i"
#    #./bin/align_benchmark -i "$filename" -a gap-affine-wfa -g "-1,1,1,1" > /dev/null &
#    ./bin/align_benchmark -i "$filename" -a gap-affine-wfa-adaptive -g "-1,1,1,1" > /dev/null 2>&1 &
#    valgrind --tool=cachegrind ./bin/align_benchmark -i "$filename" -a gap-affine-wfa-adaptive > ./stat.txt
#done
#wait > /dev/null
#end=`date +%s.%N`
#echo "$end - $start" | bc -l 


