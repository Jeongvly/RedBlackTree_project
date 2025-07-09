#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

// Color enum : BLACK(0), RED(1)
enum Color
{
	BLACK = 0,
	RED = 1,
};

// ������û ������ ��� ����ü
struct Register
{
	int sid;				// �й�
	string subject;			// ��û����
	string sname;			// �̸�
	int semester;			// �̼��б��
	string phone;			// ��ȭ��ȣ
	int timestamp;			// ������û �ð�
};


// �����Ʈ���� ��� Ŭ����
class Node
{
public:
	Node* parent;				// �θ� ���
	Node* left_child;			// ���� �ڽ� ���
	Node* right_child;			// ������ �ڽ� ���	
	Color color;				// ��� ����
	Register r;					// ��忡 ����� ��� ����
	int depth;					// ����� ���� 

	// ������û ������ �ʱ�ȭ 
	Node(Register r)
		:parent{ nullptr }, left_child{ nullptr }, right_child{ nullptr }, color{ RED }, r{ r }, depth{0}
	{
	}

	// �⺻ �ʱ�ȭ 
	Node()
		:parent{ nullptr }, left_child{ nullptr }, right_child{ nullptr }, color{ BLACK }, r{}, depth{0}
	{
	}

	// ���� �ڽ� ����
	void insertLeftChild(Node* left)
	{
		left_child = left;
		left->parent = this;
	}

	// ������ �ڽ� ����
	void insertRightChild(Node* right)
	{
		right_child = right;
		right->parent = this;
	}

};

//Timestamp ������ ��� ��
struct compTimestamp {
	bool operator()(Node* a, Node* b) const
	{
		return a->r.timestamp < b->r.timestamp; 
	}
};

// ���� �̸� ���������� ��� ��
struct compSubject {
	bool operator()(Node* a, Node* b) const
	{
		return a->r.subject < b->r.subject;
	}
};


// �����Ʈ�� Ŭ����
class RedBlackTree
{
public:
	Node *root;
	unordered_map<string, set<Node*, compTimestamp>> mapSubjectOrderedTimestamp;		// ���� timestamp ������ ���ĵ� ��� ����
	unordered_map<int, set<Node*, compSubject>> mapSidOrderedSubject;					// �л��� ���� ���� ������ ���ĵ� ��� ���� 

	RedBlackTree()
		:root{nullptr}
	{ }

	~RedBlackTree()
	{
		removeAll(root);
	}

	void removeAll(Node* input)
	{
		if (input == nullptr)
			return;
		removeAll(input->left_child);
		removeAll(input->right_child);
		delete input;
	}

	// ���� �� ����� Ư�� ����� ���� ���ߴ� �Լ�
	void redBlackBalance(Node* input)
	{
		// ��Ʈ ���� BLACK
		if (input == root)
			input->color = BLACK;
		else if (input->parent->color == RED)
		{
			Node* parent = input->parent;
			Node* child = input;
			
			// �θ��� ���� ��� 
			Node* sibling = (parent->parent->left_child == parent ? parent->parent->right_child : parent->parent->left_child);

			// �θ��� ���� ��尡 RED�� recoloring, BLACK�̸� restructuring
			if (sibling->color == RED)
				recoloring(child);
			else
				restructuring(child);
		}
	}

	// �ش� ����� ���� ���� �Լ�
	void downDepth(Node* input)
	{
		if (input != nullptr)
		{
			input->depth -= 1;
			downDepth(input->left_child);
			downDepth(input->right_child);
		}
	}

	// �ش� ����� ���� ���� �Լ�
	void upDepth(Node* input)
	{
		if (input != nullptr)
		{
			input->depth += 1;
			upDepth(input->left_child);
			upDepth(input->right_child);
		}
	}

	// ������ ȸ�� �Լ�
	void rotationRight(Node* input)
	{
		Node* parent = input->parent;
		Node* child = input;
		// ���̰� ���� �� ����
		swap(parent->depth, child->depth);
		upDepth(parent->right_child);
		downDepth(child->left_child);

		// �θ� root�� ���
		if (parent == root)
		{
			child->parent = nullptr;
			root = child;
		}

		// �θ� ���� �ڽ��� ���
		else if (parent->parent->left_child == parent)
		{

			parent->parent->left_child = child;
			child->parent = parent->parent;
		}

		// �θ� ������ �ڽ��� ���
		else
		{
			parent->parent->right_child = child;
			child->parent = parent->parent;
		}

		// �ڽ�, �θ� ������Ʈ
		parent->parent = child;
		parent->left_child = child->right_child;
		child->right_child->parent = parent;
		child->right_child = parent;
	}

	// ���� ȸ�� �Լ�
	void rotationLeft(Node* input)
	{
		Node* parent = input->parent;
		Node* child = input;

		// ���̰� ���� �� ����
		swap(parent->depth, child->depth);
		upDepth(parent->left_child);
		downDepth(child->right_child);

		// �θ� root�� ���
		if (parent == root)
		{
			child->parent = nullptr;
			root = child;
		}

		// �θ� ���� �ڽ��� ���
		else if (parent->parent->left_child == parent)
		{

			parent->parent->left_child = child;
			child->parent = parent->parent;
		}

		// �θ� ������ �ڽ��� ���
		else
		{
			parent->parent->right_child = child;
			child->parent = parent->parent;
		}

		// �ڽ�, �θ� ������Ʈ
		parent->parent = child;
		parent->right_child = child->left_child;
		child->left_child->parent = parent;
		child->left_child = parent;
	}

	// Case 1: ���� ��尡 BLACK
	// 4���� ���� (LL, LR, RL, RR)�� ����� rotation�� ���� ���� 
	void restructuring(Node* input)
	{
		Node* grandParent = input->parent->parent;
		Node* parent = input->parent;
		Node* child = input;


		if (grandParent->left_child == parent)
		{
			if (parent->left_child == child)
			{
				grandParent->color = RED;
				parent->color = BLACK;
				rotationRight(parent);
			}
			else
			{
				grandParent->color = RED;
				child->color = BLACK;
				rotationLeft(input);
				rotationRight(input);
			}
		}
		else
		{
			if (parent->left_child == input)
			{

				grandParent->color = RED;
				child->color = BLACK;
				rotationRight(input);
				rotationLeft(input);

			}
			else
			{
				grandParent->color = RED;
				parent->color = BLACK;
				rotationLeft(parent);
			}
		}
	}

	// Case 2 : ���� ��尡 RED
	// �θ�, �θ��� ���� ��� <-> �θ��� �θ� ��� �� ����
	// grandparent�� RED�� �Ǹ鼭 ���ĵ� �� �����Ƿ� grandparent�� redBlackBalance üũ
	void recoloring(Node* input)
	{ 
		Node* grandParent = input->parent->parent;
		Node* leftParent = grandParent->left_child;
		Node* rightParent = grandParent->right_child;
		grandParent->color = RED;
		leftParent->color = BLACK;
		rightParent->color = BLACK;
		redBlackBalance(grandParent);
	}

	// ���ο� ������û ��� ���� �Ǵ� ����
	void insert(Register r)
	{
		int D = 0;				// Ʈ������ ������û ������ ����� ����� ����
		int U = 0;				// ������ ������ key�� �����ߴ��� ���� ������(1), �������� ����(0) 

		// Ʈ���� ����ִ� ���
		if (root == nullptr)
		{
			root = new Node{r};

			// ���� ��� ����
			Node* left_child = new Node{};
			root->insertLeftChild(left_child);
			Node* right_child = new Node{};
			root->insertRightChild(right_child);

			// ��� ���� BLACK���� ����
			root->color = BLACK;

			// ����, �л��� ��� ���տ� ����
			mapSubjectOrderedTimestamp[r.subject].insert(root);
			mapSidOrderedSubject[r.sid].insert(root);
		}
		
		else
		{
			Node* temp = root;
			
			// ��尡 ���� ��尡 �ƴҶ� 
			while (temp->left_child != nullptr && temp->right_child != nullptr)
			{
				// �й� �� �� ���� �κ� Ʈ��, ������ �κ� Ʈ�� Ž�� ����
				if (temp->r.sid > r.sid)
					temp = temp->left_child;
				else if (temp->r.sid < r.sid)
					temp = temp->right_child;
				// �й� ���Ͻ� ���� �̸� ��
				else
				{
					// �й�, ���� ���Ͻ� �ش� ��带 �������� �ʰ�, timestamp ����
					if (temp->r.subject == r.subject)
					{
						U = 1;
						D = temp->depth;
						
						// ���� ���տ��� �ش� ����� timestamp ������ ������Ʈ�ϱ� ���� �ش� ��� ������ �����
						auto& ms = mapSubjectOrderedTimestamp[r.subject];
						auto it = ms.find(temp);
						ms.erase(it);

						temp->r.timestamp = r.timestamp;

						mapSubjectOrderedTimestamp[r.subject].insert(temp);
						break;
					}
					else if (temp->r.subject > r.subject)
						temp = temp->left_child;
					else
						temp = temp->right_child;
				}
			}

			// ������ ��尡 �ƴ϶��, Ž���� ����� ��ġ�� ����
			if (U != 1)
			{
				// ���� ��忡�� ���� ���� ������Ʈ
				temp->r = r;
				temp->color = RED;
				temp->depth = temp->parent->depth + 1;

				// ���� ����� ���� ��� ����
				Node* left_child = new Node{};
				Node* right_child = new Node{};
				temp->insertLeftChild(left_child);
				temp->insertRightChild(right_child);

				// ���� ���� üũ
				if (temp->parent->color == RED)
					redBlackBalance(temp);

				D = temp->depth;

				// ����, �л��� ��� ���տ� ����
				mapSubjectOrderedTimestamp[r.subject].insert(temp);
				mapSidOrderedSubject[r.sid].insert(temp);
			}
		}

		cout << D << " " << U << "\n";
	}
	
	// Ư�� �л��� ��ü ��û ���� ��� �Լ�
	void printSubject(int sid)
	{
		// �ش� �л��� ��û����� �������� �ʴ� ���
		if (mapSidOrderedSubject[sid].empty())
			cout << "No records found" << "\n";
		else
		{
			// �ش� �л��� ����� �� �ش� ����� �� ���
			for (auto& e : mapSidOrderedSubject[sid])
			{
				cout << e->r.subject << " " << (e->color == BLACK ? "B" : "R") << " ";
			}
			cout << "\n";
		}
	}

	// Ư�� ������ ��û�� �л� �� ��� �Լ�
	void printSubjectNum(string inputSubject)
	{
		int deptSum = 0;		// �ش� ������ ��û�� ������ ���� ��� ������ ����		
		int stuendtNum = 0;		// �ش� ������ ��û�� �л� ��

		for (auto & e : mapSubjectOrderedTimestamp[inputSubject])
		{
			deptSum += e->depth;
			++stuendtNum;
		}

		cout << stuendtNum << " " << deptSum << "\n";
	}

	// Ư�� ������ ���� ���� ��û�� K���� �й� ��ȸ �Լ�
	void printSubjectTimestamp(string inputSubject, int k)
	{
		int count = k;		

		// timestamp�� ���� ������� K���� ����� �й�, ���� ���
		for (auto & e : mapSubjectOrderedTimestamp[inputSubject])
		{
			// K�� ��� �ƴٸ� break
			if (count <= 0)
				break;

			cout << e->r.sid << " " << (e->color == BLACK ? "B" : "R") << " ";
			--count;

		}
		cout << "\n";

	}
	

};

int main()
{

	RedBlackTree rbt;

	// ������ �� 
	int n;
	cin >> n;

	for (int i = 0; i < n; ++i)
	{
		// ���� ����
		char statement;
		cin >> statement;

		// ������û ���� �Է�
		if (statement == 'I')
		{
			Register r;

			cin >> r.sid >> r.subject >> r.sname >> r.semester >> r.phone >> r.timestamp;
			rbt.insert(r);
		}
		// Ư�� �л��� ��ü ��û ���� ��ȸ
		else if (statement == 'L')
		{
			int sid;

			cin >> sid;
			rbt.printSubject(sid);
		}
		// Ư�� ������ ��û�� �л� �� 
		else if (statement == 'C')
		{
			string subject;

			cin >> subject;
			rbt.printSubjectNum(subject);
		}
		// Ư�� ������ ���� ���� ��û�� K���� �й� ��ȸ
		else if (statement == 'M')
		{
			string subject;
			int k;

			cin >> subject >> k;
			rbt.printSubjectTimestamp(subject, k);
		}
	}

}