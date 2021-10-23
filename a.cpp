#if 1
#include <stdio.h>
#include <malloc.h>

void mmemset(void*p,char c, size_t l) {
	char* cc = (char*)p;
	while(l-->0) cc[l]=c;
}
size_t mstrlen(const char str[]) {
  size_t l = 0;
  while(str[l] != '\0')l++;
  return l;
}

void mstrcpy(char dst[], const char src[]) {
  int i  = 0;
  while((dst[i] = src[i]) != '\0') ++i;
}

int mstrcmp(const char str1[], const char str2[]) {
  int i  = 0;
  while(str1[i] != '\0' && str1[i] == str2[i]) ++i;
  return str1[i]-str2[i];
}

size_t mstrhash(const char str[], const size_t mode) {
  size_t h = 8591;
  char c = 0;
  while((c=*str++) != '\0') {
    h = ((h<<5)+h+c)%mode;
  }
  return h % mode;
}

struct Node {
  Node(const char name[], const char desc[], int point) 
    : point{point} {
      mstrcpy(this->name, name);
      mstrcpy(this->desc, desc);
  }

  void setDesc(const char desc[]) { mstrcpy(this->desc, desc); }
  void setPoint(int point) { this->point = point; }
  void setNext(Node* next) { this->next = next; }

  char name[64];
  char desc[1024];
  int point;
  Node* next;
};

template<size_t MAP_CAPACITY = 10000>
struct NodeMap {
  NodeMap(size_t _mapCapacity = MAP_CAPACITY) 
          : mapCapacity{_mapCapacity}, mapSize{0} 
  {
    nodeList = (Node**)malloc(sizeof(Node*) * mapCapacity);
    for (size_t i = 0; i < mapCapacity; ++i)
    {
      // nodeList[i] = (Node*)malloc(sizeof(Node)); 
      nodeList[i] = nullptr;
    }
  }

  ~NodeMap() {
    for (size_t i = 0; i < mapCapacity; i++)
    {
      if(nodeList[i]) free(nodeList[i]);
    }
    free(nodeList);
  }

  void put(const char name[32], const char desc[128]) {
    size_t h = mstrhash(name, mapCapacity);
    Node* prev = nullptr;
    Node* node = nodeList[h];
    while(node) {
      if(mstrcmp(name, node->name) == 0) break;
      prev = node;
      node = node->next;
    }

    if(prev) {
      if(node) {
        mstrcpy(node->desc, desc);
      }
      else {
        node = (Node*)malloc(sizeof(Node));
        mstrcpy(node->name, name);
        mstrcpy(node->desc, desc);

        prev->next = node;

        mapSize++;
      }
    }
    else {
      node = (Node*)malloc(sizeof(Node));
      mstrcpy(node->name, name);
      mstrcpy(node->desc, desc);

      nodeList[h] = node;

      mapSize++;
    }
  }

  Node** nodeList;
  size_t mapCapacity;
  size_t mapSize;
};


int main(int argc, char* argv[]) {

  NodeMap<30000> nm;

  printf("#1 mstrcmp(%s,%s)=%s\n", argv[0],argv[0],mstrcmp(argv[0],argv[0])==0? "OK":"NG");
  char bb[100];
  mmemset(bb, '\0', 100);
  mstrcpy(bb, argv[0]);
  printf("#2 mstrcmp(%s,mstrcpy(%s)=%s\n", argv[0],bb, mstrcmp(argv[0],bb)==0? "OK":"NG");

  for (int i = 1; i < argc; ++i) {
	  printf("mstrlen(%s)=%d\n", argv[i], mstrlen(argv[i]));
	  if(i%2==1) 
		nm.put(argv[i], argv[i+1]);	
  }

  return 0;
}

#endif
#if 0


#include <stdio.h>
#include <malloc.h>

#define MAX 0xFFFFFFFF

void mmemset(void* p, char c, size_t s) {
  char* pp = (char*)p;
  while(s-->0)  *pp++ = c;
}

void upheap(int array[], int k) {
  int v;
  v = array[k];
//  array[0] = MAX;
  while(array[k/2]<=v && k > 0) {
    array[k] = array[k/2];
    k/=2;
  }
  array[k] = v;
}

void insert(int array[], int *hn, int v) {
  array[++(*hn)] = v;
  upheap(array, *hn);
}

void downheap(int array[], int n, int k) {
  int i;
  int v = array[k];
  while(k<=n/2) {
    i = k<<1;
    if(i<n&&array[i]<array[i+1]) i++;
    if(v>=array[i]) break;
    array[k] = array[i];
    k = i; 
  }
  array[k] = v;
}

int mdelete(int array[], int *n) {
  int v = array[1];
  array[1] = array[(*n)--];
  downheap(array, *n, 1);
  return v;
}

void heap_sort(int array[], int n) {
  int hn = 0;
  int i = 0;
  for(i = 1;i<=n;i++) {
    insert(array, &hn, array[i]);
  }
  for(i=hn;i>=1;i--) {
    array[i] = mdelete(array, &hn);
  }
  for (int j = 0; j < n; j++)
  {
    array[j] = array[j+1];
  }
}

void print_array(int array[], int size) {
  for (int i = 0; i < size; i++)
  {
    printf("[%4d]", array[i]);
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  int n = 0;
  int* arr;
  
  sscanf(argv[1], "%d", &n);
  printf("n=%d\n", n);
  arr = (int*)malloc(sizeof(int)*n);
  mmemset(arr, 0, sizeof(int)*n);
  for (size_t i = 0; i < n; i++)
  {
    sscanf(argv[i+2], "%d", &(arr[i]));
    printf(">>argv[%d]=%s, %d\n", i, argv[i+2], arr[i]);
  }

  printf("(1) : "); print_array(arr, n);
  heap_sort(arr, n);
  printf("(2) : "); print_array(arr, n);

  return 0;
}


#endif
#if 0
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
 
// helper class for runtime polymorphism demo below
struct B {
  virtual void bar() { std::cout << "B::bar\n"; }
  virtual ~B() = default;
};
struct D : B
{
    D() { std::cout << "D::D\n";  }
    ~D() { std::cout << "D::~D\n";  }
    void bar() override { std::cout << "D::bar\n";  }
};
 
// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
    p->bar();
    return p;
}
 
// helper function for the custom deleter demo below
void close_file(std::FILE* fp) { std::fclose(fp); }
 
// unique_ptr-based linked list demo
struct List {
  struct Node {
    int data;
    std::unique_ptr<Node> next;
    Node(int data) : data{data}, next{nullptr} {}
  };
  List() : head{nullptr} {};
  // N.B. iterative destructor to avoid stack overflow on long lists
  ~List() { clean(); }
  // copy/move and other APIs skipped for simplicity
  void push(int data) {
    auto temp{std::make_unique<Node>(data)};
    if(head) temp->next = std::move(head);
    head = std::move(temp);
  }

  void pop() {
      if(head == nullptr) return;
      head = std::move(head->next);
  }

  void remove(int data) {
      List::Node* node = head.get();
      while(node) {
          if(node->data == data) {
              List::Node* temp = node;
              node = node->next.get();
          }
          node = node->next.get();
      }
  }

  int find(int data) {
      Node* node = head.get();
      while(node) {
          if(node->data == data) {
              return 1;
          }
          if(node->next)
            node = node->next.get();
          else break;
      }
      return 0;
  }

  void clean() {
      while(head) { head = std::move(head->next); }
  }

  friend std::ostream& operator<<(std::ostream& os, const List&o);
private:
  std::unique_ptr<Node> head;
};

std::ostream& operator<<(std::ostream& os, const List& o) {
    List::Node* head = o.head.get();
    while(head) { os << head->data << ' '; head = head->next.get(); }
    return os;
}
 
int main()
{
  std::cout << "1) Unique ownership semantics demo\n";
  {
      auto p = std::make_unique<D>(); // p is a unique_ptr that owns a D
      auto q = pass_through(std::move(p)); 
      assert(!p); // now p owns nothing and holds a null pointer
      q->bar();   // and q owns the D object
  } // ~D called here
 
  std::cout << "2) Runtime polymorphism demo\n";
  {
    std::unique_ptr<B> p = std::make_unique<D>(); // p is a unique_ptr that owns a D
                                                  // as a pointer to base
    p->bar(); // virtual dispatch
 
    std::vector<std::unique_ptr<B>> v;  // unique_ptr can be stored in a container
    v.push_back(std::make_unique<D>());
    v.push_back(std::move(p));
    v.emplace_back(new D);
    for(auto& p: v) p->bar(); // virtual dispatch
  } // ~D called 3 times
 
  std::cout << "3) Custom deleter demo\n";
  std::ofstream("demo.txt") << 'x'; // prepare the file to read
  {
      std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
                                                           &close_file);
      if(fp) // fopen could have failed; in which case fp holds a null pointer
        std::cout << (char)std::fgetc(fp.get()) << '\n';
  } // fclose() called here, but only if FILE* is not a null pointer
    // (that is, if fopen succeeded)
 
  std::cout << "4) Custom lambda-expression deleter demo\n";
  {
    std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
  } // the lambda above is called and D is destroyed
 
  std::cout << "5) Array form of unique_ptr demo\n";
  {
      std::unique_ptr<D[]> p(new D[3]);
  } // calls ~D 3 times
 
  std::cout << "6) Linked list demo\n";
  {
    List l;
    for(long n = 1; n != 1'000'000; ++n) l.push(n);

    std::cout << "l.find(" << 999999 << ")=" << (l.find(999999) ? "true" : "false") << std::endl;
    l.pop();
    std::cout << "l.find(" << 999999 << ")=" << (l.find(999999) ? "true" : "false") << std::endl;

    std::cout << "destroying 1'000'000 nodes... ";
  } // destroys all 1 million nodes
  std::cout << "Done.\n";
}

#endif
