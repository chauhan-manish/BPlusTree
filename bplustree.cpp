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
ofstream fout;


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
		for(ll i = 0; i < ORDER; i++)
		{			
			ll upper_b = root->key[i];
			if(i == 2)
				upper_b = LONG_LONG_MAX;
			//cout<<lower_b<<" "<<upper_b<<"\n";
			//getchar();
			if(root->key[i] == LONG_LONG_MAX)
			{
				root = root->child_ptr[i];
				break;
			}
			if(num > lower_b && num <= upper_b)
			{
				root = root->child_ptr[i];
				break;
			}
			lower_b = root->key[i];
		}
		return findBlock(root, num);
	}
}

BPlusTreeNode* insert(BPlusTreeNode *node, ll num)
{
	if(node->key[ORDER - 2] == LONG_LONG_MAX)
	{
		if(num <= node->key[0])
		{
			node->key[1] = node->key[0];
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
			
			new_node->child_ptr[2] = node->child_ptr[2];
			node->child_ptr[2] = new_node;

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
			vector< BPlusTreeNode* >temp_child_ptr = node->child_ptr;
			temp.push_back(num);
			sort(temp.begin(), temp.end());

			ll index = upper_bound(temp.begin(), temp.end(), num) - temp.begin();
			temp_child_ptr.insert(temp_child_ptr.begin() + index, node->buffer);
			

			new_node->key[0] = temp[2];
			new_node->key[1] = LONG_LONG_MAX;
			new_node->child_ptr[0] = temp_child_ptr[2];
			new_node->child_ptr[1] = temp_child_ptr[3];
			new_node->child_ptr[2] = NULL;

			new_node->child_ptr[0]->parent = new_node;
			new_node->child_ptr[1]->parent = new_node;

			node->key[0] = temp[0];
			node->key[1] = LONG_LONG_MAX;
			node->child_ptr[0] = temp_child_ptr[0];
			node->child_ptr[1] = temp_child_ptr[1];
			node->child_ptr[2] = NULL;
			
			node->child_ptr[0]->parent = node;
			node->child_ptr[1]->parent = node;

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

bool find(BPlusTreeNode *node, ll num)
{
	for(ll i = 0; i < ORDER - 1; i++)
	{
		if(node->key[i] == num)
			return true;
	}
	return false;
}

ll count(BPlusTreeNode *node, ll num)
{
	ll c = 0;
	if(node == NULL)
		return 0;
	for(ll i = 0; i < ORDER - 1; i++)
	{
		if(node->key[i] == num)
			c++;
		if(node->key[i] > num)
			break;
		if(node->key[i] == LONG_LONG_MAX)
			c = c + count(node->child_ptr[2], num);
	}
	c = c + count(node->child_ptr[2], num);
	return c;
}

void range(BPlusTreeNode *node, ll x, ll y)
{
	if(node == NULL)
		return;
	for(ll i = 0; i < ORDER - 1; i++)
	{
		if(node->key[i] >= x && node->key[i] <= y){
			//cout << node->key[i] << "\n";
			fout << node->key[i] << "\n";
		}
		if(node->key[i] > y)
			break;
		if(node->key[i] == LONG_LONG_MAX)
			range(node->child_ptr[2], x, y);
	}
	range(node->child_ptr[2], x, y);
	return;
}

void traverse(BPlusTreeNode *root)
{
	if(root == NULL)
		return;
	queue<BPlusTreeNode *>q;
	q.push(root);
	while(!q.empty())
	{
		BPlusTreeNode *node = q.front();
		q.pop();
		if(node->isleaf)
			cout<<"L:";
		for (int i = 0; i < node->key.size(); ++i)
			if(node->key[i] == LONG_LONG_MAX)
				cout<<"-"<<"|";
			else
				cout<<node->key[i]<<"|";
		cout<<"\n";

		for (int i = 0; i < node->child_ptr.size(); ++i)
			if(!node->isleaf && node->child_ptr[i] != NULL)
				q.push(node->child_ptr[i]);
	}

	cout << endl;
}

int main(int argc, char **argv)
{	
	ifstream fin;
	fin.open(argv[1]);
	string line;
	ll x, y;
	fout.open("out.txt");
	while(fin)
	{
		getline(fin, line);
		vector< string > command = split(line);
		
		//cout << line <<"\n";

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
				//getchar();
				//cout << node->key[0] <<"\n";
				root = insert(node, x);
			}
		}
		else if(command[0] == "FIND"){
			x = stoll(command[1]);
			BPlusTreeNode *node = findBlock(root, x);
			if(node == NULL)
				cout << "NO\n";
			else
			{
				if(find(node, x))
				{
					//cout << "YES\n";
					fout << "YES\n";
				}
				else
				{
					//cout << "NO\n";
					fout << "NO\n";
				}
			}
		}
		else if(command[0] == "COUNT"){
			x = stoll(command[1]);
			BPlusTreeNode *node = findBlock(root, x);
			if(node == NULL)
				cout << "0\n";
			else
			{
				//cout << count(node, x) << "\n";
				fout << count(node, x) << "\n";
			}
		}
		else if(command[0] == "RANGE"){
			x = stoll(command[1]);
			y = stoll(command[2]);
			BPlusTreeNode *node = findBlock(root, x);
			if(node == NULL)
				cout << "0\n";
			else
			{	range(node, x, y);
				//cout << "\n";
			}
		}
		else if(command[0] != ""){
			cout << "Invalid Command\n";
			exit(0);
		}

		//traverse(root);
	}
	//traverse(root);
	return 0;
}