//Tree based MPC simulation in the framework and structure 2017/8
//Life is short£¬and I should have chosen python.

//Bug log begin:

/*bug1 begin: endless loop;IDs' output goes wrong but the input is right.
  Yubo Chen advices me to put"srand((unsigned)time(0));"in the main function, which really counts.
  Ziheng Lin: Endless loop is for "int flag = 0" in "Tree::returnID()"should be written inside(The error can not be detected by Breakpoint Debugging)
  bug1 end*/ 

/*IDs: "IDs:" not 'IDs:' in C++
  a*(threshold/R_num) remember the "()"
  "strncpy" should add '\0' by yourself, otherwise "ÍÍ¡¢îþ"*/

/*bug2 begin:sometimes "ÍÍ"?  for "()" again
  bug2 end*/

//Bug log end

#include<iostream>
#include<ctime>
#include<algorithm>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<fstream>
using namespace std;

//define the tree's capacity
const int N_max = 1000;
//IDs of parties registered in CA
int CA[N_max];
//avoid duplicate IDs
vector <int> IDs;
//The number of Child is between ChildNum1 and ChildNum2.
const int ChildNum1 = 2;
const int ChildNum2 = 5;
//the number of parties
//const int NodeSum = 20;
//all the data
//char *ALL_DATA = "abcdefghijklmnopqrst";

//TreeNode structure
struct TreeNode
{
	int ID; 
	char *data;
	TreeNode *firstchild;
	TreeNode *nextsibling;
	TreeNode *father;
};
//Tree structure
class Tree{
public:
	 Tree();
     Tree(int node_num);
	 void AddNode(TreeNode *fathernode,int threshold);
	 void Initialization(TreeNode *node);
private:
	 int Ini_ChildNum();
	 int Ini_CA_ID();
	 int returnID();
	 void AddBrotherNode(TreeNode* bro, TreeNode* node);
	 void print_tree();
private:
	 //TreeNode * root;
};

Tree::Tree(){}

//add brother nodes
void Tree::AddBrotherNode(TreeNode* bro, TreeNode* node)
{
    if(bro->nextsibling == NULL)
        bro->nextsibling = node;
    else
        AddBrotherNode(bro->nextsibling, node);
}

//Recursive spanning tree,evey subtree's root is responsible for allocating threshold(number) childnodes.
void Tree::AddNode(TreeNode *fathernode,int threshold)
{
	int R_num = Ini_ChildNum();
	if(R_num <= threshold)
	{
		//sign marks the number of nexsibling, in other words, it reflects the recursion round.
		int sign = 0;
		for(int i=0;i<R_num;i++)
		{
			//make up the firstchild node
			if(fathernode->firstchild==NULL)
			{
				//make a new TreeNode and its ID
				int tempID = returnID();
				fathernode->firstchild = new TreeNode;
				Initialization(fathernode->firstchild);
				fathernode->firstchild->ID = tempID;
				fathernode->firstchild->nextsibling = NULL;
				fathernode->firstchild->father = fathernode;
				//Allocate the data. Root of subtree is responsible for the last data and does not pass it to his childnodes.
				char *tempstr = new char[threshold-(R_num-1)*(threshold/R_num)+1];
				//strncpy: '\0' in the end!
				strncpy(tempstr,fathernode->data,threshold-(R_num-1)*(threshold/R_num));
				tempstr[threshold-(R_num-1)*(threshold/R_num)] = '\0';
				fathernode->firstchild->data = tempstr;
				cout<<"firstchild data:"<<fathernode->firstchild->data<<endl;
				IDs.push_back(tempID);
				cout<<"firstchild ID:"<<tempID<<endl;
			}
			//make up the siblingchild nodes as above
			else
			{
				TreeNode *tempnode = new TreeNode;
				Initialization(tempnode);
				int tempID = returnID();
				tempnode->ID = tempID;
				tempnode->nextsibling = NULL;
				tempnode->father = fathernode;
				char *tempstr = new char[threshold/R_num+1];
				strncpy(tempstr,fathernode->data+threshold-(R_num-1)*(threshold/R_num)+sign*(threshold/R_num),threshold/R_num);
				tempstr[threshold/R_num] = '\0';
				tempnode->data = tempstr;
				cout<<"siblingchild data:"<<tempnode->data<<endl;
				sign++;
				IDs.push_back(tempID);
				cout<<"siblingchild ID:"<<tempID<<endl;
				AddBrotherNode(fathernode->firstchild,tempnode);
			}
		}
		//Incurision of all childnodes of the fathernode
		AddNode(fathernode->firstchild,threshold-(threshold/R_num)*(R_num-1)-1);
		for(int i=1;i<R_num;i++)
		{
			int j = i;
			TreeNode *tempnode2 = fathernode->firstchild;
			while(j!=0)
			{
				j--;
				tempnode2 = tempnode2->nextsibling;
			}
			AddNode(tempnode2,(threshold/R_num)-1);
		}
	}
	//Incurision until the leaves.
	else
	{
		if(threshold != 0)
		{
			//distinguish the "sign" here between the "sign" above
			int sign = 1;
			for(int i=0;i<threshold;i++)
			{
				if(fathernode->firstchild==NULL)
				{
					int tempID = returnID();
					fathernode->firstchild = new TreeNode;
					Initialization(fathernode->firstchild);
					fathernode->firstchild->ID = tempID;
					fathernode->firstchild->nextsibling = NULL;
					fathernode->firstchild->father = fathernode;
					//char *tempstr = new char[threshold-(R_num-1)*(threshold/R_num)+1];
					//strncpy(tempstr,fathernode->data,threshold-(R_num-1)*(threshold/R_num));
					char *tempstr = new char[2];
					strncpy(tempstr,fathernode->data,1);
					tempstr[1] = '\0';
					fathernode->firstchild->data = tempstr;
					cout<<"leaves_firstchild data:"<<fathernode->firstchild->data<<endl;
					IDs.push_back(tempID);
					cout<<"leaves node ID:"<<tempID<<endl;
				}
				else
				{
					TreeNode *tempnode = new TreeNode;
					Initialization(tempnode);
					int tempID = returnID();
					tempnode->ID = tempID;
					tempnode->nextsibling = NULL;
					tempnode->father = fathernode;
					//char *tempstr = new char[threshold/R_num+1];
					//strncpy(tempstr,fathernode->data+threshold-(R_num-1)*(threshold/R_num)+sign*(threshold/R_num),threshold/R_num);
					char *tempstr = new char[2];
					strncpy(tempstr,fathernode->data+sign,1);
					tempstr[1] = '\0';
					tempnode->data = tempstr;
					cout<<"leaves_siblingchild data:"<<tempnode->data<<endl;
					sign++;
					IDs.push_back(tempID);
					cout<<"leaves node ID:"<<tempID<<endl;
					AddBrotherNode(fathernode->firstchild,tempnode);
				}
			}
		}
		cout<<"One branch of the Tree has been constructed."<<endl;
	}
}

//make ID randomly from CA
int Tree::Ini_CA_ID()
{
	int val = rand()%N_max+1;
	return val;
}

//return a never used ID 
int Tree::returnID()
{
	int tempID;
	while(1)
	{
		int flag = 0;
		tempID=Ini_CA_ID();
		for(int i=0;i<IDs.size();i++)
		{
			if(IDs[i]==tempID) flag=1;
		}
		if(flag==0) break;
	}
	return tempID;
}
//make the number of childnodes randomly
int Tree::Ini_ChildNum()
{
	int range = ChildNum2-ChildNum1+1;
	int val = rand()%range+ChildNum1;
	return val;
}

//Initialization of New TreeNode
void Tree::Initialization(TreeNode *node)
{
	node->ID = 0;
	node->firstchild = NULL;
	node->nextsibling = NULL;
	node->data = NULL;
}

int main()
{
	srand((unsigned)time(0));
	//IDs: the positive integer from 1 to N_max
	for(int i=0;i<N_max;i++)
	{
		CA[i]=i+1;
	}
	//splitting
	/*
	const char *split = ","; 
	char *p; 
	p = strtok (ALL_DATA,split); 
	*/
	char *ALL_DATA = new char[];
	//File operation
	ifstream File;
	char filename[512] = "data.txt";
	File.open(filename,ios::in);  
    if(File.fail())  
    {  
        cout<<"The file is not here."<<endl;  
        File.close();  
    }  
    else//File exists.
    { 
		int q = 0;
        while(!File.eof()) //Read the data into the array.  
        {  
            File>>ALL_DATA[q];
			q++;
        } 
		ALL_DATA[q-1] = '\0';
        File.close(); //Close the file.
	}
	int NodeSum = strlen(ALL_DATA);
	cout<<"NodeSum:"<<NodeSum<<endl;
	if(NodeSum <= N_max)
	{
		//Initialization of the root.
		TreeNode *root = new TreeNode;
		Tree T;
		root->ID = 0;
		root->firstchild = NULL;
		root->nextsibling = NULL;
		root->father = NULL;
		root->data = ALL_DATA;
		cout<<"We can draw the tree according to the information as followed according to the instruction."<<endl;
		cout<<"Instruction: Add firstchild to the tree, as well as his siblingnodexs until the next firstchild."<<endl;
		cout<<"-----------Tree based MPC-----------"<<endl;
		//the core algorithm
		T.AddNode(root,NodeSum);

		cout<< "IDs:";
		for(int i=0;i<IDs.size();i++)
		{
			cout<<IDs[i]<<' ';
		}
		cout<<endl;
		cout<<"The tree has been structed."<<endl;
		cout<<"-----------Tree based MPC-----------"<<endl;
	}
	else
	{
		cout<<"Error: NodeSum is out of range."<<endl;
	}
	system("pause");
}