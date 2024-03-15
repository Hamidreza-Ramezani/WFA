l1=100
l2=150
l3=250
l4=500
l5=1000
l6=5000
l7=10000
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
data_path6="${dataset_path}/${synthetic}-l${l6}-${distance}-1MPairs"
data_path7="${dataset_path}/${synthetic}-l${l7}-${distance}-1MPairs"
results_dir="$HOME/cortes-roofline-results/march15/para-wfa"


advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l1}-${distance}-${pairs_count}Pairs-V2"  --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "0,3,4,1"

#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l2}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path2} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l3}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path3} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l4}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path4} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l5}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path5} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l6}-${distance}-1MPairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path6} -a gap-affine-wfa-adaptive -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l7}-${distance}-1MPairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path7} -a gap-affine-wfa-adaptive -g "0,3,4,1"




l1=100
l2=150
l3=250
l4=500
l5=1000
l6=5000
l7=10000
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
results_dir="$HOME/cortes-roofline-results/para-wfa"


#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l1}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path1} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l2}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path2} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l3}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path3} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l4}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path4} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l5}-${distance}-${pairs_count}Pairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path5} -a gap-affine-wfa -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l6}-${distance}-1MPairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path6} -a gap-affine-wfa-adaptive -g "0,3,4,1"
#
#advisor -collect roofline -stacks -enable-cache-simulation -project-dir "${results_dir}/l${l7}-${distance}-1MPairs" --app-working-dir=$HOME/WFA/bin -- $HOME/WFA/bin/align_benchmark -i ${data_path7} -a gap-affine-wfa-adaptive -g "0,3,4,1"


