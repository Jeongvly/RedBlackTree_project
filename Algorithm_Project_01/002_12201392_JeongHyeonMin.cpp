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

// 수강신청 정보를 담는 구조체
struct Register
{
	int sid;				// 학번
	string subject;			// 신청과목
	string sname;			// 이름
	int semester;			// 이수학기수
	string phone;			// 전화번호
	int timestamp;			// 수강신청 시간
};


// 레드블랙트리의 노드 클래스
class Node
{
public:
	Node* parent;				// 부모 노드
	Node* left_child;			// 왼쪽 자식 노드
	Node* right_child;			// 오른쪽 자식 노드	
	Color color;				// 노드 색상
	Register r;					// 노드에 저장된 등록 정보
	int depth;					// 노드의 깊이 

	// 수간신청 정보로 초기화 
	Node(Register r)
		:parent{ nullptr }, left_child{ nullptr }, right_child{ nullptr }, color{ RED }, r{ r }, depth{0}
	{
	}

	// 기본 초기화 
	Node()
		:parent{ nullptr }, left_child{ nullptr }, right_child{ nullptr }, color{ BLACK }, r{}, depth{0}
	{
	}

	// 왼쪽 자식 삽입
	void insertLeftChild(Node* left)
	{
		left_child = left;
		left->parent = this;
	}

	// 오른쪽 자식 삽입
	void insertRightChild(Node* right)
	{
		right_child = right;
		right->parent = this;
	}

};

//Timestamp 순으로 노드 비교
struct compTimestamp {
	bool operator()(Node* a, Node* b) const
	{
		return a->r.timestamp < b->r.timestamp; 
	}
};

// 과목 이름 사전순으로 노드 비교
struct compSubject {
	bool operator()(Node* a, Node* b) const
	{
		return a->r.subject < b->r.subject;
	}
};


// 레드블랙트리 클래스
class RedBlackTree
{
public:
	Node *root;
	unordered_map<string, set<Node*, compTimestamp>> mapSubjectOrderedTimestamp;		// 과목별 timestamp 순서로 정렬된 노드 집합
	unordered_map<int, set<Node*, compSubject>> mapSidOrderedSubject;					// 학생별 과목 사전 순서로 정렬된 노드 집합 

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

	// 삽입 후 레드블랙 특성 위배시 균형 맞추는 함수
	void redBlackBalance(Node* input)
	{
		// 루트 노드는 BLACK
		if (input == root)
			input->color = BLACK;
		else if (input->parent->color == RED)
		{
			Node* parent = input->parent;
			Node* child = input;
			
			// 부모의 형제 노드 
			Node* sibling = (parent->parent->left_child == parent ? parent->parent->right_child : parent->parent->left_child);

			// 부모의 형제 노드가 RED면 recoloring, BLACK이면 restructuring
			if (sibling->color == RED)
				recoloring(child);
			else
				restructuring(child);
		}
	}

	// 해당 노드의 깊이 감소 함수
	void downDepth(Node* input)
	{
		if (input != nullptr)
		{
			input->depth -= 1;
			downDepth(input->left_child);
			downDepth(input->right_child);
		}
	}

	// 해당 노드의 깊이 증가 함수
	void upDepth(Node* input)
	{
		if (input != nullptr)
		{
			input->depth += 1;
			upDepth(input->left_child);
			upDepth(input->right_child);
		}
	}

	// 오른쪽 회전 함수
	void rotationRight(Node* input)
	{
		Node* parent = input->parent;
		Node* child = input;
		// 깊이값 스왑 후 조정
		swap(parent->depth, child->depth);
		upDepth(parent->right_child);
		downDepth(child->left_child);

		// 부모가 root인 경우
		if (parent == root)
		{
			child->parent = nullptr;
			root = child;
		}

		// 부모가 왼쪽 자식인 경우
		else if (parent->parent->left_child == parent)
		{

			parent->parent->left_child = child;
			child->parent = parent->parent;
		}

		// 부모가 오른쪽 자식인 경우
		else
		{
			parent->parent->right_child = child;
			child->parent = parent->parent;
		}

		// 자식, 부모 업데이트
		parent->parent = child;
		parent->left_child = child->right_child;
		child->right_child->parent = parent;
		child->right_child = parent;
	}

	// 왼쪽 회전 함수
	void rotationLeft(Node* input)
	{
		Node* parent = input->parent;
		Node* child = input;

		// 깊이값 스왑 후 조정
		swap(parent->depth, child->depth);
		upDepth(parent->left_child);
		downDepth(child->right_child);

		// 부모가 root인 경우
		if (parent == root)
		{
			child->parent = nullptr;
			root = child;
		}

		// 부모가 왼쪽 자식인 경우
		else if (parent->parent->left_child == parent)
		{

			parent->parent->left_child = child;
			child->parent = parent->parent;
		}

		// 부모가 오른쪽 자식인 경우
		else
		{
			parent->parent->right_child = child;
			child->parent = parent->parent;
		}

		// 자식, 부모 업데이트
		parent->parent = child;
		parent->right_child = child->left_child;
		child->left_child->parent = parent;
		child->left_child = parent;
	}

	// Case 1: 형제 노드가 BLACK
	// 4가지 구조 (LL, LR, RL, RR)로 나누어서 rotation을 통해 조정 
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

	// Case 2 : 형제 노드가 RED
	// 부모, 부모의 형제 노드 <-> 부모의 부모 노드 색 변경
	// grandparent가 RED가 되면서 전파될 수 있으므로 grandparent의 redBlackBalance 체크
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

	// 새로운 수강신청 등록 삽입 또는 갱신
	void insert(Register r)
	{
		int D = 0;				// 트리에서 수강신청 정보가 저장된 노드의 깊이
		int U = 0;				// 기존에 동일한 key가 존재했는지 여부 존재함(1), 존재하지 않음(0) 

		// 트리가 비어있는 경우
		if (root == nullptr)
		{
			root = new Node{r};

			// 더미 노드 생성
			Node* left_child = new Node{};
			root->insertLeftChild(left_child);
			Node* right_child = new Node{};
			root->insertRightChild(right_child);

			// 노드 색상 BLACK으로 변경
			root->color = BLACK;

			// 과목별, 학생별 노드 집합에 삽입
			mapSubjectOrderedTimestamp[r.subject].insert(root);
			mapSidOrderedSubject[r.sid].insert(root);
		}
		
		else
		{
			Node* temp = root;
			
			// 노드가 더미 노드가 아닐때 
			while (temp->left_child != nullptr && temp->right_child != nullptr)
			{
				// 학번 비교 후 왼쪽 부분 트리, 오른쪽 부분 트리 탐색 설정
				if (temp->r.sid > r.sid)
					temp = temp->left_child;
				else if (temp->r.sid < r.sid)
					temp = temp->right_child;
				// 학번 동일시 과목 이름 비교
				else
				{
					// 학번, 과목 동일시 해당 노드를 삽입하지 않고, timestamp 갱신
					if (temp->r.subject == r.subject)
					{
						U = 1;
						D = temp->depth;
						
						// 과목별 집합에서 해당 노드의 timestamp 순서로 업데이트하기 위해 해당 노드 삭제후 재삽입
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

			// 동일한 노드가 아니라면, 탐색한 노드의 위치에 삽입
			if (U != 1)
			{
				// 더미 노드에서 삽입 노드로 업데이트
				temp->r = r;
				temp->color = RED;
				temp->depth = temp->parent->depth + 1;

				// 삽입 노드의 더미 노드 생성
				Node* left_child = new Node{};
				Node* right_child = new Node{};
				temp->insertLeftChild(left_child);
				temp->insertRightChild(right_child);

				// 더블 레드 체크
				if (temp->parent->color == RED)
					redBlackBalance(temp);

				D = temp->depth;

				// 과목별, 학생별 노드 집합에 삽입
				mapSubjectOrderedTimestamp[r.subject].insert(temp);
				mapSidOrderedSubject[r.sid].insert(temp);
			}
		}

		cout << D << " " << U << "\n";
	}
	
	// 특정 학생의 전체 신청 과목 출력 함수
	void printSubject(int sid)
	{
		// 해당 학생의 신청기록이 존재하지 않는 경우
		if (mapSidOrderedSubject[sid].empty())
			cout << "No records found" << "\n";
		else
		{
			// 해당 학생의 과목명 및 해당 노드의 색 출력
			for (auto& e : mapSidOrderedSubject[sid])
			{
				cout << e->r.subject << " " << (e->color == BLACK ? "B" : "R") << " ";
			}
			cout << "\n";
		}
	}

	// 특정 과목을 신청한 학생 수 출력 함수
	void printSubjectNum(string inputSubject)
	{
		int deptSum = 0;		// 해당 과목을 신청한 정보를 담은 노드 깊이의 총합		
		int stuendtNum = 0;		// 해당 과목을 신청한 학생 수

		for (auto & e : mapSubjectOrderedTimestamp[inputSubject])
		{
			deptSum += e->depth;
			++stuendtNum;
		}

		cout << stuendtNum << " " << deptSum << "\n";
	}

	// 특정 과목을 가장 먼저 신청한 K명의 학번 조회 함수
	void printSubjectTimestamp(string inputSubject, int k)
	{
		int count = k;		

		// timestamp가 빠른 순서대로 K개의 노드의 학번, 색을 출력
		for (auto & e : mapSubjectOrderedTimestamp[inputSubject])
		{
			// K번 출력 됐다면 break
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

	// 질의의 수 
	int n;
	cin >> n;

	for (int i = 0; i < n; ++i)
	{
		// 질의 종류
		char statement;
		cin >> statement;

		// 수강신청 정보 입력
		if (statement == 'I')
		{
			Register r;

			cin >> r.sid >> r.subject >> r.sname >> r.semester >> r.phone >> r.timestamp;
			rbt.insert(r);
		}
		// 특정 학생의 전체 신청 과목 조회
		else if (statement == 'L')
		{
			int sid;

			cin >> sid;
			rbt.printSubject(sid);
		}
		// 특정 과목을 신청한 학생 수 
		else if (statement == 'C')
		{
			string subject;

			cin >> subject;
			rbt.printSubjectNum(subject);
		}
		// 특정 과목을 가장 먼저 신청한 K명의 학번 조회
		else if (statement == 'M')
		{
			string subject;
			int k;

			cin >> subject >> k;
			rbt.printSubjectTimestamp(subject, k);
		}
	}

}