# JAVA 8 MASTER NOTES (PART 1): Lambdas, Functional Interfaces, Default/Static Methods & Anonymous Inner Classes

**Compiled Source Videos (Engineering Digest — Videos 1 to 11):**

1. [What's the Purpose of Java 8?](http://www.youtube.com/watch?v=IrBoPow7gdg)
2. [Java 8 Features Overview](http://www.youtube.com/watch?v=KaBEfoXxLIY)
3. [Lambda Expression in Java 8 (Step-by-Step)](http://www.youtube.com/watch?v=u8DpSS8Skv0)
4. [Functional Interface in Java 8](http://www.youtube.com/watch?v=WXIoT8z4bNY)
5. [Inheritance in Functional Interface](http://www.youtube.com/watch?v=m3inFZSQO54)
6. [Default Methods in Interface](http://www.youtube.com/watch?v=b4paTLGsJjE)
7. [Static Method in Interface (Main Method Inside Interface)](http://www.youtube.com/watch?v=nGGEh0aHLwQ)
8. [How to Use Lambda Expression in Java 8?](http://www.youtube.com/watch?v=ZmWBxXc0WSA)
9. [How to Create Thread Using Lambda Expression](http://www.youtube.com/watch?v=Mjb5cTzPzwg)
10. [How to Use Comparator Using Lambda Expression](http://www.youtube.com/watch?v=ID8a5j1Rbq8)
11. [Difference Between Lambda Expression & Anonymous Inner Class](http://www.youtube.com/watch?v=AIsjRP7gnQA)

---

## 1. Why Did Java 8 Come? (The 3 Core Intentions & Features)

**Sources:** [Purpose of Java 8](http://www.youtube.com/watch?v=IrBoPow7gdg) | [Java 8 Features](http://www.youtube.com/watch?v=KaBEfoXxLIY)

Java 7 tak har chote kaam ke liye lambi-chaudi classes aur boilerplate code likhna padta tha. Java 8 laane ke peeche **sirf 3 main intentions** the:

1. **Concise & Minimal Code:** Code chota, readable, aur clean hona chahiye—faltu ki kahani (boilerplate) nahi likhni pade.
2. **To Utilize Functional Programming Benefits:** Java ek Object-Oriented language hai, usmein Functional Programming (functions ko as a variable/argument pass karna) missing thi.
3. **To Enable Parallel Programming:** Aajkal multi-core (octa-core) processors aa gaye hain, unhe efficiently utilize karne ke liye Parallel Streams aur functional APIs layi gayin.

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Agar koi poochta hai ki kya Java 8 ke baad Java ek Functional Programming language ban gayi hai? **Nahi!** Java is STILL an Object-Oriented Programming Language—humne bas Functional Programming ke benefits ko OOP ke andar adopt kiya hai."*

### Major Features Introduced in Java 8 (Checklist)

* **Lambda Expressions** & **Functional Interfaces** (`@FunctionalInterface`)
* **Stream API** (Collections aur Arrays par bulk operations ke liye)
* **`default` and `static` Methods inside Interfaces**
* **Method References & Constructor References (`::`)**
* **`Optional<T>` Class** (`NullPointerException` handle karne ke liye)
* **New Date & Time API** (`java.time` package)
* **Built-in Base64 Encode & Decode** (`java.util.Base64` — ab external library nahi chahiye)

> **🎯 How to Articulate in an Interview (English):**
> *"Java 8 was introduced with three primary goals: reducing boilerplate through concise syntax, bringing functional programming paradigms like first-class functions into an OOP language, and enabling easy parallel data processing on multi-core CPUs via the Stream API."*

---

## 2. Lambda Expressions: Step-by-Step Conversion & Type Inference

**Source:** [Lambda Expression in Java 8](http://www.youtube.com/watch?v=u8DpSS8Skv0)

### 2.1 What is a Lambda Expression?

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Lambda Expression kuch nahi hai bhai, sirf ek **Anonymous Function** hai—bole toh ek aisa function jiska **koi Naam nahi hai**, jiska **koi Return Type nahi hai**, aur jiska **koi Access Modifier (`public`/`private`) nahi hai**!"*

### 2.2 The 4 Mechanical Steps to Convert ANY Method into a Lambda

Kisi bhi normal Java method ko Lambda Expression banane ke 4 steps hain:

1. **Remove Modifier** (`private` / `public` hata do)
2. **Remove Return Type** (`void` / `int` / `String` hata do)
3. **Remove Method Name** (naam hata do)
4. **Place Arrow (`->`)** between Parameters `(...)` and Body `{...}`

```java
// Example 1: No parameters, void return
// Normal Method:
private void sayHello() {
    System.out.println("Hello");
}
// Raw Lambda:
() -> { System.out.println("Hello"); }

// Example 2: Two parameters, void return
// Normal Method:
private void add(int a, int b) {
    System.out.println(a + b);
}
// Raw Lambda:
(int a, int b) -> { System.out.println(a + b); }

// Example 3: One parameter, returns int
// Normal Method:
private int getStringLength(String str) {
    return str.length();
}
// Raw Lambda:
(String str) -> { return str.length(); }

```

---

### 2.3 The 4 Rules to Make a Lambda Expression Concise ("Sundar Banane Ke 4 Tareeke")

1. **Rule 1 — Single Statement in Body $\rightarrow$ Remove Curly Brackets `{}`:**
Agar body mein sirf 1 line hai, toh `{}` aur andar ka `;` hata do:
`() -> System.out.println("Hello");`
2. **Rule 2 — Use Type Inference $\rightarrow$ Remove Parameter Data Types:**
`(int a, int b)` likhne ki zarurat nahi hai, compiler khud samajh jayega:
`(a, b) -> System.out.println(a + b);`
3. **Rule 3 — No `return` Keyword (When `{}` are Removed):**
Agar tumne `{}` hata diye hain aur expression kuch return kar रहा hai, toh **`return` keyword likhna mana hai** (compiler khud samajh jaata hai ki value return ho rahi hai):
`(str) -> str.length()` *(Note: Agar `{}` lagaye hain, toh `return` aur `;` dono lagane padenge: `str -> { return str.length(); }`)*
4. **Rule 4 — Single Parameter $\rightarrow$ Remove Small Brackets `()`:**
Agar sirf **1 single parameter** hai (aur uska type nahi likha hai), toh `()` bhi hata sakte ho:
`str -> str.length()` *(Note: `0` parameters `()` ya `2+` parameters `(a, b)` par brackets hatana mana hai!)*

---

### 2.4 Deep-Dive: When to Write `(int a, int b) -> {}` vs. `(a, b) -> {}`?

Tumhara specific doubt: **Compiler ko bina type likhe kaise pata chalta hai, aur kab type likhna zaroori hota hai?**

* **How Compiler Infers Types (Target Typing):**
Har Lambda Expression kisi na kisi **Functional Interface** ke variable mein assign hota hai ya method argument mein pass hota hai. Compiler us Functional Interface ke इकलौते abstract method ka signature dekhta hai (jaise `int compare(Integer a, Integer b)`), aur wahan se apne aap `a` aur `b` ka type `Integer` maan leta hai.
* **Rule A — 99% Cases Mein `(a, b) -> {}` Likho:**
Jab Functional Interface ka type clear ho (jaise `Comparator<Integer>`, `Predicate<String>`, ya Streams mein `.filter(x -> x > 5)`), tab hamesha bina type ke `(a, b) -> ...` likho.
* **Rule B — Kab `(int a, int b) -> {}` Likhna Majboori Hai? (Method Overloading Ambiguity):**
Maan lo kisi class mein do overloaded methods hain:
```java
interface IntOperation { int calc(int a, int b); }
interface StringOperation { String calc(String a, String b); }

void process(IntOperation op) { ... }
void process(StringOperation op) { ... }

```


Agar tum `process((a, b) -> a + b);` call karoge, toh **`+` operator `int` aur `String` dono par chalta hai**! Compiler confuse ho jayega ki `IntOperation` wala `process` chalana hai ya `StringOperation` wala (**Compile-Time Error: `reference to process is ambiguous**`).
* **Fix:** Yahan tumhe explicitly type batana padega:
`process((int a, int b) -> a + b);` ya `process((String a, String b) -> a + b);`


* **Rule C — Illegal Syntax (Half-Typed Parameters):**
Ya toh **saare** parameters ka type likho, ya **kisi ka mat likho**! Ek ka likhna aur doosre ka chhodna compile error hai:
* `(int a, int b) -> a + b` ✅ *(Valid)*
* `(a, b) -> a + b` ✅ *(Valid)*
* `(var a, var b) -> a + b` ✅ *(Valid in Java 11+ — useful only if you want to put an annotation like `@NonNull var a`)*
* `(int a, b) -> a + b` ❌ **Compile-Time Error!** *(Cannot mix explicit and inferred types)*
* `int a -> a + 1` ❌ **Compile-Time Error!** *(Agar explicit type `int` likha hai, toh single parameter par bhi small brackets `(int a) -> a + 1` lagana mandatory hai!)*



> **🎯 How to Articulate in an Interview (English):**
> *"A Lambda expression is an anonymous function that implements a Functional Interface's single abstract method. Through Target Typing, the Java compiler automatically infers parameter types from the target Functional Interface signature, making explicit types like `(int a, int b)` unnecessary unless resolving ambiguity between overloaded methods."*

---

## 3. Functional Interfaces & Inheritance Rules

**Sources:** [Functional Interface in Java 8](http://www.youtube.com/watch?v=WXIoT8z4bNY) | [Inheritance in Functional Interface](http://www.youtube.com/watch?v=m3inFZSQO54)

### 3.1 Exact Definition of a Functional Interface

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Functional Interface kya hota hai? Woh interface jismein **EXACTLY ONE Abstract Method (SAM — Single Abstract Method)** ho! Uske andar **`default` aur `static` methods kitne bhi ho sakte hain (0, 1, 10, ya 100)**—restriction sirf aur sirf Abstract Method par hai ki woh **ek aur sirf ek** hona chahiye!"*

```java
@FunctionalInterface
public interface MyInterface {
    void sayHello(); // Exactly 1 Abstract Method (public abstract is implicit)

    // Any number of default and static methods are 100% allowed:
    default void sayBye() { System.out.println("Bye"); }
    static void sayOk()   { System.out.println("OK"); }
}

```

*(Quiz Edge Case: Kya ek bilkul khali `interface Empty {}` Functional Interface hota hai? **Nahi!** Kyunki usmein `0` abstract methods hain, jabki **कम से कम और ज़्यादा से ज़्यादा 1** abstract method hona zaroori hai).*

---

### 3.2 Why Use `@FunctionalInterface` Annotation If It Is Optional?

Agar tum `@FunctionalInterface` annotation **na** bhi lagao, aur interface mein 1 abstract method ho, tab bhi woh 100% valid Functional Interface hai aur uspar Lambda chalega. **Toh phir `@FunctionalInterface` lagate kyun hain?**

* **The Team Safety Reason:** Maan lo tumne `MyInterface` banaya (bina annotation ke) aur poore project mein 50 jagah uspar Lambda Expressions likh diye. Kal ko koi naya developer aaya aur usne `MyInterface` mein ek doosra abstract method `void sayBye();` add kar diya. Ab `MyInterface` Functional Interface nahi raha, aur tumhare project ke **saare 50 Lambda Expressions ek sath phat jayenge (compile error denge)**!
* `@FunctionalInterface` lagane se **compiler usi waqt interface par lock laga deta hai**—jaise hi koi doosra abstract method add karne ki koshish karega, compiler wahin par `Invalid '@FunctionalInterface' annotation` error de dega!

---

### 3.3 The 4 Inheritance Cases in Functional Interfaces (`Child extends Parent`)

Maan lo `Parent` ek Functional Interface hai jismein `void sayHello();` hai, aur `interface Child extends Parent` banaya gaya:

| Case | What `Child` Interface Contains | Is `Child` a Valid `@FunctionalInterface`? | Exact Reason |
| --- | --- | --- | --- |
| **Case 1** | `Child` is completely **empty** `{}` | **YES ✅** | `Child` inherits the 1 abstract method `sayHello()` from `Parent`. Total = 1. |
| **Case 2** | `Child` re-declares the **exact same** `void sayHello();` | **YES ✅** | Overriding/re-declaring the same signature keeps total abstract methods = 1. |
| **Case 3** | `Child` declares a **new** abstract method `void sayBye();` | **NO ❌ (Compile Error with `@FunctionalInterface`)** | `Child` now has **2 abstract methods** (`sayHello` + `sayBye`). *(Works as a normal interface if you remove `@FunctionalInterface`).* |
| **Case 4** | `Child` adds `default` or `static` methods | **YES ✅** | No limit on `default` / `static` methods. Total abstract methods = 1. |

> **🎯 How to Articulate in an Interview (English):**
> *"A Functional Interface is an interface containing exactly one abstract method, though it can have any number of default and static methods. While the `@FunctionalInterface` annotation is optional, it enforces a compile-time check that prevents developers from accidentally adding a second abstract method and breaking existing lambda implementations."*

---

## 4. `default` & `static` Methods Inside Interfaces (Java 8)

**Sources:** [Default Methods in Interface](http://www.youtube.com/watch?v=b4paTLGsJjE) | [Static Method in Interface](http://www.youtube.com/watch?v=nGGEh0aHLwQ)

Java 1.7 tak interface ke andar sirf `public abstract` methods (bina body wale) allowed the. Java 8 ne interface ke andar **concrete methods (body wale methods)** likhne ke 2 tareeke diye: **`default` methods** aur **`static` methods**.

---

### 4.1 `default` Methods & The Diamond Problem (Multiple Inheritance)

* **Syntax:** Interface ke andar `default` keyword laga kar method ki body likhi jaati hai:
```java
interface Parent {
    default void sayHello() {
        System.out.println("Hello from Parent");
    }
}

```


* **How Child Classes Use It:** Jo class `Parent` ko implement karti hai (`class Child implements Parent`), uske paas `sayHello()` **automatically inherit** ho jaata hai—woh chahe toh use as-is call kare, ya apni class mein `public void sayHello()` likh kar **override** kar le.

#### The Classic Interview Trap: Multiple Inheritance Conflict (Diamond Problem)

Maan lo do interfaces `A` aur `B` dono mein **same signature** wala `default void sayHello()` method hai, aur `MyClass` dono ko implement karti hai:

```java
interface A {
    default void sayHello() { System.out.println("A says Hello"); }
}
interface B {
    default void sayHello() { System.out.println("B says Hello"); }
}

public class MyClass implements A, B {
    // COMPILER ERROR if you leave this empty!
    // "class MyClass inherits unrelated defaults for sayHello() from types A and B"

    // MANDATORY FIX: Override sayHello() in MyClass!
    @Override
    public void sayHello() {
        // Option 1: Write your own custom logic
        System.out.println("My own implementation");

        // Option 2: Explicitly call A's or B's default method using InterfaceName.super:
        A.super.sayHello();
        B.super.sayHello();
    }
}

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Jab do interfaces mein same naam ka `default` method ho aur ek class dono ko implement kare, toh compiler confuse ho jaata hai ki `A` wala chalana hai ya `B` wala. Solution: Apni class mein us method ko **override karna hi padega**, aur agar `A` ya `B` ka logic call karna hai toh **`A.super.sayHello()`** ya **`B.super.sayHello()`** likhna padega!"*

---

### 4.2 `static` Methods Inside Interfaces & Running `main()` in an Interface!

```java
public interface A {
    static void sayHello() {
        System.out.println("Hello from Static Interface Method!");
    }

    // YES! Since Java 8, you can write and run main() directly inside an Interface!
    public static void main(String[] args) {
        System.out.println("Running main() directly from an Interface!");
        A.sayHello();
    }
}

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Interface ke andar likhe `static` method ko call karne ka is duniya mein **SIRF AUR SIRF EK HI TAREEKA HAI**—seedha **Interface ke Naam se (`A.sayHello()`)**! Na tum use implementing class ke object (`obj.sayHello()`) se call kar sakte ho, aur na hi implementing class ke naam (`MyClass.sayHello()`) se! Kyunki interface ke `static` methods implementing class ko **dikhte hi nahi hain (inherit hi nahi hote)**!"*

* **Important Consequence (Not Overriding):**
Kyunki `A.sayHello()` `MyClass implements A` ko dikhta hi nahi hai, agar tum `MyClass` ke andar `void sayHello()` (ya `private void sayHello()`) banaoge, toh woh **overriding NAHI hai**—woh `MyClass` ka apna ek bilkul naya independent method maana jayega (`@Override` lagane par compile error aayega).

| Feature | `default` Method in Interface | `static` Method in Interface |
| --- | --- | --- |
| **Inherited by Implementing Class?** | **YES** (Visible to child class & objects) | **NO** (Completely invisible to implementing class) |
| **Can It Be Overridden?** | **YES** | **NO** |
| **How to Call It?** | `obj.method()` (or `InterfaceName.super.method()` inside child) | **ONLY via `InterfaceName.method()**` |

> **🎯 How to Articulate in an Interview (English):**
> *"`default` methods allow adding new functionality to existing interfaces without breaking implementing classes; if a class implements two interfaces with identical `default` methods, it must override the method and can disambiguate using `InterfaceName.super.methodName()`. Conversely, interface `static` methods belong strictly to the interface, are not inherited by implementing classes, cannot be overridden, and can only be invoked via `InterfaceName.methodName()`."*

---

## 5. How to Use Lambda Expressions in Real Code (Custom Interfaces, `Runnable` & `Comparator`)

**Sources:** [How to Use Lambda Expression](http://www.youtube.com/watch?v=ZmWBxXc0WSA) | [Create Thread Using Lambda](http://www.youtube.com/watch?v=Mjb5cTzPzwg) | [Comparator Using Lambda](http://www.youtube.com/watch?v=ID8a5j1Rbq8)

### 5.1 The Core Connection: Functional Interface = Data Type for Lambda

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Jaise hum likhte hain `int a = 1;` ya `String name = "Vipul";`, bilkul waise hi **Functional Interface acts as the Data Type for a Lambda Expression!** Aur woh Lambda Expression kuch aur nahi, balki us Functional Interface ke andar pade **इकलौते Abstract Method ka implementation** hota hai! Isliye ab alag se `SoftwareEngineer implements Employee` class banane ki koi zarurat nahi hai—use delete maar do!"*

```java
@FunctionalInterface
public interface Employee {
    String getName(); // Single Abstract Method
}

public class Main {
    public static void main(String[] args) {
        // OLD WAY (Required a separate class SoftwareEngineer implements Employee):
        // Employee se = new SoftwareEngineer();

        // JAVA 8 LAMBDA WAY (Zero extra classes needed!):
        Employee se = () -> "Software Engineer";
        System.out.println(se.getName()); // Prints: Software Engineer

        Employee editor = () -> "Editor";
        System.out.println(editor.getName()); // Prints: Editor
    }
}

```

*(Why 2 Abstract Methods Break Lambda: Agar `Employee` mein `getName()` ke saath `getAge()` bhi abstract hota, toh `Employee se = () -> "Software Engineer";` likhte waqt compiler ko pata hi nahi chalta ki yeh lambda `getName()` ki body hai ya `getAge()` ki!)*

---

### 5.2 Creating a Thread Using `Runnable` + Lambda

`java.lang.Runnable` ek pre-defined Functional Interface hai jismein sirf 1 abstract method hota hai: **`public void run();`** (takes no arguments, returns `void`).

```java
// Directly implementing Runnable's run() method using a Lambda:
Runnable runnable = () -> {
    for (int i = 1; i <= 10; i++) {
        System.out.println("Hello " + i);
    }
};

Thread childThread = new Thread(runnable);
childThread.start();

```

---

### 5.3 Custom Sorting Using `Comparator<T>` + Lambda

`java.util.Comparator<T>` ek Functional Interface hai jismein sirf 1 abstract method hota hai: **`int compare(T a, T b);`**.

* **Vipul's Quick Rule:**
* **Ascending Order (Natural):** `(a, b) -> a - b` *(or overflow-safe `Integer.compare(a, b)`)*
* **Descending Order (Reverse):** `(a, b) -> b - a` *(or overflow-safe `Integer.compare(b, a)`)*



```java
// 1. Sorting an ArrayList in Descending Order:
List<Integer> list = new ArrayList<>(List.of(2, 6, 0, 99, 6));
Collections.sort(list, (a, b) -> b - a); // [99, 6, 6, 2, 0]

// 2. TreeSet in Descending Order (Pass Comparator Lambda in Constructor):
Set<Integer> set = new TreeSet<>((a, b) -> b - a);

// 3. TreeMap Sorted by Keys in Descending Order:
Map<Integer, String> map = new TreeMap<>((a, b) -> b - a);

// 4. Sorting a List of Custom Objects (Student{id, name}) by ID Descending:
List<Student> students = new ArrayList<>();
students.add(new Student(2, "Vipul"));
students.add(new Student(3, "Raj"));
students.add(new Student(33, "Akshit"));

// Custom objects don't have natural ordering unless specified; pass Lambda Comparator:
Collections.sort(students, (a, b) -> b.id - a.id); // Sorts: 33 (Akshit), 3 (Raj), 2 (Vipul)

```

---

## 6. Lambda Expression vs. Anonymous Inner Class (And `effectively final` + `this` Trap)

**Source:** [Difference Between Lambda & Anonymous Inner Class](http://www.youtube.com/watch?v=AIsjRP7gnQA)

### 6.1 When MUST You Use an Anonymous Inner Class Instead of a Lambda?

Agar kisi Interface mein **1 se zyada abstract methods** hain (jaise `Employee` mein `getSalary()` **aur** `getDesignation()` dono hain), toh woh Functional Interface **nahi** hai—wahan Lambda Expression kaam **nahi** karega!
Agar tumhe bina alag file/class banaye haatho-haath uska implementation likhna hai, toh **Anonymous Inner Class** use hoti hai:

```java
public interface Employee {
    String getSalary();
    String getDesignation();
}

// Anonymous Inner Class (Instantiating an unnamed class that implements Employee on the fly):
Employee emp = new Employee() {
    @Override
    public String getSalary() { return "100"; }

    @Override
    public String getDesignation() { return "Software Engineer"; }
};

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Kya Lambda Expression Anonymous Inner Class ka 100% replacement hai? **Nahi!** Anonymous Inner Class zyada powerful hai—woh kisi bhi Interface (chahe usmein 10 abstract methods hon) ya Abstract/Concrete Class ko extend kar sakti hai, jabki **Lambda Expression SIRF AUR SIRF Functional Interface (1 abstract method) ke liye hi chal sakta hai**!"*

---

### 6.2 Variable Capture Rule: Local Variables Must Be `final` or `Effectively Final`

Agar kisi method ke andar ek **local variable** (`int a = 2;`) bana hai aur tum use Lambda Expression (ya Anonymous Inner Class) ke andar use karte ho, toh woh local variable automatically **`final` (Effectively Final)** maan liya jaata hai—tum uski value modify **nahi** kar sakte!
Lekin class ke **Instance Variable (Field)** ko Lambda ke andar bilkul modify kar sakte ho:

```java
public class Main {
    int instanceVar = 2; // Instance variable (Field of Main class)

    private void doSomething() {
        int localVar = 2; // Local variable of doSomething() method

        Employee emp = () -> {
            instanceVar = 3; // ✅ ALLOWED! Instance variables can be modified freely.
            // localVar = 3; // ❌ COMPILE ERROR: "Variable used in lambda expression should be final or effectively final"
            return "100";
        };
    }
}

```

*(Why? Local variables Stack par rehte hain aur method khatam hote hi destroy ho jaate hain, jabki Lambda object Heap par baad mein kisi doosre thread par bhi chal sakta hai—isliye Java local variable ki copy capture karta hai aur use mutate karne se rokta hai).*

---

### 6.3 The `this` Keyword & Scope Difference (Lambda = Function vs. Anonymous Class = Actual Class)

```java
public class Main {
    int x = 10; // Instance variable of Main class

    private void doSomething() {
        // 1. Inside a LAMBDA EXPRESSION:
        // Lambda is just a FUNCTION (not a separate class), so any variable declared inside it is a LOCAL variable!
        Employee lambdaEmp = () -> {
            int x = 20; // This 'x' is just a LOCAL variable of the function, NOT an instance field!
            System.out.println(this.x); // Prints 10! Because 'this' refers to the outer 'Main' class object!
            return "100";
        };

        // 2. Inside an ANONYMOUS INNER CLASS:
        // Anonymous Inner Class IS A SEPARATE CLASS with its own scope and instance variables!
        Employee anonEmp = new Employee() {
            int x = 20; // This 'x' is an INSTANCE VARIABLE of the Anonymous Inner Class itself!

            @Override
            public String getSalary() {
                System.out.println(this.x); // Prints 20! Because 'this' refers to the Anonymous Inner Class object!
                // (To access Main's x here, you would have to write Main.this.x)
                return "100";
            }
        };
    }
}

```

### Summary Comparison Table: Lambda Expression vs. Anonymous Inner Class

| Feature | Lambda Expression | Anonymous Inner Class |
| --- | --- | --- |
| **Target Type** | **ONLY Functional Interfaces** (1 Abstract Method) | Any Interface (1 or multiple methods), Abstract Class, or Concrete Class |
| **Nature & Scope** | It is an **anonymous function**; shares the enclosing class's scope | It is a **separate unnamed class**; creates its own class scope |
| **Can Have Instance Variables?** | **NO** (Variables declared inside `{}` are local variables) | **YES** (Variables declared inside class body are instance fields) |
| **Meaning of `this**` | Refers to the **enclosing outer class** instance | Refers to the **anonymous inner class** instance itself |
| **Compilation / Memory** | Uses `invokedynamic` (No extra `.class` file generated $\rightarrow$ smaller JAR) | Generates a separate `Main$1.class` file on disk |

> **🎯 How to Articulate in an Interview (English):**
> *"Unlike an Anonymous Inner Class—which compiles into a separate `.class` file, can implement interfaces with multiple abstract methods, can hold state via instance variables, and binds `this` to its own instance—a Lambda Expression is a stateless function restricted strictly to Functional Interfaces, shares the lexical scope of the enclosing method where `this` refers to the outer class, and requires any captured local variables to be effectively final."*

---