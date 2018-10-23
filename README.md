# ● Study-C Language
C는 1972년 켄 톰슨과 데니스 리치가 벨 연구소에서 일할 당시 새로 개발된 유닉스 운영 체제에서 사용하기 위해 개발한 프로그래밍 언어이다.

## ★ Memory Layout

<p align="center">
 <img src="https://cdncontribute.geeksforgeeks.org/wp-content/uploads/memoryLayoutC.jpg" />
</p>

###### § Code Area (코드 영역)

* 실행할 프로그램의 코드가 저장되는 메모리 공간이다. 따라서 CPU는 코드 영역에 저장 된 명령문들을 하나씩 가져가서 실행을 한다.

###### § Data Area (데이터 영역)

* 전역변수와 Static으로 선언되는 static 변수가 할당된다. 즉, 이 영역에 할당되는 변수들은 프로그램의 시작과 동시에 메모리 공간에 할당되어 프로그램 종료 시까지 남아있게 된다는 특징이 있다.

###### § Stack Area (스택 영역)

* 지역변수와 매개변수가 할당된다. 이렇듯 이 영역에 할당되는 변수들은 선언된 함수를 빠져나가면 소멸된다는 특징이 있다.

* The stack area contains the program stack, a LIFO structure, typically located in the higher parts of memory. On the standard PC x86 computer architecture it grows toward address zero; on some other architectures it grows the opposite direction. A “stack pointer” register tracks the top of the stack; it is adjusted each time a value is “pushed” onto the stack. The set of values pushed for one function call is termed a “stack frame”; A stack frame consists at minimum of a return address.

* Stack, where automatic variables are stored, along with information that is saved each time a function is called. Each time a function is called, the address of where to return to and certain information about the caller’s environment, such as some of the machine registers, are saved on the stack. The newly called function then allocates room on the stack for its automatic and temporary variables. This is how recursive functions in C can work. Each time a recursive function calls itself, a new stack frame is used, so one set of variables doesn’t interfere with the variables from another instance of the function.

###### § Heap Area (힙 영역)

* 프로그래머가 원하는 시점에 변수를 할당하고 또 소멸하도록 지원하는 메모리 영역이다.

* Heap is the segment where dynamic memory allocation usually takes place.

* The heap area begins at the end of the BSS segment and grows to larger addresses from there.The Heap area is managed by malloc, realloc, and free, which may use the brk and sbrk system calls to adjust its size (note that the use of brk/sbrk and a single “heap area” is not required to fulfill the contract of malloc/realloc/free; they may also be implemented using mmap to reserve potentially non-contiguous regions of virtual memory into the process’ virtual address space). The Heap area is shared by all shared libraries and dynamically loaded modules in a process.

# ● Study-C++ Language
C++는 AT&T 벨 연구소의 비야네 스트롭스트룹이 1983년 발표하여 발전한 프로그래밍 언어이다.

## [★ Method Overloading](https://www.tutorialspoint.com/cgi-bin/printpage.cgi)

* You can have multiple definitions for the same function name in the same scope. The definition of the function must differ from each other by the types and/or the number of arguments in the argument list. You cannot overload function declarations that differ only by return type.

```C++
#include <iostream>
using namespace std;
 
class printData {
   public:
      void print(int i) {
        cout << "Printing int: " << i << endl;
      }
      void print(double  f) {
        cout << "Printing float: " << f << endl;
      }
      void print(char* c) {
        cout << "Printing character: " << c << endl;
      }
};

int main(void) {
   printData pd;
 
   // Call print to print integer
   pd.print(5);
   
   // Call print to print float
   pd.print(500.263);
   
   // Call print to print character
   pd.print("Hello C++");
 
   return 0;
}
```

## ★ Operator Overrding

* You can redefine or overload most of the built-in operators available in C++. Thus, a programmer can use operators with user-defined types as well.

* Overloaded operators are functions with special names: the keyword "operator" followed by the symbol for the operator being defined. Like any other function, an overloaded operator has a return type and a parameter list.

```C++
#include <iostream>
using namespace std;

class Box {
   public:
      double getVolume(void) {
         return length * breadth * height;
      }
      void setLength( double len ) {
         length = len;
      }
      void setBreadth( double bre ) {
         breadth = bre;
      }
      void setHeight( double hei ) {
         height = hei;
      }
      
      // Overload + operator to add two Box objects.
      Box operator+(const Box& b) {
         Box box;
         box.length = this->length + b.length;
         box.breadth = this->breadth + b.breadth;
         box.height = this->height + b.height;
         return box;
      }
      
   private:
      double length;      // Length of a box
      double breadth;     // Breadth of a box
      double height;      // Height of a box
};

// Main function for the program
int main() {
   Box Box1;                // Declare Box1 of type Box
   Box Box2;                // Declare Box2 of type Box
   Box Box3;                // Declare Box3 of type Box
   double volume = 0.0;     // Store the volume of a box here
 
   // box 1 specification
   Box1.setLength(6.0); 
   Box1.setBreadth(7.0); 
   Box1.setHeight(5.0);
 
   // box 2 specification
   Box2.setLength(12.0); 
   Box2.setBreadth(13.0); 
   Box2.setHeight(10.0);
 
   // volume of box 1
   volume = Box1.getVolume();
   cout << "Volume of Box1 : " << volume <<endl;
 
   // volume of box 2
   volume = Box2.getVolume();
   cout << "Volume of Box2 : " << volume <<endl;

   // Add two object as follows:
   Box3 = Box1 + Box2;

   // volume of box 3
   volume = Box3.getVolume();
   cout << "Volume of Box3 : " << volume <<endl;

   return 0;
}
```

## ★ Method Overriding

* 메소드 오버라이딩,(method overriding)은 객체 지향 프로그래밍에서 서브클래스 또는 자식 클래스가 자신의 슈퍼클래스들 또는 부모 클래스들 중 하나에 의해 이미 제공된 메소드를 특정한 형태로 구현하는 것을 제공하는 언어의 특징이다. 서브클래스에서의 구현은 부모 클래스에서 같은 이름, 같은 파라미터 또는 시그니처 그리고 같은 반환형을 갖는 메소드를 제공함으로써 슈퍼클래스에서의 구현을 오버라이드한다.

```C++
#include <iostream>

//---------------------------------------------------------------------------
class TRectangle
{
public:
    TRectangle(double l, double w) : length(l), width(w) {}
    virtual void print() const;

private:
    double length;
    double width;
};

//---------------------------------------------------------------------------
void TRectangle::print() const
{
   // print() method of base class.
   std::cout << "Length = " << this->length << "; Width = " << this->width;
}

//---------------------------------------------------------------------------
class TBox : public TRectangle
{
public:
    TBox(double l, double w, double h) : TRectangle(l, w), height(h) {}
    // virtual is optional here, but it is a good practice to remind it to the developer.
    virtual void print() const;
private:
    double height;
};

//---------------------------------------------------------------------------
// print() method of derived class.
void TBox::print() const
{
   // Invoke parent print() method.
   TRectangle::print();
   std::cout << "; Height = " << this->height;
}
```

## ★ Friend

* 경우에 따라 개별 클래스의 모든 함수 또는 클래스의 멤버가 아닌 함수에 멤버 수준 액세스 권한을 부여 하는 것이 더 편리합니다. 클래스 구현자만 이 클래스의 friend를 선언할 수 있습니다. 함수 또는 클래스는 자신을 클래스의 friend로 선언할 수 없습니다. 클래스 선언에서 friend 키워드 및 비멤버 함수 또는 기타 클래스의 이름을 사용하여 클래스의 전용 멤버 및 보호된 멤버에 대한 액세스 권한을 부여합니다.

* In principle, private and protected members of a class cannot be accessed from outside the same class in which they are declared. However, this rule does not apply to "friends".

```C++
// friend functions
#include <iostream>
using namespace std;

class Rectangle {
    int width, height;
  public:
    Rectangle() {}
    Rectangle (int x, int y) : width(x), height(y) {}
    int area() {return width * height;}
    friend Rectangle duplicate (const Rectangle&);
};

Rectangle duplicate (const Rectangle& param)
{
  Rectangle res;
  res.width = param.width*2;
  res.height = param.height*2;
  return res;
}

int main () {
  Rectangle foo;
  Rectangle bar (2,3);
  foo = duplicate (bar);
  cout << foo.area() << '\n';
  return 0;
}
```

## ★ Pointer and Reference

#### ＃ Pointer

* In computer science, a pointer is a programming language object that stores the memory address of another value located in computer memory. A pointer references a location in memory, and obtaining the value stored at that location is known as dereferencing the pointer. As an analogy, a page number in a book's index could be considered a pointer to the corresponding page; dereferencing such a pointer would be done by flipping to the page with the given page number and reading the text found on that page. The actual format and content of a pointer variable is dependent on the underlying computer architecture.

#### # Reference
* In the C++ programming language, a reference is a simple reference datatype that is less powerful but safer than the pointer type inherited from C. The name C++ reference may cause confusion, as in computer science a reference is a general concept datatype, with pointers and C++ references being specific reference datatype implementations. The definition of a reference in C++ is such that it does not need to exist. **It can be implemented as a new name for an existing object** (similar to rename keyword in Ada).

###### § Reference Features

* It is not possible to refer directly to a reference object after it is defined; any occurrence of its name refers directly to the object it references.

* Once a reference is created, it cannot be later made to reference another object; it cannot be reseated. This is often done with pointers.

* References cannot be null, whereas pointers can; every reference refers to some object, although it may or may not be valid. Note that for this reason, containers of references are not allowed.

* References cannot be uninitialized. Because it is impossible to reinitialize a reference, they must be initialized as soon as they are created. In particular, local and global variables must be initialized where they are defined, and references which are data members of class instances must be initialized in the initializer list of the class's constructor. For example:

* * *

#### [※ Difference Pointer and Reference](https://www.geeksforgeeks.org/references-in-c/)

<p align="center">
 <img src="https://user-images.githubusercontent.com/20036523/47368932-8f1f0b80-d71d-11e8-9a53-3e2f72aa85d7.jpg" />
</p>

**◆ References are less powerful than pointers**

1) Once a reference is created, it cannot be later made to reference another object; it cannot be reseated. This is often done with pointers.
2) References cannot be NULL. Pointers are often made NULL to indicate that they are not pointing to any valid thing.
3) A reference must be initialized when declared. There is no such restriction with pointers

* Due to the above limitations, references in C++ cannot be used for implementing data structures like Linked List, Tree, etc. In Java, references don’t have above restrictions, and can be used to implement all data structures. References being more powerful in Java, is the main reason Java doesn’t need pointers.

**◆ References are safer and easier to use:**

1) Safer: Since references must be initialized, wild references like wild pointers are unlikely to exist. It is still possible to have references that don’t refer to a valid location (See questions 5 and 6 in the below exercise )
2) Easier to use: References don’t need dereferencing operator to access the value. They can be used like normal variables. ‘&’ operator is needed only at the time of declaration. Also, members of an object reference can be accessed with dot operator (‘.’), unlike pointers where arrow operator (->) is needed to access members.

* Together with the above reasons, there are few places like copy constructor argument where pointer cannot be used. Reference must be used pass the argument in copy constructor. Similarly references must be used for overloading some operators like ++.

## ★ Virtual Functions & Pure Virtual Functions

* 가상 함수는 파생 클래스가 안전하게 재정의할 수 있는 함수이다. 만약 상속 관계가 아니라면 가상 함수를 선언할 필요가 없으므로 가상 함수는 상속 계층내에서만 의미가 있으며 파생 클래스에게 재정의 기회를 주기 위해 존재하는 것이라고 할 수 있다. 그러나 가상 함수를 반드시 재정의해야만 하는 것은 아니다. 기반 클래스의 동작을 그대로 쓰고 싶으면 단순히 상속만 받고 변경할 필요가 있을 때만 재정의하면 된다. 기반 클래스가 가상 함수를 만드는 이유는 혹시라도 재정의하고 포인터로 호출할 때를 대비한 것이다. 가상 함수는 재정의해도 되는 함수이지 반드시 재정의해야 하는 함수는 아니다.

* 순수 가상 함수(Pure Virtual Function)는 파생 클래스에서 반드시 재정의해야 하는 함수이다. 순수 가상 함수는 일반적으로 함수의 동작을 정의하는 본체를 가지지 않으며 따라서 이 상태에서는 호출할 수 없다. 본체가 없다는 뜻으로 함수 선언부의 끝에 =0이라는 표기를 하는데 이는 함수만 있고 코드는 비어 있다는 뜻이다.

#### ※ Difference Virtual Functions and Pure Virtual Functions

* A virtual function makes its class a polymorphic base class. Derived classes can override virtual functions. Virtual functions called through base class pointers/references will be resolved at run-time.

```C++
class Base {
  // ...
  virtual void f() = 0;
  // ...

Derived d;
 Base& rb = d;
 // if Base::f() is virtual and Derived overrides it, Derived::f() will be called
 rb.f();  
```

* A pure virtual function implicitly makes the class it is defined for abstract (unlike in Java where you have a keyword to explicitly declare the class abstract). Abstract classes cannot be instantiated. Derived classes need to override/implement all inherited pure virtual functions. If they do not, they too will become abstract.

## ★ REFERENCE
* [메소드 오버라이딩,(method overriding) - 위키백과](https://ko.wikipedia.org/wiki/%EB%A9%94%EC%86%8C%EB%93%9C_%EC%98%A4%EB%B2%84%EB%9D%BC%EC%9D%B4%EB%94%A9)
* [C++ Overloading (Operator and Function)](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm)
* [Memory Layout of C Programs - ](https://www.geeksforgeeks.org/memory-layout-of-c-program/)
* [순수 가상 함수 -C/C++ by WinAPI](http://soen.kr/lecture/ccpp/cpp3/30-3-1.htm)
* [Difference between a virtual function and a pure virtual function [duplicate]](https://stackoverflow.com/questions/2652198/difference-between-a-virtual-function-and-a-pure-virtual-function)
* [friend (C++) - MS](https://msdn.microsoft.com/ko-kr/library/465sdshe.aspx)
* [Friendship and inheritance - cplusplus](http://www.cplusplus.com/doc/tutorial/inheritance/)
* [References in C++ - GeeksforGeeks](https://www.geeksforgeeks.org/references-in-c/)
* [Pointer (computer programming) - 위키백과](https://en.wikipedia.org/wiki/Pointer_(computer_programming))
* [Difference between Reference and Pointer in C++](https://www.thecrazyprogrammer.com/2016/12/difference-between-reference-and-pointer.html)
