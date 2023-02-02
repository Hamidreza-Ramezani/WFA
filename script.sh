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

export OMP_NUM_THREADS=8

#./bin/align_benchmark -i ${data_path1} -a edit-dp
#./bin/align_benchmark -i ${data_path1} -a edit-dp-banded --bandwidth 1
#./bin/align_benchmark -i ${data_path1} -a gap-lineal-nw -p "-1,1,1,1"
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg
#./bin/align_benchmark -i ${data_path1} -a gap-affine-swg-banded --bandwidth 1
./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "-1,1,1,1"
#./bin/align_benchmark -i ${data_path1} -a gap-affine-wfa-adaptive -g "-1,1,1,1"



