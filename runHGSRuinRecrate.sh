for file in instances/*
  do
    ./hgs "$file" -b $ruinRate >> experiments/HGS_RuinRecreate_FullResults_3Jobs.txt
  done