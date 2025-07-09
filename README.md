# 📚 수강신청 관리 프로그램 (Red-Black Tree 기반)

## 🔧 프로젝트 목적

레드블랙트리(Red-Black Tree)를 기반으로 학생들의 수강신청 정보를 **효율적으로 관리**하는 프로그램을 구현합니다.  
O(log n)의 시간 복잡도로 **삽입, 조회, 갱신**이 가능한 구조로 설계되어 대용량의 수강 정보를 안정적으로 처리할 수 있습니다.

---

## 📌 기능 정보

### 1. 수강신청 정보 입력 및 갱신
```
I sid subject sname semester phone timestamp
```
- 신규 수강신청 정보 삽입 또는 기존 정보 갱신
- 출력:  
  ```
  D U
  ```
  - D: 삽입/갱신된 노드의 깊이  
  - U: 갱신 여부 (1: 갱신, 0: 신규)

---

### 2. 특정 학생의 신청 과목 조회
```
L sid
```
- 해당 학번의 학생이 신청한 과목을 **과목명 기준 사전순으로 조회**
- 출력:  
  ```
  subject1 color1 subject2 color2 ...
  ```
  - `color`: RED 또는 BLACK  
- 존재하지 않는 경우:
  ```
  No records found
  ```

---

### 3. 특정 과목의 수강생 수 및 깊이 합산
```
C subject
```
- 출력:
  ```
  N SUM
  ```
  - N: 해당 과목 수강 인원 수
  - SUM: 해당 노드들의 깊이 총합

---

### 4. 특정 과목의 선착순 신청 K명 조회
```
M subject K
```
- 출력:
  ```
  sid1 color1 sid2 color2 ...
  ```

---

## 📌 개발 환경

- **Language**: C++14  
- **IDE**: Visual Studio 2022  
- **CPU**: Intel

---

## 📌 주요 자료구조 및 구성

### 1. `Register` 구조체
```cpp
int sid;          // 학번
string subject;   // 과목명
string sname;     // 이름
int semester;     // 이수학기
string phone;     // 전화번호
int timestamp;    // 신청 시간
```

---

### 2. `Node` 클래스

| 멤버 변수 | 설명 |
|-----------|------|
| `Node* parent, left_child, right_child` | 연결 포인터 |
| `Color color` | 노드 색 (RED/BLACK) |
| `Register r` | 수강신청 정보 |
| `int depth` | 트리에서의 깊이 |

---

### 3. `RedBlackTree` 클래스

| 멤버 변수 | 설명 |
|-----------|------|
| `Node* root` | 트리 루트 |
| `unordered_map<string, set<Node*, compTimestamp>>` | 과목별 timestamp 순 정렬 |
| `unordered_map<int, set<Node*, compSubject>>` | 학번별 과목 사전순 정렬 |

| 주요 함수 | 기능 |
|-----------|------|
| `insert()` | 수강신청 입력/갱신 |
| `redBlackBalance()` | Double Red 처리 |
| `restructuring(), recoloring()` | 균형 맞춤 |
| `rotationLeft(), rotationRight()` | 좌/우 회전 |
| `printSubject(sid)` | 특정 학생 과목 조회 |
| `printSubjectNum(subject)` | 과목별 수강생 수 & 깊이합 |
| `printSubjectTimestamp(subject, K)` | 선착순 K명 조회 |

---

### 4. 비교 함수

- `compTimestamp`: timestamp 기준 오름차순 정렬  
- `compSubject`: 과목명 기준 사전순 정렬

---

## 📌 기능별 알고리즘 & 시간 복잡도

| 기능 | 알고리즘 요약 | 시간 복잡도 |
|------|----------------|--------------|
| **수강신청 입력/갱신** | 이진 탐색 + 트리 삽입/갱신 + 균형 조정 | O(log n) |
| **학생별 과목 조회 (L)** | 학번별 해시 접근 + 정렬된 set 순회 | O(k) |
| **과목별 수강생 수 조회 (C)** | 과목별 해시 접근 + 노드 순회 | O(m) |
| **과목별 선착순 조회 (M)** | 과목별 해시 접근 + K개 순회 | O(min(K, m)) |

---

## 📌 인터페이스 및 사용법

1. 실행 후 질의 수 `n` 입력  
2. `n`개의 명령 입력 반복 (최대 100,000)

### 예시 입력
```
I 12201234 Algorithms Minseok 6 01023456789 10204
L 12201234
C Algorithms
M Algorithms 2
```

---

## ✅ 장점

- 레드블랙트리를 이용하여 모든 연산이 **O(log n)**으로 처리 가능
- `unordered_map + set` 자료구조로 **학생/과목별 빠른 접근성 확보**
- 효율적이고 확장 가능한 설계

---

## ⚠️ 단점

- 메모리 사용량 증가 (`학생수 + 과목수 + 2N` 규모의 자료구조 유지)
- 삽입 시 `map<, set<>>` 업데이트 필요로 인해 단일 트리 탐색보다 느림

---

## 🚀 향후 개선 방향

- `C subject` 기능의 시간 단축을 위해  
  → **과목별 학생 수 및 깊이 합을 삽입 시점에 미리 누적 관리**  
  → 조회 시 반복 없이 O(1) 또는 O(1 + log m) 가능하게 개선
