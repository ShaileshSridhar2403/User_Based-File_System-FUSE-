#include<bits/stdc++.h>
// #include "tree.h"
// #include<string.h>
#include "../include/fstree.h"
// #include "../include/persistence.h"
// FSMD *root;


void copyTodataStruct(FSMD* nd,dataStruct *ds){
	cout <<"In copy to data struct" <<endl;
	//dataStruct* ds = (dataStruct*)malloc(sizeof(dataStruct));
	
	ds->size = nd->size;//nd->size;
	//cout << nd->offset<<endl;
	ds->offset = nd->offset;
	// cout << "hello" <<endl;

	strcpy(ds->name,(nd->name).c_str());
	if(!nd->isDir)strcpy(ds->data,(nd->data).c_str());
	
	ds->isDir = nd->isDir;
	ds->isDeleted = nd->isDeleted;
	ds->permissions = nd->permissions;
	ds->user_id = nd->user_id;
	ds->group_id = nd->group_id;

	ds->noc = nd->noc;
	ds->nof = nd->nof;
	ds->deadEnd = nd->deadEnd;

	ds->a_time = nd->a_time;  
    ds->m_time = nd->b_time;
    ds->c_time = nd->b_time;
    ds->b_time = nd->b_time;	

    //return ds;
}

FSMD* copyToNode(dataStruct ds){
	
string name(ds.name);
	FSMD* nd = make_node(name,name,ds.isDir,root);   //to be checked by satyam

	cout <<"-------------------------------------Copied-------------------"<<endl;

	cout<<"NAME : "<<name<<"\t"<<"number of children : "<<ds.noc<<"\n";

	if(nd != NULL)
		printf("YESSSSS");

	nd->name.assign(ds.name);
	if(!ds.isDir)
		nd->data.assign(ds.data); 
	nd->size = ds.size;
	nd->offset = ds.offset;

	nd->isDir = ds.isDir;
	nd->isDeleted = ds.isDeleted;
	nd->permissions = ds.permissions;
	nd->user_id = ds.user_id;
	nd->group_id = ds.group_id;

	nd->noc = ds.noc;
	nd->nof = ds.nof;
	nd->deadEnd = ds.deadEnd; //Please dont change it.(contact Shailesh and bongale)
	cout <<"DEAD ----------------------------END-----------------------"<<endl;
	cout <<nd->deadEnd <<endl;
	nd->a_time = ds.a_time;  
    nd->m_time = ds.b_time;
    nd->c_time = ds.b_time;
    nd->b_time = ds.b_time;

return nd;

}





/*int writeOneToFile(FILE* fp,FSMD* nd){
	// FILE * fp = fopen("data","wb");
	// dataStruct* ds = (dataStruct*)malloc(sizeof(dataStruct));
	dataStruct* ds = copyTodataStruct(nd);
	cout << "please work............" << endl;
	// fflush(stdout);
	if (fp != NULL){
		
		cout<<"NAME\n"<< root->name;
		fwrite(ds, sizeof(struct dataStruct),1,fp);
		
		return 1;
	}
	return 0;

}*/
int writeToFile(char* filepath,FSMD* root){
	cout<<"--------------------------------------WRITE TO FILE----------------------------"<<endl;
	fflush(stdout);
	//persistence_array pa[100];
	for(int i = 0;i <100;i++)
	{
		pa[i].init = 0;
	}
	FILE * fp = fopen("../data","wb");
	if(fp != NULL){
				cout << "-----------------inside fp write---------------------------"<<endl;

		/*writeOneToFile(fp,root);
		cout<<"--------------------------------------WRITE TO FILE2----------------------------"<<endl;
		for(int i=0;i < root->deadEnd;i++){
			if(root->children[i] != NULL)
				writeOneToFile(fp,root->children[i]);
		}*/
		for(int i = 0;i < 100;i++)
		{
			if(global_array[i] != NULL)
			{	//persistence_array pa[100];
				cout << "i in write"<<i <<endl;
				cout << "Inside assigning bock"<<endl;
				// dataStruct *ds = (dataStruct *)malloc(sizeof(dataStruct));
				 copyTodataStruct(global_array[i],&(pa[i].data_struct));
				 pa[i].init = 1;
				 cout <<"-------------------------------------Copied-------------------"<<endl;
				 // pa[i].parent = global_array[i]->parent->inode_number;
				if(global_array[i]->isDir)
				{
					for(int j = 0;j<10;j++)
					{
						if(global_array[i]->children[j] != NULL)
						{
							pa[i].children[j] = global_array[i]->children[j]->inode_number;//need to deal with many deletions
						}
						else
						{
							pa[i].children[j] =  -1;
						}

					}
				}
						

		}
	}
		fwrite(pa, sizeof(persistence_array)*100,1,fp);
		fclose(fp);
		return 1;
}
return 0;
}

/*FSMD* readOneFromFile(FILE* fp){
	dataStruct* ds = (dataStruct*)malloc(sizeof(dataStruct));
	// FILE * fp = fopen("data","rb");
	if(fp != NULL){
		
		int check = fread(ds,sizeof(dataStruct),1,fp);
		printf("No of items:%d  ",check);
		return copyToNode(ds);

	}
	return NULL;
}*/

FSMD* readFromFile(char* filepath){
	cout<<"--------------------------------------READ FROM FILE----------------------------";
	FILE * fp = fopen("../data","rb");
	if (fp != NULL){
		cout << "-----------------inside fp read---------------------------"<<endl;
		/*root = readOneFromFile(fp);
		cout<<"Number of children"<<root->noc<<endl;
		for(int i=0;i < root->noc;i++){
			root->children[i] = readOneFromFile(fp);
			root->children[i]->parent = root;
			cout <<"child"<<root->children[i]->name<<endl;
		}*/
		//persistence_array pa[100];
		fread(pa,sizeof(persistence_array)*100,1,fp);
		// root = copyToNode((pa[0])->data_struct);
		// root->parent = NULL;
		for(int i = 0;i < 100;i++)
		{
			if(pa[i].init == 1)
			{
				cout <<"---------------i in read from--------------::" << i <<endl;
				global_array[i] = copyToNode(pa[i].data_struct);
				cout << "return ofcopynode" <<endl;
				/*if(global_array[i]->isDir && global_array[i]->noc != 0)
				{
					cout << "---------------inside is dir-------------------" <<endl;
					for(int j=0;j<10;j++)
					{
						if(pa[i].children[j] != -1)
						{
							cout<<"Copy node called" <<endl;
							global_array[i]->children[j] = copyToNode(pa[pa[i].children[j]].data_struct);
						}
					}
				}*/
			}

		}
		global_array[0]->parent = NULL;
		for(int i = 0;i<100;i++)
		{
			if(pa[i].data_struct.isDir)
			{
				for(int j = 0;j<10;j++)
				{
					if(pa[i].children[j] != -1)
					{
						global_array[i]->children[j] = global_array[pa[i].children[j]];
						global_array[i]->children[j]->parent = global_array[i];
					}
					else
					{
						global_array[i]->children[j] = NULL;
					}

				}
			}
		}
		fclose(fp);
		root = global_array[0];	
		return root;
	}
	return NULL;
}
