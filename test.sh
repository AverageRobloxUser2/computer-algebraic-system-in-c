cd build
make -j128
for file in test_*; do
    echo "TESTING $file"
  ./"$file"
    echo "----------------"
done
cd ..
