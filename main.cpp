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

/*bug3 begin: Here must be "return NULL, the default is not NULL."
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
const int ChildNum2 = 4;
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
	 void Addition(TreeNode *root,int ID1,int ID2);
private:
	 int Ini_ChildNum();
	 int Ini_CA_ID();
	 int returnID();
	 TreeNode* search_node(TreeNode *root,int ID);
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

//Find a node according to its ID and the calculation would not be included in the reality for the code is not for multiprocess.
TreeNode* Tree::search_node(TreeNode *root,int ID)
{
	TreeNode *tempnode = new TreeNode;
	tempnode = root;
	while(1)
	{
		if(tempnode->ID == ID)
		{
			return tempnode;
		}
		else
		{	
			if(tempnode->nextsibling != NULL)
			{
				tempnode = tempnode->nextsibling;
			}
			else
			{
				//cout<<"Error: ID can not be found1."<<endl;
				break;
			}
		}
	}
	tempnode = root;
	while(1)
	{
		if(tempnode->firstchild != NULL)
		{
			if(search_node(tempnode->firstchild,ID) != NULL)
			{
				return search_node(tempnode->firstchild,ID);
			}
		}

		if(tempnode->nextsibling != NULL)
		{
			tempnode = tempnode->nextsibling;
		}
		else
		{
			//cout<<"Error: ID can not be found2."<<endl;
			//***Here must be "return NULL, the default is not NULL."
			return NULL;
		}
	}
}

//additon process
void Tree::Addition(TreeNode *root,int ID1,int ID2)
{
	if(ID1 != ID2)
	{
		int flag = 0;
		for(int i=0;i<IDs.size();i++)
		{
			if(IDs[i]==ID1 || IDs[i]==ID2)
				flag++;
		}
		if(flag != 2)
		{
			//cout<<"Error: At least one of the IDs is not in the array IDs."<<endl;
			return ;
		}
		else
		{
			int *fathers1 = new int[];
			int *fathers2 = new int[];
			TreeNode *tempnode1 = new TreeNode;
			TreeNode *tempnode2 = new TreeNode;
			tempnode1 = search_node(root->firstchild,ID1);
			tempnode2 = search_node(root->firstchild,ID2);
			int k1 = 0;
			int k2 = 0;
			cout<<"Node ID in ID1's branch: ";
			while(1)
			{
				fathers1[k1] = tempnode1->ID;
				cout<<fathers1[k1]<<" ";
				if(tempnode1->father != NULL)
				{
					tempnode1 = tempnode1->father;
				}
				else{break;}
				k1++;
			}
			cout<<endl;
			cout<<"Node ID in ID2's branch: ";
			while(1)
			{
				fathers2[k2] = tempnode2->ID;
				cout<<fathers2[k2]<<" ";
				if(tempnode2->father != NULL)
				{
					tempnode2 = tempnode2->father;
				}
				else{break;}
				k2++;
			}
			cout<<endl;

			//find the cousin upper nodes
			int flag = 0;
			for(int m1=0;m1<sizeof(fathers1);m1++)
			{
				if(flag==0)
				{
					for(int m2=0;m2<sizeof(fathers2);m2++)
					{
						if(fathers1[m1]==fathers2[m2])
						{
							if(m1!=0 && m2!=0)
							{
								cout<<"ID1's branch "<<fathers1[m1-1]<<" and ID2's branch "<<fathers2[m2-1]<<" are the cousins."<<endl;
								cout<<"They calculate the addition for "<<ID1<<" and "<<ID2<<endl;
								flag = 1;
							}
							else
							{
								cout<<"ID1 and ID2 have already been in the same branch."<<endl;
							}
							break;
						}
					}
				}
				else{break;}
			}
		}
	}
	else
	{
		cout<<"Error: The same ID is not allowed."<<endl;
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
		cout<<"We can draw the tree according to the information as followed."<<endl;
		cout<<"Instruction: Add firstchild to the tree, as well as his siblingnodexs until the next firstchild."<<endl;
		cout<<"-----------Tree based MPC's construction-----------"<<endl;
		//the core algorithm
		T.AddNode(root,NodeSum);

		cout<< "IDs:";
		for(int i=0;i<IDs.size();i++)
		{
			cout<<IDs[i]<<' ';
		}
		cout<<endl;
		cout<<"The tree has been structed."<<endl;
		cout<<"-----------Tree based MPC's construction-----------"<<endl<<endl;

		//additon process
		cout<<"-----------Tree based MPC's addition-----------"<<endl;
		cout<<"Input 0 0 to exit."<<endl;
		while(1)
		{
			cout<<"Input two IDs above to calculate the addition."<<endl;
			int ID1,ID2;
			cin>>ID1>>ID2;
			if(ID1 == 0 && ID2 == 0)
			{
				cout<<"Successful exit."<<endl;
				break;
			}
			else
			{
				T.Addition(root,ID1,ID2);
				cout<<"-----------Tree based MPC's addition-----------"<<endl<<endl;
			}
		}
	}
	else
	{
		cout<<"Error: NodeSum is out of range."<<endl;
	}
	system("pause");
}