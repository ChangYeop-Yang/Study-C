# ■ Study - C/C++ Language

* C++는 AT&T 벨 연구소의 비야네 스트롭스트룹이 1983년 발표하여 발전한 프로그래밍 언어이다.

## 📣 메모리 계층 (Memory Layout)

<p align="center">
 <img src="https://cdncontribute.geeksforgeeks.org/wp-content/uploads/memoryLayoutC.jpg" />
</p>

###### 🔑 코드 영역 (Code Area)

* 실행할 프로그램의 코드가 저장되는 메모리 공간이다. 따라서 CPU는 코드 영역에 저장 된 명령문들을 하나씩 가져가서 실행을 한다.

###### 🔑 데이터 영역 (Data Area)

* 전역변수와 Static으로 선언되는 static 변수가 할당된다. 즉, 이 영역에 할당되는 변수들은 프로그램의 시작과 동시에 메모리 공간에 할당되어 프로그램 종료 시까지 남아있게 된다는 특징이 있다.

###### 🔑 스택 영역 (Stack Area)

* 지역변수와 매개변수가 할당된다. 이렇듯 이 영역에 할당되는 변수들은 선언된 함수를 빠져나가면 소멸된다는 특징이 있다.

* The stack area contains the program stack, a LIFO structure, typically located in the higher parts of memory. On the standard PC x86 computer architecture it grows toward address zero; on some other architectures it grows the opposite direction. A “stack pointer” register tracks the top of the stack; it is adjusted each time a value is “pushed” onto the stack. The set of values pushed for one function call is termed a “stack frame”; A stack frame consists at minimum of a return address.

* Stack, where automatic variables are stored, along with information that is saved each time a function is called. Each time a function is called, the address of where to return to and certain information about the caller’s environment, such as some of the machine registers, are saved on the stack. The newly called function then allocates room on the stack for its automatic and temporary variables. This is how recursive functions in C can work. Each time a recursive function calls itself, a new stack frame is used, so one set of variables doesn’t interfere with the variables from another instance of the function.

###### 🔑 힙 영역 (Heap Area)

* 프로그래머가 원하는 시점에 변수를 할당하고 또 소멸하도록 지원하는 메모리 영역이다.

* Heap is the segment where dynamic memory allocation usually takes place.

* The heap area begins at the end of the BSS segment and grows to larger addresses from there.The Heap area is managed by malloc, realloc, and free, which may use the brk and sbrk system calls to adjust its size (note that the use of brk/sbrk and a single “heap area” is not required to fulfill the contract of malloc/realloc/free; they may also be implemented using mmap to reserve potentially non-contiguous regions of virtual memory into the process’ virtual address space). The Heap area is shared by all shared libraries and dynamically loaded modules in a process.

## 📣 [접근제어 지시자 (Access Modifier)](https://www.geeksforgeeks.org/access-modifiers-in-c/)

#### 💊 Private

* 자기 클래스 내부의 메서드에서만 접근 허용한다.

* The class members declared as private can be accessed only by the functions inside the class. They are not allowed to be accessed directly by any object or function outside the class. Only the member functions or the friend functions are allowed to access the private data members of a class.

#### 💊 Protected 

* 자기 클래스 내부 또는 상속받은 자식 클래스에서 접근 허용한다.

* Protected access modifier is similar to that of private access modifiers, the difference is that the class member declared as Protected are inaccessible outside the class but they can be accessed by any subclass(derived class) of that class.

#### 💊 Public 

* 모든 접근을 허용한다.

* All the class members declared under public will be available to everyone. The data members and member functions declared public can be accessed by other classes too. The public members of a class can be accessed from anywhere in the program using the direct member access operator (.) with the object of that class.

###### 📃 접근제어 지시자 (Access Modifier) Source Code

```C++
class Car {
// Data Fields ///////////////
public: // 지금부터 선언되는 멤버변수와 함수는 모두 접근 허용.
   int  year;
   char maker[50];

protected: // 지금부터 선언되는 멤버변수와 함수는 자기 클래스와 상속 클래스에서만 접근 허용.
   int  capEngine;

private: // 지금부터 선언되는 멤버변수와 함수는 자기 클래스에서만 접근 허용.
   char ecu[20];
   char colorCode[30];

/// Methods ///////////////////////////////////
public:
   Car() {  }  // 생성자는 외부에서 접근 허용이 되어야 한다.
   ~Car() {  }  // 소멸자

   int getMkYear() { return year; }
   int getCapEngine() { return capEngine; }

protected: // 자기 클래스 내부와 상속클래스에서 접근 허용한다.
   char* getEcuType() { return ecu; }
   char* getColorCode() { return colorCode; }

// ....
};
```

## 📣 [함수 오버로드 (Method Overloading)](https://www.tutorialspoint.com/cgi-bin/printpage.cgi)

* You can have multiple definitions for the same function name in the same scope. The definition of the function must differ from each other by the types and/or the number of arguments in the argument list. You cannot overload function declarations that differ only by return type.

###### 📃 함수 오버로드 (Method Overloading) Source Code

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

## 📣 연산자 오버라이딩 (Operator Overrding)

* You can redefine or overload most of the built-in operators available in C++. Thus, a programmer can use operators with user-defined types as well.

* Overloaded operators are functions with special names: the keyword "operator" followed by the symbol for the operator being defined. Like any other function, an overloaded operator has a return type and a parameter list.

###### 📃 연산자 오버라이딩 (Operator Overrding)

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

## 📣 함수 오버라이딩 (Method Overriding)

* 메소드 오버라이딩,(method overriding)은 객체 지향 프로그래밍에서 서브클래스 또는 자식 클래스가 자신의 슈퍼클래스들 또는 부모 클래스들 중 하나에 의해 이미 제공된 메소드를 특정한 형태로 구현하는 것을 제공하는 언어의 특징이다. 서브클래스에서의 구현은 부모 클래스에서 같은 이름, 같은 파라미터 또는 시그니처 그리고 같은 반환형을 갖는 메소드를 제공함으로써 슈퍼클래스에서의 구현을 오버라이드한다.

###### 📃 함수 오버라이딩 (Method Overrding)

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

## 📣 Friend

* 프렌드는 지정한 대상에 한해 해당 객체의 모든 멤버에 접근할 수 있는 권한을 부여해 줍니다. 이러한 friend 키워드는 전역 함수, 클래스, 멤버 함수의 세 가지 형태로 사용할 수 있습니다.

* In principle, private and protected members of a class cannot be accessed from outside the same class in which they are declared. However, this rule does not apply to "friends".

###### 📃  Friend Source Code

* 클래스의 friend 선언을 하면 직접 private 멤버 접근 가능하다.

```C++
// classes_as_friends2.cpp
// compile with: /EHsc
#include <iostream>

using namespace std;
class YourClass {
friend class YourOtherClass;  // Declare a friend class
public:
   YourClass() : topSecret(0){}
   void printMember() { cout << topSecret << endl; }
private:
   int topSecret;
};

class YourOtherClass {
public:
   void change( YourClass& yc, int x ){yc.topSecret = x;}
};

int main() {
   YourClass yc1;
   YourOtherClass yoc1;
   yc1.printMember();
   yoc1.change( yc1, 5 );
   yc1.printMember();
}
```

## 📣 포인터와 참조자 (Pointer and Reference)

#### 💊 포인터 (Pointer)

* In computer science, a pointer is a programming language object that stores the memory address of another value located in computer memory. A pointer references a location in memory, and obtaining the value stored at that location is known as dereferencing the pointer. As an analogy, a page number in a book's index could be considered a pointer to the corresponding page; dereferencing such a pointer would be done by flipping to the page with the given page number and reading the text found on that page. The actual format and content of a pointer variable is dependent on the underlying computer architecture.

#### 💊 참조자 (Reference)

* In the C++ programming language, a reference is a simple reference datatype that is less powerful but safer than the pointer type inherited from C. The name C++ reference may cause confusion, as in computer science a reference is a general concept datatype, with pointers and C++ references being specific reference datatype implementations. The definition of a reference in C++ is such that it does not need to exist. **It can be implemented as a new name for an existing object** (similar to rename keyword in Ada).

###### 🔑 참조자 특징 (Reference Features)

* It is not possible to refer directly to a reference object after it is defined; any occurrence of its name refers directly to the object it references.

* Once a reference is created, it cannot be later made to reference another object; it cannot be reseated. This is often done with pointers.

* References cannot be null, whereas pointers can; every reference refers to some object, although it may or may not be valid. Note that for this reason, containers of references are not allowed.

* References cannot be uninitialized. Because it is impossible to reinitialize a reference, they must be initialized as soon as they are created. In particular, local and global variables must be initialized where they are defined, and references which are data members of class instances must be initialized in the initializer list of the class's constructor.

###### 📃 참조자 (Reference) Source Code

```C++
#include<iostream> 
using namespace std; 

int main() 
{ 
int x = 10; 

// ref is a reference to x. 
int& ref = x; 

// Value of x is now changed to 20 
ref = 20; 
cout << "x = " << x << endl ; 

// Value of x is now changed to 30 
x = 30; 
cout << "ref = " << ref << endl ; 

return 0; 
} 
```

#### 💊 [참조자와 포인터의 차이점 (Difference Pointer and Reference)](https://www.geeksforgeeks.org/references-in-c/)

<p align="center">
 <img src="https://user-images.githubusercontent.com/20036523/47368932-8f1f0b80-d71d-11e8-9a53-3e2f72aa85d7.jpg" />
</p>

###### 💡 References are less powerful than pointers

1) Once a reference is created, it cannot be later made to reference another object; it cannot be reseated. This is often done with pointers.

2) References cannot be NULL. Pointers are often made NULL to indicate that they are not pointing to any valid thing.

3) A reference must be initialized when declared. There is no such restriction with pointers

* Due to the above limitations, references in C++ cannot be used for implementing data structures like Linked List, Tree, etc. In Java, references don’t have above restrictions, and can be used to implement all data structures. References being more powerful in Java, is the main reason Java doesn’t need pointers.

## 📣 [가상 함수 (Virtual Functions)](https://docs.microsoft.com/en-us/cpp/cpp/virtual-functions?view=vs-2019)

* 가상 함수는 파생 클래스가 안전하게 재정의할 수 있는 함수이다. 만약 상속 관계가 아니라면 가상 함수를 선언할 필요가 없으므로 가상 함수는 상속 계층내에서만 의미가 있으며 파생 클래스에게 재정의 기회를 주기 위해 존재하는 것이라고 할 수 있다. 그러나 **가상 함수를 반드시 재정의해야만 하는 것은 아니다. 기반 클래스의 동작을 그대로 쓰고 싶으면 단순히 상속만 받고 변경할 필요가 있을 때만 재정의하면 된다.** 기반 클래스가 가상 함수를 만드는 이유는 혹시라도 재정의하고 포인터로 호출할 때를 대비한 것이다. **가상 함수는 재정의해도 되는 함수이지 반드시 재정의해야 하는 함수는 아니다.**

###### 📃 가상 함수 (Virtual Functions) Source Code

```C++
// deriv_VirtualFunctions2.cpp
// compile with: /EHsc
#include <iostream>
using namespace std;

class Base {
public:
   virtual void NameOf();   // Virtual function.
   void InvokingClass();   // Nonvirtual function.
};

// Implement the two functions.
void Base::NameOf() {
   cout << "Base::NameOf\n";
}

void Base::InvokingClass() {
   cout << "Invoked by Base\n";
}

class Derived : public Base {
public:
   void NameOf();   // Virtual function.
   void InvokingClass();   // Nonvirtual function.
};

// Implement the two functions.
void Derived::NameOf() {
   cout << "Derived::NameOf\n";
}

void Derived::InvokingClass() {
   cout << "Invoked by Derived\n";
}

int main() {
   // Declare an object of type Derived.
   Derived aDerived;

   // Declare two pointers, one of type Derived * and the other
   //  of type Base *, and initialize them to point to aDerived.
   Derived *pDerived = &aDerived;
   Base    *pBase    = &aDerived;

   // Call the functions.
   pBase->NameOf();           // Call virtual function.
   pBase->InvokingClass();    // Call nonvirtual function.
   pDerived->NameOf();        // Call virtual function.
   pDerived->InvokingClass(); // Call nonvirtual function.
}
```

## 📣 [순수 가상 함수 (Pure Virtual Functions)](https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/)

* 순수 가상 함수(Pure Virtual Function)는 **파생 클래스에서 반드시 재정의해야 하는 함수**이다. 순수 가상 함수는 일반적으로 함수의 동작을 정의하는 본체를 가지지 않으며 따라서 이 상태에서는 호출할 수 없다. 본체가 없다는 뜻으로 함수 선언부의 끝에 `= 0`이라는 표기를 하는데 이는 함수만 있고 코드는 비어 있다는 뜻이다.

###### 📃 순수 가상 함수 (Pure Virtual Functions) Source Code

```C++
#include<iostream> 
using namespace std; 

class Base 
{ 
int x; 
public: 
	virtual void fun() = 0; 
	int getX() { return x; } 
}; 

// This class inherits from Base and implements fun() 
class Derived: public Base 
{ 
	int y; 
public: 
	void fun() { cout << "fun() called"; } 
}; 

int main(void) 
{ 
	Derived d; 
	d.fun(); 
	return 0; 
} 

```
## 📣 [스마트 포인터 (Smart Functor)](http://tcpschool.com/cpp/cpp_template_smartPointer)

* 스마트 포인터(smart pointer)란 포인터처럼 동작하는 클래스 템플릿으로, 사용이 끝난 메모리를 자동으로 해제해 줍니다.

* In computer science, a smart pointer is an abstract data type that simulates a pointer while providing added features, such as automatic memory management or bounds checking. Such features are intended to reduce bugs caused by the misuse of pointers, while retaining efficiency. Smart pointers typically keep track of the memory they point to, and may also be used to manage other resources, such as network connections and file handles. Smart pointers were first popularized in the programming language C++ during the first half of the 1990s as rebuttal to criticisms of C++'s lack of automatic garbage collection.

* Pointer misuse can be a major source of bugs. **Smart pointers prevent most situations of memory leaks by making the memory deallocation automatic.** More generally, they make object destruction automatic: an object controlled by a smart pointer is automatically destroyed (finalized and then deallocated) when the last (or only) owner of an object is destroyed, for example because the owner is a local variable, and execution leaves the variable's scope. Smart pointers also eliminate dangling pointers by postponing destruction until an object is no longer in use.

#### 💊 unique_ptr

* A unique_ptr is a container for a raw pointer, which the unique_ptr is said to own. A unique_ptr explicitly prevents copying of its contained pointer (as would happen with normal assignment), but the ``std::move`` function can be used to transfer ownership of the contained pointer to another unique_ptr. **A unique_ptr cannot be copied because its copy constructor and assignment operators are explicitly deleted.**

* unique_ptr은 하나의 스마트 포인터만이 특정 객체를 소유할 수 있도록, 객체에 소유권 개념을 도입한 스마트 포인터입니다. 또한 이 스마트 포인터는 해당 객체의 소유권을 가지고 있을 때만, 소멸자가 해당 객체를 삭제할 수 있습니다.

###### 📃 unique_ptr Source Code

```C++
std::unique_ptr<int> p1(new int(5));
std::unique_ptr<int> p2 = p1; //Compile error.
std::unique_ptr<int> p3 = std::move(p1); //Transfers ownership. p3 now owns the memory and p1 is set to nullptr.

// unique_ptr<int> ptr03 = ptr01;  // 대입 연산자를 이용한 복사는 오류를 발생시킴.

p3.reset(); //Deletes the memory.
p1.reset(); //Does nothing.
```

#### 💊 shared_ptr

* A shared_ptr is a container for a raw pointer. **It maintains reference counting ownership of its contained pointer in cooperation with all copies of the shared_ptr.** An object referenced by the contained raw pointer will be destroyed when and only when all copies of the shared_ptr have been destroyed. (참조 횟수가 0이 되면 delete 키워드를 사용하여 메모리를 자동으로 해제합니다.)

###### 📃 shared_ptr Source Code ①

```C++
std::shared_ptr<int> p0(new int(5));        // valid, allocates 1 integer and initialize it with value 5
std::shared_ptr<int[]> p1(new int[5]);      // valid, allocates 5 integers
std::shared_ptr<int[]> p2 = p1; //Both now own the memory.

p1.reset(); //Memory still exists, due to p2.
p2.reset(); //Deletes the memory, since no one else owns the memory.
```

###### 📃 shared_ptr Source Code ②

``` C++
shared_ptr<int> ptr01(new int(5)); // int형 shared_ptr인 ptr01을 선언하고 초기화함.
cout << ptr01.use_count() << endl; // 1
auto ptr02(ptr01);                 // 복사 생성자를 이용한 초기화

cout << ptr01.use_count() << endl; // 2
auto ptr03 = ptr01;                // 대입을 통한 초기화

cout << ptr01.use_count() << endl; // 3
```

#### 💊 weak_ptr

* weak_ptr은 하나 이상의 shared_ptr 인스턴스가 소유하는 객체에 대한 접근을 제공하지만, 소유자의 수에는 포함되지 않는 스마트 포인터입니다. 즉, weak_ptr은 shared_ptr 인스턴스 사이의 순환 참조를 제거하기 위해서 사용됩니다.

* A weak_ptr is a container for a raw pointer. It is created as a copy of a shared_ptr. The existence or destruction of weak_ptr copies of a shared_ptr have no effect on the shared_ptr or its other copies. After all copies of a shared_ptr have been destroyed, all weak_ptr copies become empty.

* Because the implementation of shared_ptr uses reference counting, circular references are potentially a problem. A circular shared_ptr chain can be broken by changing the code so that one of the references is a weak_ptr.

###### 📃 weak_ptr Source Code

```C++
std::shared_ptr<int> p1 = std::make_shared<int>(5);
std::weak_ptr<int> wp1 {p1}; //p1 owns the memory.

{
    std::shared_ptr<int> p2 = wp1.lock(); //Now p1 and p2 own the memory.
    // p2 is initialized from a weak pointer, so 
    // you have to check if the memory still exists!
    if (p2) {
        do_something_with(p2);
    }
}
//p2 is destroyed. Memory is owned by p1.

p1.reset(); // Delete the memory.

std::shared_ptr<int> p3 = wp1.lock(); 
//Memory is gone, so we get an empty shared_ptr.
if (p3) { // code will not execute
    action_that_needs_a_live_pointer(p3);
}
```

## 📣 [Const](https://docs.microsoft.com/en-us/cpp/cpp/const-cpp?view=vs-2019)

* Const는 C, C++에서 변수의 값이 바뀌는 것을 방지하기 위한 한정사이다. 즉, 이 한정사가 붙은 변수는 상수로 취급된다.

###### 🔑 Const Value

* The const keyword specifies that a variable's value is constant and tells the compiler to prevent the programmer from modifying it.

```C++
// constant_values1.cpp
int main() {
   const int i = 5;
   i = 10;   // C3892
   i++;   // C2105
}
```

###### 🔑 Const Member Functions

* Declaring a member function with the const keyword specifies that the function is a "read-only" function that does not modify the object for which it is called. A constant member function cannot modify any non-static data members or call any member functions that aren't constant.To declare a constant member function, place the const keyword after the closing parenthesis of the argument list. The const keyword is required in both the declaration and the definition.

* 멤버변수에 저장 된 값을 변경하지 않겠다는 선언이다. 따라서 `const` 선언이 추가된 멤버함수 내에서 멤버변수의 값을 변경하는 코드가 삽입되면, 컴파일 에러가 발생한다.

```C++
// constant_member_function.cpp
class Date
{
      public:
          Date( int mn, int dy, int yr );
          int getMonth() const;     // A read-only function
          void setMonth( int mn );   // A write function; can't be const
       private:
          int mon th;
};

int Date::getMonth() const
{
   return month;        // Doesn't modify anything
}
void Date::setMonth( int mn )
{
   month = mn;          // Modifies data member
}
int main()
{
   Date MyDate( 7, 4, 1998 );
   const Date BirthDate( 1, 18, 1953 );
   MyDate.setMonth( 4 );    // Okay
   BirthDate.getMonth();    // Okay
   BirthDate.setMonth( 4 ); // C2662 Error
}
```

## 📣 [Difference Between Class and Struct in C++](https://blogs.mentor.com/colinwalls/blog/2014/06/02/struct-vs-class-in-c/)

* The only difference between a struct and class in C++ is the default accessibility of member variables and methods. In a struct they are public; in a class they are private.

* C++에서의 Struct와 Class의 유일한 차이점은 접근제어 지시자(Access Modifier)를 선언하지 않을 때 Class는 `Private`으로 Struct는 `Public`으로 선언된다.

* 키워드 Struct를 이용해서 정의한 구조체(클래스)에 선언된 변수와 함수에 별도의 접근제어 지시자(Access Modifier)를 선언하지 않으면, 모든 변수와 함수는 `Public`으로 선언된다.

* 키워드 Class를 이용해서 정의한 클래스에 선언된 변수와 함수에 별도의 접근제어 지시자(Access Modifier)를 선언하지 않으면, 모든 변수와 함수는 `Private`으로 선언된다.

## 📣 [Difference Between new/delete and malloc/free in C++](https://stackoverflow.com/questions/240212/what-is-the-difference-between-new-delete-and-malloc-free)

* The most relevant difference is that the `new` operator allocates memory then calls the constructor, and `delete` calls the destructor then deallocates the memory.

* * `new` 연산자는 자유 기억 공간(free store)이라고 불리는 메모리 공간(memory pool)에 객체를 위한 메모리를 할당받습니다. 또한, `new` 연산자를 통해 할당받은 메모리는 따로 이름이 없으므로 해당 포인터로만 접근할 수 있게 됩니다.

###### 🔑 New/Delete

* Allocate/release memory
* Memory allocated from 'Free Store'
* Returns a fully typed pointer.
* new (standard version) never returns a NULL (will throw on failure)
* Are called with Type-ID (compiler calculates the size)
* Has a version explicitly to handle arrays.
* Reallocating (to get more space) not handled intuitively (because of copy constructor).
* Whether they call malloc/free is implementation defined.
* Can add a new memory allocator to deal with low memory (set_new_handler)
* operator new/delete can be overridden legally
* constructor/destructor used to initialize/destroy the object

###### 🔑 Malloc/Free

* Allocates/release memory
* Memory allocated from 'Heap'
* Returns a void*
* Returns NULL on failure
* Must specify the size required in bytes.
* Allocating array requires manual calculation of space.
* Reallocating larger chunk of memory simple (No copy constructor to worry about)
* They will NOT call new/delete
* No way to splice user code into the allocation sequence to help with low memory.
* malloc/free can NOT be overridden legally

###### 🔑 Malloc과 Free와의 관계

* 표준 C++이 C 표준 라이브러리를 포함하기 때문에, C 동적 메모리 할당 루틴들인 malloc, realloc 그리고 free도 C++ 프로그래머에게 사용될 수 있다. 이것들은 객체 초기화와 소멸을 수행하지 않기 때문에 이러한 루틴들의 사용은 대부분의 경우에 추천되지 않는다. 사실 new와 delete는 객체 초기화를 직접 수행하는 것을 피하기 위해 C++의 첫 번째 버전에서 도입되었다. </br></br>realloc으로 할당된 배열을 키우거나 줄이는 C 루틴들과 대조적으로, new[]에 의해 할당된 메모리 버퍼의 크기를 변경하는 것은 불가능하다. C++ 표준 라이브러리는 대신 자신의 std::vector 템플릿 클래스에서 확장되거나 줄여질 수 있는 동적 배열을 제공한다. </br></br>C++ 표준은 new/delete와 C 메모리 할당 루틴들 사이에서 특정한 관계를 명시하지 않지만, new와 delete는 일반적으로 malloc과 free의 래퍼로서 구현된다. 두 집단의 연산자들을 혼합하는 것은 정의되지 않은 행동을 유발하며 실제로 락의 릴리즈의 실패로 인한 데드락 같은 비극적인 결과를 초래한다.

## 🚀 REFERENCE

✈️ [C/C++ REFERENCE URL](https://github.com/ChangYeop-Yang/Study-C/issues/2)

## ★ Developer Information

|:rocket: Github QR Code|:pencil: Naver-Blog QR Code|:eyeglasses: Linked-In QR Code|
|:---------------------:|:-------------------------:|:----------------------------:|
|![](https://user-images.githubusercontent.com/20036523/50044128-60406880-00c2-11e9-8d57-ea1cb8e6b2a7.jpg)|![](https://user-images.githubusercontent.com/20036523/50044131-60d8ff00-00c2-11e9-818c-cf5ad97dc76e.jpg)|![](https://user-images.githubusercontent.com/20036523/50044130-60d8ff00-00c2-11e9-991a-107bffa2bf57.jpg)|
