
for file in instances/*
  do
    ./hgs "$file" -b >> HGS_results.txt
  done