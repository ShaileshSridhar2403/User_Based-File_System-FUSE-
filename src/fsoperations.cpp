#include "../include/fsoperations.h"
#include "../include/persistence.h"

int fs_getattr( const char *path, struct stat *st ){
	
	string dpath(path);
	string dir_str;
	
	FSMD *dir_node = NULL;
	dir_str = getDir(dpath);
	
	cout<<"CALLED GETATTR\n";
	// cout<<"size of datamap --> "<<datamap_size<<"\n";
	cout<<dir_str<<"\n";

	if(dir_str == "/"){
		dir_node = root;
	}else if(dir_str.length()){
		dir_node = searcher(root,path);
	}
	//if not found, return -ENOENT, to take actions, allows mkdir
	if(dir_node == NULL){
		return -ENOENT;
	}
	cout << "not enoent" <<endl;
	if ( dir_node->isDir ){
		st->st_mode = dir_node->permissions;
		st->st_nlink = 2;
	}else{
		st->st_nlink = 1;
		st->st_mode = dir_node->permissions;
		if(dir_node->size > 0){

			st->st_size = dir_node->data.size();
			st->st_blocks = (((st->st_size) / 512) + 1);

		}else{
			st->st_size = 0;
			st->st_blocks = 0;
		}
		
	}

	st->st_nlink += dir_node->noc;
	st->st_uid = dir_node->user_id; 
	st->st_gid = dir_node->group_id;

	st->st_atime = dir_node->a_time;
	st->st_mtime = dir_node->m_time;
	st->st_ctime = dir_node->c_time;
	
	return 0;
}

int fs_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ){
	
	cout<<"READDIR CALLED\n";

	filler( buffer, ".", NULL, 0 );
	filler( buffer, "..", NULL, 0 );
	
	string dpath(path);

	FSMD *dir_node = NULL;

	if(dpath == "/"){
		dir_node = root;
	}else{
		dir_node = searcher(root,dpath);
	}

	if(dir_node == NULL)
		return -ENOENT;
		
	for(int i=0;i < dir_node->deadEnd;i++){	//change
		if(dir_node->children[i] != NULL){
			filler(buffer,(dir_node->children[i]->name).c_str(),NULL,0);
		}
	}
	writeToFile((char*)path,root);
	cout << "written to file" <<endl;
	
	return 0;
}



int fs_access(const char * path, int mask){
	printf("ACCESS CALLED\n");	
	string dpath(path);
	string dir_name = getDir(dpath);

	uid_t u = getuid();
	gid_t g = getgid();
	int per_flag=0;

	FSMD *dir_node = NULL;
	if(strlen(path) == 1){
        if(root == NULL){
        	make_tree();
        }
        dir_node = root;
        cout<<"here\n";
	}else{
		dir_node = searcher(root,path);
	}
	if(mask == 0){
		mask = 1;
	}
	mode_t p = dir_node->permissions;
	switch(mask){
		case 1:{
				if(u == dir_node->user_id){
					p = p & S_IXUSR;
					if(p == 0100)
						per_flag = 1;
				}
				else if(g == dir_node->group_id){
					p = p & S_IXGRP;
					if(p == 010)
						per_flag=1;
				}
				else{
					p = p & S_IXOTH;
					if(p == 01)
						per_flag = 1;
				}
				break;
			}
		case 4:{
				if(u == dir_node->user_id){
					p = p & S_IRUSR;
					if(p == 0400)
						per_flag = 1;
				}
				else if(g == dir_node->group_id){
					p = p & S_IRGRP;
					if(p == 040)
						per_flag = 1;
				}
				else{
					p = p & S_IROTH;
					if(p == 04)
						per_flag = 1;
				}
				break;
			}
		case 2:{
				if(u == dir_node->user_id){
					p = p & S_IWUSR;
					if(p == 0200)
						per_flag = 1;
				}
				else if(g == dir_node->group_id){
					p = p & S_IWGRP;
					if(p == 020)
						per_flag = 1;
				}
				else{
					p = p & S_IWOTH;
					if(p == 02)
						per_flag = 1;
				}
				break;			
			}
		}
		if(per_flag == 1)
			return 0;

		return -EACCES;
}


int fs_read(const char *path, char *buffer, size_t size, off_t offset,struct fuse_file_info *fi){
	cout<<"CALLED READ\t\t";

	if(fs_access(path,R_OK) != 0)
		return -EACCES;
	
	string dpath(path);
	
	FSMD *dir_node = NULL;
	
	string select;
	string dir_name = getDir(dpath);

	dir_node = searcher(root,path);

	if(dir_node != NULL){

		if(!dir_node->isDir){
			dir_node->a_time = time(NULL);
			int len = dir_node->data.length();
			
			if(!len)
				return 0;

			cout<<"BUFFER\t\t"<<buffer<<"\n\n";
			memcpy(buffer, (dir_node->data).c_str() + offset, size);  //not enough space
			// memcpy(buffer, (dir_node->data).c_str() , size);  //not enough space
			
			return size;
		}

	}
	return -ENOENT;
}

int fs_chmod(const char *path,mode_t newp){
	FSMD *dir_node = NULL;
	string dpath(path);
	dir_node = searcher(root,dpath);
	if(dir_node != NULL){
		dir_node->c_time = time(NULL);
		dir_node->permissions = newp;
		return 0;
	}
	return -ENOENT;
}

int fs_mkdir(const char *path,mode_t mode){
	
	cout<<"CALLED MKDIR\t\t\n";
	const string dpath(path);
	cout<<path<<"\n\n";
	insert_node(dpath,1);
	return 0;
}

int fs_mknod(const char *path,mode_t mode,dev_t dev){
	cout<<"CALLED MKNOD\t\t\n";
	const string dpath(path);
	insert_node(dpath,0);
	return 0;
}


int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){

	cout<<"WRITE CALLED here for path :: "<<path<<"\n";
	if(fs_access(path,R_OK)!=0)
		return -EACCES;

	string dpath(path);
	FSMD *dir_node = NULL;

	dir_node = searcher(root,dpath);

	if(dir_node != NULL){
		if(!dir_node->isDir){

			dir_node->a_time = time( NULL );
			dir_node->m_time = time(NULL);
			dir_node->c_time = time(NULL);

			dir_node->size = size+offset;
			
			dir_node->data.resize(offset+size);

			for(int i=0;i<size;i++)
				dir_node->data[offset+i] = buf[i];

			if(offset <= 0){
				dir_node->data.replace((int)strlen(buf),(int)dir_node->data.length(),"");
			}

			memset((char *)buf, 0, strlen(buf));
			return size;
		}

	}
	
	return -ENOENT;
}

int fs_truncate(const char *path, off_t size){
	string dpath(path);
	FSMD *dir_node = searcher(root,dpath);
	string dir_name = getDir(dpath);
	FSMD *parent_dir = searcher(root,dpath);
	// cout<<"here*********************************************************|\n";
	if(dir_node != NULL){

		if(size <= 0){
			dir_node->data.clear();
			dir_node->size = 0;

		}else{

			char *buffer = MALL(char,size+1);
			dir_node->data.copy(buffer,0,size);
			dir_node->data.clear();
			dir_node->data.assign(buffer);
			dir_node->size = size;

		}
		return 0;
	}

	return -ENOENT;

}

int fs_open(const char *path, struct fuse_file_info *fi){
	
	FSMD *dir_node = NULL;
	dir_node = searcher(root,path);

	return 0;
}


int fs_rmdir(const char * path){
	printf("GETATTR CALLED\n");
	
	string dpath(path);
	
	if(dpath == "/"){
		cout<<"Cannot delete ROOT\n";
		return -ENOTEMPTY;
	}
	
	FSMD * dir_node = searcher(root,path);
	int ret = delete_node(dir_node);
	if(ret < 0)
		return -ENOTEMPTY;

	return 0;
}


int fs_unlink(const char *path){
	string dpath(path);
	FSMD *dir_node = searcher(root,path);
	if(dir_node == NULL || dir_node->isDir)
		return 0;
	delete_file(dir_node);

	return 0;
}

int fs_rename (const char *old_name_path, const char *new_name_path){

    printf("old name=%s\n",old_name_path);
    printf("new name=%s\n",new_name_path);
    
    FSMD *nd = searcher(root,old_name_path);
    string dpath(new_name_path);
    string new_name = getDir(dpath);

    if(nd != NULL){
        nd->name.assign(new_name);
        printf("Done\n");
        return 0;
    }
    printf("File not found\n");
    return -ENOENT;
}


// int fs_rename (const char *old_name_path, const char *new_name_path){

// 	FSMD *src = NULL,*dest = NULL;
// 	src = searcher(root,old_name_path);
// 	if(src == NULL)
// 		return -ENOENT;
	
// 	dest = searcher(root,new_name_path);
// 	if(dest == NULL)
// 		return -ENOENT;

// 	if(dest != NULL){
// 		if(src->isDir && !dest->isDir){
// 			return -EPERM;
// 		}
// 	}
// 	shift_node(old_name_path,new_name_path);

// 	return 0;
// }