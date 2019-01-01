#include<bits/stdc++.h>
#define ll unsigned long long
#define FUSE_USE_VERSION 30

#define CALL(a,b) (a*)calloc(sizeof(a),b)
#define MALL(a,b) (a*)malloc(sizeof(a)*b)
#define REALL(a,b,c) (a*)realloc(b,sizeof(a)*c)
#define B_SIZE(x)rev (x/512) + 1

#include <fuse.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "../include/fstree.h"
#include "../include/fsoperations.h"
#include "../include/persistence.h"
// #include "../include/bitmap.h"
// #include "../include/fsdisk.h"
// static constexpr struct fuse_operations{};

using namespace std;

FSMD *root ;
FSMD* global_array[100];
unsigned long int in_num;
persistence_array pa[100];
char const *path = "~/Documents/shailesh/fuse/data";
static fuse_operations operations = {};

int first = 0;

int main( int argc, char *argv[] ){

    operations.getattr     = fs_getattr;
    operations.mknod       = fs_mknod;
    operations.mkdir       = fs_mkdir;
    operations.unlink      = fs_unlink;
    operations.rmdir       = fs_rmdir;
    operations.rename      = fs_rename;
    operations.chmod       = fs_chmod;
    operations.truncate    = fs_truncate;
    operations.open        = fs_open;
    operations.read        = fs_read;
    operations.write       = fs_write;
    operations.readdir     = fs_readdir;
    operations.access      = fs_access;


    // if(createdisk()){
    // 	serialize_metadata_wrapper(root);
    // }


    // create_tree();
    // sata_pata();
    int i;
    in_num = 0;
    for(i = 0;i<100;i++)
        global_array[i] = NULL;
    root = readFromFile((char*)path);
    cout <<"After root" <<endl;
    if(root == NULL)
    {
        //cout <<"Root is null" <<endl;
        root = create_tree();
        fflush(stdout);
    }
    cout << "After if" <<endl;
	return fuse_main( argc, argv, &operations, NULL );
}
