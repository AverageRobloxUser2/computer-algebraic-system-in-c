cd build
make -j128
for file in test_*; do
    echo ""
    echo "TESTING $file"
  ./"$file"
done
cd ..
