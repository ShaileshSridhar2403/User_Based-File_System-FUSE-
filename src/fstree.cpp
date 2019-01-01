#include "../include/fstree.h"
#include "../include/persistence.h"

FSMD *make_node(string path,const string &name,bool dir,FSMD *parent){
	
	cout<<"MAKE NODE CALLED\n";

	time_t at,bt,ct,mt;
	FSMD *nd = MALL(FSMD, 1);

	nd->isDir = dir;
	nd->permissions = (dir)? (S_IFDIR | 0777) : (S_IFREG | 0777);

	nd->name.assign(name);
	cout<< nd->name<<endl;
	nd->size = 0;
	//nd->data[0] = '\0';
	//unsigned long int in_num = 0;
	nd->inode_number = in_num;
	in_num = in_num + 1;
	nd->deadEnd = 0;
	nd->noc = 0;
	nd->nof = 0;
	nd->isDeleted = 0;

	nd->group_id = getgid();
	nd->user_id = getuid();

	nd->a_time = time(&at);
	nd->m_time = time(&mt);
	nd->c_time = time(&ct);
	nd->b_time = time(&bt);
	nd->children = MALL(FSMD*,10);
	for(int i = 0;i<10;i++)
	{
		nd->children[i] = NULL;
	}
	nd->parent = parent;
	cout<<"end\n";
	global_array[nd->inode_number] = nd;  //to be changed if space is occupied
	cout << "--------------nd->inode_number--------------- " <<nd->inode_number<<endl;
	//fflush(stdout);
	return nd;
}

void make_tree(){
	root = make_node("/","root",1,NULL);
}

FSMD *create_tree(){
	cout << "create tree called"<<endl;
	return make_node("/","root",1,NULL);
}

//gets the directory_name plus it changes the path value
string getDir(string &path){

	// root /xyz/abc

	string dir(path);
	string final;
	
	// root/xyz/abc/ --> root/xyz/abc
	if(dir.back() == '/'){
		if(dir.length()>1){
			dir.pop_back();
			path.pop_back();
		}else if(dir.length() == 1){
			final.assign(dir);
			return final;
		}
	}

	int len = dir.length();
	size_t found = dir.rfind("/");

	// final => abc
	// path => root/xyz/
	if(found != string::npos && found+1 != len){
	    final.assign(dir,found+1,len-found);
	    path.replace(found+1,path.length(),"");
	}

	// path => root/xyz
    if(path.length()>1 && path.back()=='/')
    	path.pop_back();
    
	return final;
}

FSMD *searcher(FSMD *node, string path){
	// _ /xyz/abc/			len = 12

	int len = path.length();

	if(!len)
		return node;
	
	// path => xyz/abc/
	if(path.front() == '/'){
		if(len > 1)
			path.erase(path.begin());
	}

	size_t found = path.find("/");
	string name;
	len = path.length();

	// path => abc/
	// name => xyz
	if(found != string::npos){

		name.assign(path,0,found);
		path.replace(0,found,"");

	}else{

		name.assign(path,0,len);
		path.replace(0,len,"");

	}
	// search xyz in the node
	for(int i=0;i < node->deadEnd;i++){
		if(node->children[i] != NULL){
			if(node->children[i]->name == name){
				return searcher(node->children[i],path);
			}
		}
	}

	return NULL;

}


void insert_node(const string &path, bool isDir){
	cout<<"CALLED INSERT NODE\n";
	string dpath(path);
	string dir_name = getDir(dpath);
	FSMD *dir_node = NULL;
	// cout<<"CALLED INSERT NODE\n";

	cout<<"path = "<<path<<"\n";
	if(root == NULL){
		make_tree();
		return ;
	}
	if(dpath == "/"){
		cout<<"here we are\n";
		if(root->children == NULL){
		//	cout << "asdsaasasddddddddddddddasdsadasdsainsrt"<<endl;
			root->children = MALL(FSMD*,root->deadEnd+1);//changed
		}else{
			// root->children = REALL(FSMD*,root->children,root->deadEnd+1);//changed
		}
		
		root->children[root->noc] = make_node(dpath,dir_name,isDir,root);//change
		root->deadEnd++;//changed
		
		root->noc++;

	}else{

		string pp(path);
		//search for the node
		dir_node = searcher(root,dpath);

		if(dir_node != NULL){

			if(dir_node->parent != NULL){
				dir_node->c_time = time(NULL);
				dir_node->m_time = time(NULL);
			}

			if(root->children == NULL){
				root->children = MALL(FSMD*,root->deadEnd+1);//change
			}else{
				// root->children = REALL(FSMD*,root->children,root->deadEnd+1);//change
			}

			// dir_node->children = REALL(FSMD*,dir_node->children,dir_node->deadEnd+1);//change
		
			dir_node->children[dir_node->noc] = make_node(dpath,dir_name,isDir,dir_node);//change
			dir_node->deadEnd++;	//change
			dir_node->noc++;
		}

	}

}

int delete_node(FSMD* r){
	// don't delete if it is a file
	if(r == NULL || !r->isDir || r->noc)
		return -1;
	cout << r->parent->deadEnd<<endl;
	// traverse the parent for the node
	for(int i=0;i < r->parent->deadEnd;i++){	//change
		if(r->parent->children[i] == r){
			r->parent->children[i]->name.clear();
			r->parent->children[i] = NULL;
			r->parent->noc--;
			global_array[r->inode_number] = NULL;
			pa[r->inode_number].init = 0;
			free(r);
			return 0;
		}
	}

	return 1;
}

int delete_file(FSMD *r){
	
	// don't delete if it is a directory
	if(r->isDir || r == NULL || r->noc)
		return 0;

	// traverse the parent for the node
	for(int i=0;i < r->parent->deadEnd;i++){	//change
		if(r->parent->children[i] == r){
			r->parent->children[i] = NULL;
			r->parent->noc--;
			r->parent->nof--;
			r->data.clear();
			global_array[r->inode_number] = NULL;
			pa[r->inode_number].init = 0;
			free(r);		
			return 0;
		}
	}

	return 0;

}

/*
void shift_node(const char *old_name_path, const char *new_name_path){
	string dpath(new_name_path);
	string spath(old_name_path);

	FSMD *src = searcher(root,spath);
	FSMD *dest = searcher(root,dpath);

	if(src == NULL || dest == NULL)return;
	if(!dest->isDir) return ;

	string src_name = getDir(spath);
	string dest_name = getDir(dpath);

	FSMD *src_parent = searcher(root,spath);
	FSMD *dest_parent = searcher(root,dpath);

	FSMD *ts = src;
	FSMD *td = dest;

	dest_parent->noc++;
	src_parent->noc--;

	dest_parent->children = REALL(FSMD*,dest_parent->children,dest_parent->noc);
	dest

}

*/




/*

void load_node(char * path, char * type, gid_t groupid, uid_t userid, time_t lc_time, time_t lm_time, time_t la_time, time_t lb_time, unsigned long int inode, off_t size, mode_t lpermissions){
    printf("LOAD_NODE CALLED\n");
    if(root == NULL){
		printf("LOADING ROOT NODE!\n");
        root = init_node("/", "root", NULL, 1);
		root->group_id = groupid;
		root->user_id = userid;
		root->c_time = lc_time;
		root->a_time = la_time;
		root->m_time = lm_time;
		root->b_time = lb_time;
		root->inode_number = inode;
		root->size = size;
    }
    else{
    	string  dpath(path);
    	string dir_name = getDir(dpath);
    	string t_dir_name.assign(dir_name);

        FSMD * dir_node = NULL;
        if(dpath.length() == 1){     

            root->noc++;
            if(root->children == NULL){
                root->children = (FSMD **)malloc(sizeof(FSMD *));
				if(strcmp(type,"directory")==0){
                	root->children[0] = init_node(path, dir, root,1);
					root->children[0]->permissions=lpermissions;
					root->children[0]->isDir = 1;
				}
				else{	root->nof++;
					root->children[0] = insert_node(path, dir, root,2);
					root->children[0]->permissions=lpermissions;
					root->children[0]->isDir = 0;
				}
				root->children[0]->group_id = groupid;
				root->children[0]->user_id = userid;
				root->children[0]->c_time = lc_time;
				root->children[0]->a_time = la_time;
				root->children[0]->m_time = lm_time;
				root->children[0]->b_time = lb_time;
				root->children[0]->inode_number = inode;
				root->children[0]->size = size;
            }
            else{
                root->children = (FSMD **)realloc(root->children, sizeof(FSMD *) * root->noc);
				if(strcmp(type,"directory")==0){
                	root->children[root->noc - 1] = init_node(path, dir, root,1);
					root->children[root->noc - 1]->permissions = lpermissions; 
					root->children[0]->isDir = 1;
				}
				else{
					root->nof++;
					root->children[root->noc - 1] = init_node(path, dir, root,2);
					root->children[root->noc - 1]->permissions= lpermissions; 
					root->fchildren = (FSMD **)realloc(root->fchildren, sizeof(FSMD *) * root->nof);
					root->fchildren[root->nof - 1] = init_file(path,tdir);
					root->children[0]->isDir = 0;
				}
				root->children[root->noc - 1]->group_id = groupid;
				root->children[root->noc - 1]->user_id = userid;
				root->children[root->noc - 1]->c_time = lc_time;
				root->children[root->noc - 1]->a_time = la_time;
				root->children[root->noc - 1]->m_time = lm_time;
				root->children[root->noc - 1]->b_time = lb_time;
				root->children[root->noc - 1]->inode_number = inode;
				root->children[root->noc - 1]->size = size;
            }


        }
        else{
            dir_node = searcher(root,dpath);
            if(dir_node != NULL){
				if(dir_node->parent!=NULL){
					dir_node->c_time=time(NULL);
					dir_node->m_time=time(NULL);
				}
                dir_node->noc++;
                dir_node->children = (FSMD **)realloc(dir_node->children, sizeof(FSMD *) * dir_node->noc);
				if(strcmp(type,"directory")==0){
                	dir_node->children[dir_node->noc - 1] = init_node(path, dir, dir_node,1);
					dir_node->children[dir_node->noc - 1] ->permissions = lpermissions; //S_IFDIR | 0755
					dir_node->children[0]->isDir = 1;
				}
				else{	dir_node->nof++;
					dir_node->children[dir_node->noc - 1] = init_node(path, dir, dir_node,2);
					dir_node->children[dir_node->noc - 1] ->permissions = lpermissions; //S_IFREG | 0644
					dir_node->fchildren = (FSMD **)realloc(dir_node->fchildren, sizeof(FSMD *) * dir_node->nof);
					dir_node->fchildren[dir_node->nof - 1] = init_file(path,dir);
					dir_node->children[dir_node->noc - 1]->isDir = 0;
				}
            }
			dir_node->children[dir_node->noc - 1]->group_id = groupid;
			dir_node->children[dir_node->noc - 1]->user_id = userid;
			dir_node->children[dir_node->noc - 1]->c_time = lc_time;
			dir_node->children[dir_node->noc - 1]->a_time = la_time;
			dir_node->children[dir_node->noc - 1]->m_time = lm_time;
			dir_node->children[dir_node->noc - 1]->b_time = lb_time;
			dir_node->children[dir_node->noc - 1]->inode_number = inode;
			dir_node->children[dir_node->noc - 1]->size = size;
        }
    }
    return;
}
*/