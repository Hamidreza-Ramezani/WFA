#! /bin/bash


l1=100
l2=150
l3=250
l4=500
l5=1000
l6=5000
l7=10000

user="hamidkeb"
#dataset_path="/home/$user/dataset"
dataset_path="/data/hamid"
pairs_count="5M"
distance="e5"
data1="ERR240727"
data2="SRR826460"
data3="SRR826471"
synthetic="synthetic"


data_path1="${dataset_path}/${data1}-l${l1}-${distance}-${pairs_count}Pairs" 
data_path2="${dataset_path}/${data2}-l${l2}-${distance}-${pairs_count}Pairs" 
data_path3="${dataset_path}/${data3}-l${l3}-${distance}-${pairs_count}Pairs" 
data_path4="${dataset_path}/${synthetic}-l${l4}-${distance}-${pairs_count}Pairs" 
data_path5="${dataset_path}/${synthetic}-l${l5}-${distance}-${pairs_count}Pairs" 
data_path6="${dataset_path}/${synthetic}-l${l6}-${distance}-1MPairs" 
data_path7="${dataset_path}/${synthetic}-l${l7}-${distance}-1MPairs" 

#./bin/align_benchmark -i ${data_path1} -a edit-dp
#./bin/align_benchmark -i ${data_path1} -a edit-dp-banded --bandwidth 1
#./bin/align_benchmark -i ${data_path1} -a gap-lineal-nw -p "-1,1,1,1"
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg-banded --bandwidth 1
program="./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "0,3,4,1"" 
#program="./bin/align_benchmark -i ${data_path2} -a gap-affine-wfa -g "0,3,4,1""
#program="./bin/align_benchmark -i ${data_path3} -a gap-affine-wfa -g "0,3,4,1""
#program="./bin/align_benchmark -i ${data_path4} -a gap-affine-wfa -g "0,3,4,1""
#program="./bin/align_benchmark -i ${data_path5} -a gap-affine-wfa -g "0,3,4,1""
#program="./bin/align_benchmark -i ${data_path6} -a gap-affine-wfa-adaptive -g "0,3,4,1""
#program="./bin/align_benchmark -i ${data_path7} -a gap-affine-wfa-adaptive -g "0,3,4,1""

#$program1
#$program2
#$program3
#$program4
#$program5


start=`date +%s.%N`
$program
end=`date +%s.%N`
echo "$end - $start" | bc -l 

#sudo perf stat --per-socket -e power/energy-pkg/,power/energy-ram/ $program
#sudo perf stat --per-socket -e power/energy-ram/ $program

