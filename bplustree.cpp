#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ORDER 3

struct BPlusTreeNode{
	vector< ll > key;
	vector< BPlusTreeNode* > child_ptr;
	BPlusTreeNode *parent, *buffer;
	bool isleaf;
	ll n;
};

BPlusTreeNode *root = NULL;

vector< string > split(string line)
{
	vector< string > res;
	string str = "";
	for(ll i = 0; i < line.size(); i++)
	{
		if(line[i] != ' ')
			str += line[i];
		else
		{
			res.push_back(str);
			str = "";
		}
	}
	res.push_back(str);
	return res;
}

BPlusTreeNode* initialize()
{
	BPlusTreeNode *new_node = new BPlusTreeNode;
	new_node->isleaf = false;
	new_node->n = 0;
	new_node->parent = NULL;
	new_node->key = vector< ll > (ORDER - 1, LONG_LONG_MAX);
	new_node->child_ptr = vector< BPlusTreeNode* > (ORDER, NULL);
	return new_node;
}

BPlusTreeNode* findBlock(BPlusTreeNode *root, ll num)
{
	if(root == NULL)
		return NULL;
	else if(root->isleaf)
		return root;
	else{
		ll lower_b = LONG_LONG_MIN;
		for(ll i = 0; i < ORDER-1; i++)
		{
			ll upper_b = root->key[i];
			if(root->key[i] == LONG_LONG_MAX)
			{
				root = root->child_ptr[i];
				break;
			}
			if(num >= lower_b && num < upper_b)
			{
				root = root->child_ptr[i];
				break;
			}
		}
		return findBlock(root, num);
	}
}

BPlusTreeNode* insert(BPlusTreeNode *node, ll num)
{
	if(node->key[ORDER - 2] == LONG_LONG_MAX)
	{
		if(num < node->key[0])
		{
			node->key[1] = root->key[0];
			node->key[0] = num;
			if(node->isleaf == false)
			{
				node->child_ptr[2] = node->child_ptr[1];
				node->child_ptr[1] = node->buffer;
			}

		}
		else
		{
			node->key[1] = num;
			if(node->isleaf == false)
			{
				node->child_ptr[2] = node->buffer;
			}
		}
	}
	else
	{
		BPlusTreeNode *new_node = initialize();
		new_node->parent = node->parent;
		
		if(node->isleaf == true)
		{
			vector< ll >temp = node->key;
			temp.push_back(num);
			sort(temp.begin(), temp.end());

			node->key[0] = temp[0];
			node->key[1] = temp[1];
			
			new_node->key[0] = temp[2];
			new_node->isleaf = true;
			

			if(node->parent != NULL)
			{
				new_node->parent->buffer = new_node;
				root = insert(node->parent, temp[1]);
			}
			else
			{
				root = initialize();
				root->key[0] = temp[1];
				root->child_ptr[0] = node;
				root->child_ptr[1] = new_node;
				node->parent = root;
				new_node->parent = root;
			}
			
		}
		else
		{
			vector< ll >temp = node->key;
			temp.push_back(num);
			sort(temp.begin(), temp.end());

			new_node->key[0] = temp[2];
			new_node->key[1] = LONG_LONG_MAX;
			new_node->child_ptr[0] = node->child_ptr[1];
			new_node->child_ptr[1] = node->child_ptr[2];

			node->key[0] = temp[0];
			node->key[1] = LONG_LONG_MAX;
			node->child_ptr[1] = node->buffer;
			node->child_ptr[2] = NULL;			

			if(node->parent != NULL)
			{
				new_node->parent->buffer = new_node;
				root = insert(node->parent, temp[1]);
			}
			else
			{
				root = initialize();
				root->key[0] = temp[1];
				root->child_ptr[0] = node;
				root->child_ptr[1] = new_node;
				node->parent = root;
				new_node->parent = root;
			}
		} 
	}
	return root;
}

bool find(ll num)
{

}

ll count(ll num)
{

}

void range(ll x, ll y)
{

}

void traverse(BPlusTreeNode *root)
{
	if(root == NULL)
		return;
	if(root->isleaf)
		cout<<"L:";
	for (int i = 0; i < root->key.size(); ++i)
		if(root->key[i] == LONG_LONG_MAX)
			cout<<"-"<<"|";
		else
			cout<<root->key[i]<<"|";
	cout<<"\n";
	for (int i = 0; i < root->child_ptr.size(); ++i)
		traverse(root->child_ptr[i]);
	cout << endl;
}

int main(int argc, char **argv)
{	
	ifstream fin;
	fin.open(argv[1]);
	string line;
	ll x, y;
	
	while(fin)
	{
		getline(fin, line);
		vector< string > command = split(line);
		
		cout << line <<"\n";

		if(command[0] == "INSERT"){
			x = stoll(command[1]);
			BPlusTreeNode *node = findBlock(root, x);
			if(node == NULL)
			{
				root = initialize();
				root->key[0] = x;
				root->isleaf = true;
			}
			else{
				cout << node->key[0];
				root = insert(node, x);
			}
		}
		else if(command[0] == "FIND"){
			x = stoll(command[1]);
			cout << find(x);
		}
		else if(command[0] == "COUNT"){
			x = stoll(command[1]);
			cout << count(x);
		}
		else if(command[0] == "RANGE"){
			x = stoll(command[1]);
			y = stoll(command[2]);
			range(x, y);
		}
		else if(command[0] != ""){
			cout << "Invalid Command\n";
			exit(0);
		}

	}
	traverse(root);
	return 0;
}