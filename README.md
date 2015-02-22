# bgzeof
Detect truncation of BGZF files by checking the EOF marker exists

	gcc -g -Wall -O2 -o bgzeof bgzeof.c -I../htslib -L../htslib -lhts -lz
