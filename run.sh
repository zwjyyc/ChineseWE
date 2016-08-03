make
./cwe -train ../data/sample.txt -output-word vec.txt -output-char chr.txt -size 50 -window 5 -sample 1e-4 -negative 5 -hs 0 -iter 3 -ada 1 -threads 10
