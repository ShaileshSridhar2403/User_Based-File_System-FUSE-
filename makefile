COMPILER = g++ -std=c++11 -g
FILESYSTEM_FILES = src/fsmain.cpp src/fsoperations.cpp src/fstree.cpp src/bitmap.cpp src/fsdisk.cpp src/persistence.cpp

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o fsfs `pkg-config fuse --cflags --libs`
	echo 'To Mount: ./fsfs -f [mount point]'

clean:
	rm fsfs

