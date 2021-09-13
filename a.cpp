#if 1
#include <stdio.h>
#include <malloc.h>

void mmemset(void* p, char c, size_t l) {
  char* cp = (char*)p;
  while(l--) *cp++ = c;
}

size_t mstrhash(const char str[], size_t mod) {
  size_t h = 5381;
  char c = 0;
  while((c=*str++) != '\0') {
    h = ((h<<5)+h+c)%mod;
  }
  return h % mod;
}
void mstrcpy(char dst[], const char src[]) {
  int i =0;
  while((dst[i] = src[i]) != '\0') ++i;
}

int mstrcmp(const char str1[], const char str2[]) {
  int i = 0;
  while(str1[i] != '\0' && str1[i] == str2[i]) ++i;
  return str1[i]-str2[2];
}

struct Node {
  void* key;
  void* data;
  Node* next;
};

#define DEFAULT_HASH_MAP_MAX_SIZE 100000
#define DEFAULT_STRING_SIZE 256
struct HashMap {
  HashMap(
   size_t (*key_hash)(void* data, size_t mod),
   int (*key_match)(void* key1, void* key2),
   void (*destroy)(void* data),
   size_t max_size = DEFAULT_HASH_MAP_MAX_SIZE)
    : key_hash{key_hash}, 
      key_match{key_match}, 
      destroy{destroy}, 
      size{0}, 
      max_size{max_size}
  {
      nodeList = (Node**)malloc(max_size*sizeof(Node*));
      mmemset(nodeList, 0, sizeof(Node*)*max_size);
  }

  ~HashMap() {
    for (Node* p = nodeList[0]; p != nodeList[max_size-1]; ++p)
    {
      if(!p) continue;
      destroy(p->data);
      free(p);
    }
    free(nodeList);
    nodeList = NULL;
  }

  void insert(void* key, void* data) {
    size_t h = key_hash(key, max_size);
    Node* prev = NULL;
    Node* node = nodeList[h];
    while(node && !key_match(key, node->key)) {
      prev = node;
      node = node->next;
    }

    if(node) { // update
      destroy(node->data);
      node->key = key;
      node->data = data;  
    }
    else { // insert
      node = (Node*)malloc(sizeof(Node));
      mmemset(node, 0, sizeof(Node));
      node->key = key;
      node->data = data;

      if(prev) {
        node->next = prev->next;
        prev->next = node; 
      }
      else {
        nodeList[h] = node;
      }
      size++;
    }
  }

  void* get(void* key) const {
    size_t h = key_hash(key, max_size);
    Node* prev = NULL;
    Node* node = nodeList[h];
    while(node && !key_match(key, node->key)) {
      prev = node;
      node = node->next;
    }
    if(node) return node->data;
    return NULL;
  }

  size_t (*key_hash)(void* data, size_t mod);
  int (*key_match)(void* key1, void*key2);
  void (*destroy)(void* data);
  Node** nodeList;
  size_t size;
  size_t max_size;
};

struct string {
  string(const char str[]) 
    :len{DEFAULT_STRING_SIZE}
    {
      mmemset(this->str, 0, DEFAULT_STRING_SIZE);
      mstrcpy(this->str, str);
    }
  char str[DEFAULT_STRING_SIZE];
  size_t len;
};

size_t string_hash(void* key, size_t mod) {
  return mstrhash((const char*)key, mod);
}

int string_key_match(void* key1, void* key2) {
  return mstrcmp((const char*)key1, (const char*)key2) == 0;
}

void string_destroy(void* data) {
  string* s = (string*)data;
  free(s);
}

int main(int argc, char* argv[]) {
  
  HashMap m{string_hash, string_key_match, string_destroy};
  for (size_t i = 0; i < argc-1; ++i)
  {
    string* s  = new string{argv[i+1]};
    m.insert(s->str, s);
  }

  string* s = NULL;
  for (size_t i = 0; i < argc-i; ++i)
  {
    s = (string*)m.get(argv[i+1]);
    printf("list[%d]=%s\n", i, s  ? s->str : "") ;
  }
  
  return 0;
}


#endif


#if 0
#include <stdio.h>
#include <malloc.h>
#include <time.h>

/* returns an array of arrays of char*, all of which NULL */
char ***alloc_matrix(unsigned rows, unsigned columns) {
    char ***matrix = (char***)malloc(rows * sizeof(char **));
    unsigned row = 0;
    unsigned column = 0;
    if (!matrix) abort();

    for (row = 0; row < rows; row++) {
        matrix[row] = (char**)calloc(columns, sizeof(char *));
        if (!matrix[row]) abort();
        for (column = 0; column < columns; column++) {
            matrix[row][column] = NULL;
        }
    }
    return matrix;
}

/* deallocates an array of arrays of char*, calling free() on each */
void free_matrix(char ***matrix, unsigned rows, unsigned columns) {
    unsigned row = 0;
    unsigned column = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            /*    printf("column %d row %d\n", column, row);*/
            free(matrix[row][column]);
        }
        free(matrix[row]);
    }
    free(matrix);
}


int main(int argc, char **argv) {
    if(argc < 2) { printf("Usage : %s size1 size2\n", argv[0]); return 0; }
    printf("Start.\n");
     int i;
    srand(time(NULL));
    int randomnumber;
    int size = atoi(argv[1]);
    void *p[size];
    for (i = 0; i < size; i++) {
        randomnumber = rand() % 10;
        p[i] = malloc(1024 * 1024 * randomnumber);
    }

    for (i = size-1; i >= 0; i--) {
        free(p[i]);
    }

    int x = atoi(argv[2]);
    char *** matrix = alloc_matrix(x, x);
    free_matrix(matrix, x, x);
    printf("End.\n");
    return (0);
}

#endif

#if 0
#include <stdio.h>
#include <malloc.h>

static void mmemset(void* ptr, char c, size_t len) {
  char* cp = (char*)ptr;
  while(len--) {
    *cp++ = c;
  }
}
static void mstrcpy(char a[], char b[]) {
  int i = 0;
  while((a[i] = b[i]) != '\0') {++i;}
}

static int mstrcmp(const char a[], const char b[]) {
  int i = 0;
  while((a[i] != '\0') && a[i] == b[i]) ++i;
  return a[i] - b[i];
}

static const size_t NAME_LEN = 16;

struct Node {
  char name[16];
  Node* next;
};

struct NodeMap {
  size_t len;
  Node** nodeList;
  NodeMap(size_t len = 100000) :len(len) {
    nodeList = (Node**)malloc(sizeof(Node*)*len);
    for(int i = 0;i < len;++i) {
     nodeList[i] = (Node*)malloc(sizeof(Node));
     mmemset(nodeList[i]->name, 0, NAME_LEN);
   }
  }

  ~NodeMap() {
    for(int i = 0;i < len;++i) {
     free(nodeList[i]);
   }
   free(nodeList);
   printf("~NodeMap()\n");
  }
};

NodeMap nodeMap;

int main(int argc, char* argv[]) {

  for(int i =1;i<argc;++i) {
    mstrcpy(nodeMap.nodeList[i-1]->name, argv[i]); 
  }

  for(int i = 1;i<argc;++i) {
    printf("nodeMap.nodeList[%d]=%s\n", i, nodeMap.nodeList[i-1]->name);
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
