#bin/bash

# depth_k=("100" "200" "500")
# depth_n=("100" "200" "500" "1000" "2000")

# bin_k=("100" "200" "500")
# bin_n=("100" "200" "500" "1000" "2000")

# ran_k=("100" "200" "500")
# ran_n=("100" "200" "500" "1000" "2000")

depth_k=("1000")
depth_n=("100" "200" "500" "1000" "2000")

bin_k=("1000")
bin_n=("100" "200" "500" "1000" "2000")

ran_k=("1000")
ran_n=("100" "200" "500" "1000" "2000")

for k in "${ran_k[@]}"
do
    :
    for n in "${ran_n[@]}"
    do
        :
        echo "random tree k="$k" n="$n
        ./a -g $k $n 0 > ran-k-n-$k-$n
        ./a << EOF
ran-k-n-$k-$n 512 1
EOF
        ./a << EOF
ran-k-n-$k-$n 1024 1
EOF
        rm ran-k-n-$k-$n
    done
done

for k in "${depth_k[@]}"
do
    :
    for n in "${depth_n[@]}"
    do
        :
        echo "depth tree k="$k" n="$n
        ./a -g $k $n 2 > dep-k-n-$k-$n
        ./a << EOF
dep-k-n-$k-$n 512 1
EOF
        ./a << EOF
dep-k-n-$k-$n 1024 1
EOF
        rm dep-k-n-$k-$n
    done
done

for k in "${bin_k[@]}"
do
    :
    for n in "${bin_n[@]}"
    do
        :
        echo "binary tree k="$k" n="$n
        ./a -g $k $n 1 > bin-k-n-$k-$n
        ./a << EOF
bin-k-n-$k-$n 512 1
EOF
        ./a << EOF
bin-k-n-$k-$n 1024 1
EOF
        rm bin-k-n-$k-$n
    done
done


:<<'END'

echo "depth tree k=10 n=200"
./a -g 10 200 2 > dep-k-n-10-200
./a << EOF
dep-k-n-10-200 512 1
EOF
./a << EOF
dep-k-n-10-200 1024 1
EOF
rm dep-k-n-10-200

echo "depth tree k=10 n="
./a -g 10 75 2 > dep-k-n-10-75
./a << EOF
dep-k-n-10-75 512 1
EOF
./a << EOF
dep-k-n-10-75 1024 1
EOF
rm dep-k-n-10-75

echo "depth tree k=100 n=75"
./a -g 100 75 2 > dep-k-n-100-75
./a << EOF
dep-k-n-100-75 512 1
EOF
./a << EOF
dep-k-n-100-75 1024 1
EOF
rm dep-k-n-100-75
echo "depth tree k=100 n=150"
./a -g 100 150 2 > dep-k-n-100-150
./a << EOF
dep-k-n-100-150 512 1
EOF
./a << EOF
dep-k-n-100-150 1024 1
EOF
rm dep-k-n-100-150
echo "depth tree k=100 n=300"
./a -g 100 300 2 > dep-k-n-100-300
./a << EOF
dep-k-n-100-300 512 1
EOF
./a << EOF
dep-k-n-100-300 1024 1
EOF
rm dep-k-n-100-300

echo "binary tree k=10 n=300"
./a -g 10 300 1 > bin-k-n-10-300
./a << EOF
bin-k-n-10-300 512 1
EOF
./a << EOF
bin-k-n-10-300 1024 1
EOF
rm bin-k-n-10-300
echo "binary tree k=10 n=150"
./a -g 10 150 1 > bin-k-n-10-150
./a << EOF
bin-k-n-10-150 512 1
EOF
./a << EOF
bin-k-n-10-150 1024 1
EOF
rm bin-k-n-10-150
echo "binary tree k=10 n=75"
./a -g 10 75 1 > bin-k-n-10-75
./a << EOF
bin-k-n-10-75 512 1
EOF
./a << EOF
bin-k-n-10-75 1024 1
EOF
rm bin-k-n-10-75

echo "binary tree k=100 n=300"
./a -g 100 300 1 > bin-k-n-100-300
./a << EOF
bin-k-n-100-300 512 1
EOF
./a << EOF
bin-k-n-100-300 1024 1
EOF
rm bin-k-n-100-300
echo "binary tree k=100 n=150"
./a -g 100 150 1 > bin-k-n-100-150
./a << EOF
bin-k-n-100-150 512 1
EOF
./a << EOF
bin-k-n-100-150 1024 1
EOF
rm bin-k-n-100-150
echo "binary tree k=100 n=75"
./a -g 100 75 1 > bin-k-n-100-75
./a << EOF
bin-k-n-100-75 512 1
EOF
./a << EOF
bin-k-n-100-75 1024 1
EOF
rm bin-k-n-100-75

echo "random tree k=10 n=300"
./a -g 10 300 0 > ran-k-n-10-300
./a << EOF
ran-k-n-10-300 512 1
EOF
./a << EOF
ran-k-n-10-300 1024 1
EOF
rm ran-k-n-10-300
echo "random tree k=10 n=150"
./a -g 10 150 0 > ran-k-n-10-150
./a << EOF
ran-k-n-10-150 512 1
EOF
./a << EOF
ran-k-n-10-150 1024 1
EOF
rm ran-k-n-10-150
echo "random tree k=10 n=75"
./a -g 10 75 0 > ran-k-n-10-75
./a << EOF
ran-k-n-10-75 512 1
EOF
./a << EOF
ran-k-n-10-75 1024 1
EOF
rm ran-k-n-10-75

echo "random tree k=100 n=1200"
./a -g 100 1200 0 > ran-k-n-100-1200
./a << EOF
ran-k-n-100-1200 512 1
EOF
./a << EOF
ran-k-n-100-1200 1024 1
EOF
rm ran-k-n-100-1200
echo "random tree k=100 n=600"
./a -g 100 600 0 > ran-k-n-100-600
./a << EOF
ran-k-n-100-600 512 1
EOF
./a << EOF
ran-k-n-100-600 1024 1
EOF
rm ran-k-n-100-600
echo "random tree k=100 n=300"
./a -g 100 300 0 > ran-k-n-100-300
./a << EOF
ran-k-n-100-300 512 1
EOF
./a << EOF
ran-k-n-100-300 1024 1
EOF
rm ran-k-n-100-300
echo "random tree k=100 n=150"
./a -g 100 150 0 > ran-k-n-100-150
./a << EOF
ran-k-n-100-150 512 1
EOF
./a << EOF
ran-k-n-100-150 1024 1
EOF
rm ran-k-n-100-150
echo "random tree k=100 n=75"
./a -g 100 75 0 > ran-k-n-100-75
./a << EOF
ran-k-n-100-75 512 1
EOF
./a << EOF
ran-k-n-100-75 1024 1
EOF
rm ran-k-n-100-75
END