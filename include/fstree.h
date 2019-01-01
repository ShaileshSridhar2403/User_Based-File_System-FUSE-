#ifndef FSTREE
#define FSTREE

#include<bits/stdc++.h>
#include <fuse.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "fsdisk.h"

#define ll unsigned long long

#define CALL(a,b) (a*)calloc(sizeof(a),b)
#define MALL(a,b) (a*)malloc(sizeof(a)*b)
#define REALL(a,b,c) (a*)realloc(b,sizeof(a)*c)
#define B_SIZE(x)rev (x/512) + 1

using namespace std;

struct sss_metadata{
	string name;
	string data;

	off_t size;
	long int offset;
	
	int isDir;
	int isDeleted;
	mode_t permissions;
	uid_t user_id;
	gid_t group_id;

	int noc;
	int nof;
	int  deadEnd;

	time_t a_time;                  // Access time
    time_t m_time;                  // Modified time
    time_t c_time;                  // Status change time
    time_t b_time;                  // Creation time
    
    unsigned long int inode_number; // Inode number of the node in disk    
    struct sss_metadata * parent;         // Pointer to parent node
    
    // set<struct sss_metadata*> chills;

    struct sss_metadata ** children;      // Pointers to children nodes
};

typedef struct dataStruct{
	char name[100];    
    char data[1000];
    // char path[1000];

    off_t size;
	long int offset;
	
	bool isDir;
	bool isDeleted;
	mode_t permissions;
	uid_t user_id;
	gid_t group_id;

	int noc;
	int nof;
	ll deadEnd;

	time_t a_time;                  // Access time
    time_t m_time;                  // Modified time
    time_t c_time;                  // Status change time
    time_t b_time;                  // Creation time
    

}dataStruct;

typedef struct persistence_array
{
	dataStruct data_struct;
	int children[10];
	//int parent;
	int init;
}persistence_array;

typedef struct sss_metadata FSMD;
// typedef struct dataStruct dataStruct;
extern FSMD* global_array[100];
extern unsigned long int in_num ;
extern persistence_array pa[100];
extern FSMD * root;

// time_t TIMED_;

// extern MD * root;

// TREE OPERATIONS / METHODS
void make_tree();
FSMD *make_node(string path,const string&,bool dir,FSMD *);
FSMD *make_dir_node(string name);
FSMD *make_file_node(string name);

FSMD *searcher(FSMD*,string);

void insert_node(const string&,bool isDir);
void insert_file(const string&);
string getDir(string &s);
FSMD *create_tree();
void temp_files(FSMD **);

int delete_file(FSMD *);
int delete_node(FSMD *);
void load_node(char * path, char * type, 
	gid_t groupid, uid_t userid, 
	time_t lc_time, time_t lm_time, 
	time_t la_time, time_t lb_time, 
	unsigned long int inode, off_t size, mode_t lpermissions);

// void shift_node(const char *old_name_path, const char *new_name_path);

#endif
