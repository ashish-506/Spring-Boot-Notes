# JAVA 8 MASTER NOTES: `Predicate<T>` Functional Interface (Complete Breakdown + Progressive Code Lab)

**Source Video:** [Mastering Java 8 Predicates: AND, OR, and isEqual Explained with Examples (Engineering Digest)](http://www.youtube.com/watch?v=CuuR6NKgnss)

---

## 1. The Core Mental Model: `Predicate<T>` Akhir Hai Kya?

Agar video dekhte waqt code thoda bikhra hua laga, toh sabse pehle is **1-Line Mental Model** ko lock karo:

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"अगर आपसे कल को कोई चलते रास्ते में पूछता है कि भाई बताओ `Predicate` क्या होता है? तो आपको बोलना है: **`Predicate` एक Boolean-Valued Function होता है जो एक कंडीशन (Condition) को एक वेरिएबल (Variable) के अंदर होल्ड करके रखता है!** बचपन में हम `if (x > 100000)` लिखते थे, अब हम उस कंडीशन को एक `Predicate` वेरिएबल में स्टोर कर लेते हैं ताकि पूरे प्रोजेक्ट में कहीं भी उसे पास और रीयूज कर सकें!"* [[01:49](https://www.youtube.com/watch?v=CuuR6NKgnss&t=109)]

### Internal Blueprint of `java.util.function.Predicate<T>`

`Predicate<T>` Java 8 mein introduce hua ek pre-defined **Functional Interface** hai [[00:35](https://www.youtube.com/watch?v=CuuR6NKgnss&t=35)]. Iske andar kul **5 methods** hote hain (1 Abstract + 3 Default + 1 Static) [[01:20](https://www.youtube.com/watch?v=CuuR6NKgnss&t=80)]:

```java
package java.util.function;

@FunctionalInterface
public interface Predicate<T> {

    // 1. THE SINGLE ABSTRACT METHOD (Jiski body hum Lambda se likhte hain):
    boolean test(T t);

    // 2. THREE DEFAULT METHODS (Do ya zyada conditions ko jodne/palatne ke liye):
    default Predicate<T> and(Predicate<? super T> other) { ... }
    default Predicate<T> or(Predicate<? super T> other)  { ... }
    default Predicate<T> negate()                        { ... }

    // 3. ONE STATIC METHOD (Equality check karne ke liye):
    static <T> Predicate<T> isEqual(Object targetRef)    { ... }
}

```

* **Input (`<T>`):** Kisi bhi type ka **1 argument** leta hai (`Integer`, `String`, `Student`, `Employee`, etc.) [[02:41](https://www.youtube.com/watch?v=CuuR6NKgnss&t=161)].
* **Output (`boolean`):** Hamesha **`true` ya `false**` return karta hai (`test(T t)` method ke zariye) [[02:48](https://www.youtube.com/watch?v=CuuR6NKgnss&t=168)].

---

## 2. The 2-Step Execution Flow (Why People Get Confused in Code)

Jab bhi tum `Predicate` ka code likho, dimaag mein hamesha **2 alag steps** rakhna:

1. **Step 1 — Rule Banana (Defining the Lambda):**
Jab tum `Predicate<Integer> isEven = x -> x % 2 == 0;` likhte ho, **tab code chalta nahi hai!** Tumne bas `test(T t)` method ki body likh kar `isEven` variable mein save ki hai [[02:26](https://www.youtube.com/watch?v=CuuR6NKgnss&t=146)].
2. **Step 2 — Rule Chalana (Calling `.test(input)`):**
Jab tum `isEven.test(4)` call karte ho, **tab** `x` ki jagah `4` jaata hai, `4 % 2 == 0` check hota hai, aur `true` return hota hai [[08:37](https://www.youtube.com/watch?v=CuuR6NKgnss&t=517)]!

---

### Example 1: Integer Condition (Aam Zindagi vs. Java 8 `Predicate` Zindagi)

```java
import java.util.function.Predicate;

public class PredicateDemo {
    // AAM ZINDAGI (Pre-Java 8): Alag se poora method likhna padta tha [00:05:10]
    private static boolean isSalaryGreaterThanOneLakh(int x) {
        return x > 100000;
    }

    public static void main(String[] args) {
        // JAVA 8 ZINDAGI: Condition ko seedha ek Variable mein store kar liya! [00:03:07]
        // Yahan 'x -> x > 100000' Predicate ke इकलौते abstract method 'boolean test(Integer x)' ki body hai:
        Predicate<Integer> salaryGreaterThanOneLakh = x -> x > 100000;

        // Rule ko chalane ke liye .test(...) call karo: [00:03:32]
        System.out.println(salaryGreaterThanOneLakh.test(50000));   // false [00:03:52]
        System.out.println(salaryGreaterThanOneLakh.test(1000000)); // true  [00:03:58]

        // Ab 'if' ke andar lambi condition likhne ki jagah seedha predicate use karo: [00:04:24]
        int mySalary = 90000;
        if (salaryGreaterThanOneLakh.test(mySalary)) {
            System.out.println("Tax lagega!");
        }
    }
}

```

---

## 3. Asli Fayda Kya Hai? (Why Not Just Write a Normal `boolean` Method?)

Video mein `[00:05:10]` par Vipul ne sabse bada sawaal uthaya: *"Bhai hum `private boolean isGreaterThan(int x)` method bhi toh bana sakte the, phir `Predicate` kyun banaya?"* [[05:10](https://www.youtube.com/watch?v=CuuR6NKgnss&t=310)]

Iske **2 bade reasons** hain:

1. **1-Line Concise Syntax:** Pura method signature (`private boolean ...`) likhne ki jagah 1 line mein condition ban gayi [[05:39](https://www.youtube.com/watch?v=CuuR6NKgnss&t=339)].
2. **Passing Conditions as Arguments at Runtime (Stream API & Functional Programming):**
Normal Java method ko tum kisi doosre method ke bracket mein as an argument pass nahi kar sakte, lekin **`Predicate` ek variable/object hai—use tum `stream().filter(...)` ya `list.removeIf(...)` ke andar runtime par pass kar sakte ho!** [[06:36](https://www.youtube.com/watch?v=CuuR6NKgnss&t=396)]

```java
List<Integer> numbers = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

// Way 1: Using Predicate with a normal for-each loop [00:08:37]
Predicate<Integer> isEven = x -> x % 2 == 0;
for (Integer n : numbers) {
    if (isEven.test(n)) {
        System.out.print(n + " "); // Prints: 2 4 6 8 10 [00:09:03]
    }
}

// Way 2: The Real Power — Passing Predicate directly into .stream().filter(...)! [00:06:45]
int sumOfEvens = numbers.stream()
                        .filter(isEven) // Ya seedha .filter(x -> x % 2 == 0)
                        .mapToInt(n -> n)
                        .sum();

```

> **🎯 How to Articulate in an Interview (English):**
> *"`Predicate<T>` is a built-in functional interface in `java.util.function` that represents a boolean-valued function of one argument via its single abstract method `boolean test(T t)`. Instead of hardcoding `if-else` conditions, `Predicate` encapsulates conditions into reusable objects that can be composed using `and()`, `or()`, and `negate()`, and passed dynamically into higher-order functions like `Stream.filter()` or `Collection.removeIf()`."*

---

## 4. Predicate Chaining: `.and()`, `.or()`, `.negate()` & `isEqual()`

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"`and()`, `or()`, aur `negate()` turant `true`/`false` return NAHI karte! Yeh teeno methods purane Predicates ko mila kar **ek NAYA `Predicate` return karte hain**! Jab us naye `Predicate` par `.test(input)` chalaoge, tab final `true` ya `false` aayega!"* [[11:23](https://www.youtube.com/watch?v=CuuR6NKgnss&t=683)]

### 4.1 Combining Conditions with `.and()`, `.or()`, and `.negate()`

Video mein String ke 2 alag-alag Predicates banaye gaye [[09:39](https://www.youtube.com/watch?v=CuuR6NKgnss&t=579)]:

1. `startsWithV`: Kya string `'v'` se shuru hoti hai? [[09:57](https://www.youtube.com/watch?v=CuuR6NKgnss&t=597)]
2. `endsWithL`: Kya string `'l'` par khatam hoti hai? [[10:49](https://www.youtube.com/watch?v=CuuR6NKgnss&t=649)]

```java
import java.util.function.Predicate;

public class StringPredicateDemo {
    public static void main(String[] args) {
        // Predicate 1: Checks if first character is 'v' (case-insensitive) [00:09:57]
        Predicate<String> startsWithV = s -> s.toLowerCase().charAt(0) == 'v';

        // Predicate 2: Checks if last character is 'l' (case-insensitive) [00:10:49]
        Predicate<String> endsWithL = s -> s.toLowerCase().charAt(s.length() - 1) == 'l';

        // -------------------------------------------------------------------------
        // 1. DEFAULT METHOD: .and() (BOTH conditions must be true — Logical &&) [00:11:23]
        // -------------------------------------------------------------------------
        Predicate<String> startsWithVAndEndsWithL = startsWithV.and(endsWithL);
        System.out.println(startsWithVAndEndsWithL.test("Vipul")); // true  (Starts with V AND ends with L) [00:11:44]
        System.out.println(startsWithVAndEndsWithL.test("Vivek")); // false (Starts with V, but ends with K) [00:11:55]

        // -------------------------------------------------------------------------
        // 2. DEFAULT METHOD: .or() (AT LEAST ONE condition must be true — Logical ||) [00:11:55]
        // -------------------------------------------------------------------------
        Predicate<String> startsWithVOrEndsWithL = startsWithV.or(endsWithL);
        System.out.println(startsWithVOrEndsWithL.test("Vivek"));  // true  (First condition is true!) [00:12:05]
        System.out.println(startsWithVOrEndsWithL.test("Payal"));  // true  (Second condition is true!)
        System.out.println(startsWithVOrEndsWithL.test("Ashish")); // false (Both are false)

        // -------------------------------------------------------------------------
        // 3. DEFAULT METHOD: .negate() (Reverses true <-> false — Logical !) [00:12:15]
        // Scenario: Club mein 'V' naam wale logon ki entry banned hai! [00:12:22]
        // -------------------------------------------------------------------------
        Predicate<String> notStartingWithV = startsWithV.negate();
        System.out.println(notStartingWithV.test("Vipul"));  // false (Entry Denied!) [00:12:41]
        System.out.println(notStartingWithV.test("Ashish")); // true  (Entry Allowed!)
    }
}

```

---

### 4.2 Using `Predicate` with Custom Objects (`Student`) & Static `Predicate.isEqual()`

```java
import java.util.function.Predicate;

public class CustomObjectPredicateDemo {
    static class Student {
        String name;
        int id;
        Student(String name, int id) { this.name = name; this.id = id; }
        public String getName() { return name; }
        public int getId()      { return id; }
    }

    public static void main(String[] args) {
        Student s1 = new Student("Vipul", 1); // [00:13:51]
        Student s2 = new Student("Ram", 2);   // [00:14:02]

        // 1. Custom Object Predicate: Check if student ID > 1 [00:14:19]
        Predicate<Student> idGreaterThanOne = x -> x.getId() > 1;
        System.out.println(idGreaterThanOne.test(s1)); // false (id is 1)
        System.out.println(idGreaterThanOne.test(s2)); // true  (id is 2) [00:14:42]

        // 2. STATIC METHOD: Predicate.isEqual(target) [00:15:09]
        // Kyunki yeh static method hai, isliye seedha Interface ke naam (Predicate.isEqual) se call hoga! [00:15:14]
        Predicate<String> isVipul = Predicate.isEqual("Vipul"); // [00:15:27]
        System.out.println(isVipul.test("Hello")); // false [00:15:45]
        System.out.println(isVipul.test("Vipul")); // true  [00:15:56]
    }
}

```

*(Under-the-Hood Note: `Predicate.isEqual(target)` andar se `Objects.equals(target, input)` chalata hai—isliye agar `target` ya `input` `null` bhi ho, toh yeh `NullPointerException` nahi phekta, safely handle kar leta hai! Aur agar `Student` object par `Predicate.isEqual(s1)` chalaoge, toh `Student` class mein `.equals()` override hona chahiye).*

---

### 4.3 Non-Rabbit-Hole Practical Additions (Jo Aage Spring Boot & CP Mein Kaam Aayenge)

| Utility / Variant | Syntax | Kab Kaam Aata Hai? |
| --- | --- | --- |
| **`list.removeIf(Predicate)`** | `nums.removeIf(x -> x < 0);` | Kisi bhi `ArrayList` ya `Set` se bina `Iterator` likhe 1 line mein elements delete karne ke liye! |
| **`Predicate.not(Predicate)`** *(Java 11+)* | `Predicate.not(String::isBlank)` | Method references ke upar `.negate()` nahi lag sakta, wahan `Predicate.not(...)` lagta hai. |
| **`BiPredicate<T, U>`** | `(str, len) -> str.length() == len` | Jab condition check karne ke liye **1 nahi balki 2 inputs** chahiye hon! |

> **🎯 How to Articulate in an Interview (English):**
> *"`Predicate` supports functional composition through three default methods—`and()`, `or()`, and `negate()`—which follow short-circuit evaluation and allow building complex, dynamic filter pipelines from simple atomic conditions. Additionally, the static `Predicate.isEqual(target)` method creates a null-safe equality check using `Objects.equals()` under the hood."*

---

---

## 5. Progressive Practice Arena: 4 Hands-On Tasks (Basic $\rightarrow$ Production Level)

Ab neeche diye gaye **4 tasks** ko dhyan se dekho (pehle khud socho ki kaise likhoge, aur phir neeche diye gaye **Complete Solutions** se match karo). Difficulty **Level 1 se Level 4** tak badhti jayegi:

---

### 🟢 Task 1 (Level: Warm-Up) — Username Sanitizer with `.test()` and `removeIf()`

* **Problem Statement:**
Tumhare paas ek mutable list hai: `List<String> usernames = new ArrayList<>(Arrays.asList("ashish", "om", "", "   ", "sam_99", "rj", "admin123"));`
1. Ek `Predicate<String> isValidUsername` banao jo check kare ki string **blank nahi hai** (`!s.isBlank()`) **AUR** uski length **$\ge 5$** hai.
2. Ek doosra `Predicate<String> isShortOrBlank` banao jo pehle wale predicate ko **`.negate()`** karke bane.
3. `usernames.removeIf(isShortOrBlank)` use karke list mein se saare invalid usernames 1 line mein uda do aur final list print karo.



---

### 🟡 Task 2 (Level: Easy-Medium) — Modular Number Rules & Chaining (`.and()`, `.or()`, `.negate()`)

* **Problem Statement:**
3 atomic (chote) `Predicate<Integer>` banao:
1. `isPositive`: Number $> 0$ hona chahiye.
2. `isEven`: Number $2$ se divisible hona chahiye.
3. `isMultipleOf5`: Number $5$ se divisible hona chahiye.


* **Task:** Bina naya lambda (`x -> ...`) likhe, sirf upar wale teeno predicates ko `.and()`, `.or()`, aur `.negate()` se jod kar ek naya `Predicate<Integer> specialFilter` banao jo un numbers ko pass kare jo:
**"Positive hain, AUR (Even hain YA 5 ke multiple hain), LEKIN 10 ke multiple NAHI hain (i.e., Even aur MultipleOf5 dono ek sath nahi hone chahiye)!"**
* Is `specialFilter` se `[-10, -4, 0, 4, 5, 7, 10, 12, 15, 20]` ko filter karke print karo.



---

### 🟠 Task 3 (Level: Medium) — Passing `Predicate` as a Method Parameter (Service Layer Pattern)

* **Problem Statement:**
Ek `Employee` class hai jismein `(int id, String name, String role, int salary)` hain.
* Ek generic helper method likho:
`public static List<Employee> filterEmployees(List<Employee> list, Predicate<Employee> condition)`
jo list par loop chala kar sirf un employees ki nayi list return kare jo `condition.test(emp)` pass karte hain.
* Main method mein 3 predicates banao (`isSDE`, `isHighPaid` where salary $\ge 100000$, `isInstructor`) aur `filterEmployees(...)` ko alag-alag combinations (`isSDE.and(isHighPaid)`, `isSDE.or(isInstructor)`) pass karke dikhao ki **ek hi method bina code badle 10 alag tarah ki filtering kaise kar deta hai!**



---

### 🔴 Task 4 (Level: Production Backend) — Dynamic E-Commerce Filter Builder (Amazon/Flipkart Style)

* **Problem Statement:**
Jab koi user Amazon par products search karta hai, toh woh **optional filters** lagata hai (kabhi sirf `category` chunta hai, kabhi `maxPrice` bhi lagata hai, kabhi `inStockOnly` tick karta hai). Agar hum har combination ke liye `if-else` likhenge toh $2^3 = 8$ `if-else` branches ban jayengi!
* **Task:** Ek method likho:
`public static Predicate<Product> buildDynamicFilter(String category, Integer maxPrice, boolean inStockOnly)`
Jo ek base predicate `Predicate<Product> filter = p -> true;` se shuru kare, aur jo-jo filter user ne bheja hai (`category != null`, `maxPrice != null`, `inStockOnly == true`), use `filter = filter.and(...)` se dynamically chain karta jaye!



---

---

## 6. Complete Verified Solutions (Run & Observe!)

### ✅ Solution 1: Username Sanitizer (`test` + `negate` + `removeIf`)

```java
import java.util.*;
import java.util.function.Predicate;

public class Task1Solution {
    public static void main(String[] args) {
        List<String> usernames = new ArrayList<>(
            Arrays.asList("ashish", "om", "", "   ", "sam_99", "rj", "admin123")
        );

        // 1. Atomic conditions:
        Predicate<String> isNotBlank = s -> s != null && !s.isBlank();
        Predicate<String> hasMinLength5 = s -> s.length() >= 5;

        // Combine with .and():
        Predicate<String> isValidUsername = isNotBlank.and(hasMinLength5);

        // 2. Reverse condition using .negate():
        Predicate<String> isInvalidUsername = isValidUsername.negate();

        // 3. One-liner cleanup using Collection.removeIf(Predicate):
        usernames.removeIf(isInvalidUsername);

        System.out.println("Valid Usernames: " + usernames);
        // Output: Valid Usernames: [ashish, sam_99, admin123]
    }
}

```

---

### ✅ Solution 2: Modular Number Rules & Chaining

```java
import java.util.*;
import java.util.function.Predicate;

public class Task2Solution {
    public static void main(String[] args) {
        int[] nums = {-10, -4, 0, 4, 5, 7, 10, 12, 15, 20};

        // 3 Atomic Predicates:
        Predicate<Integer> isPositive     = x -> x > 0;
        Predicate<Integer> isEven         = x -> x % 2 == 0;
        Predicate<Integer> isMultipleOf5  = x -> x % 5 == 0;

        // Derived Rule 1: Even OR Multiple of 5
        Predicate<Integer> evenOrMult5 = isEven.or(isMultipleOf5);

        // Derived Rule 2: Multiple of 10 means (Even AND Multiple of 5) -> Negate it!
        Predicate<Integer> notMultipleOf10 = isEven.and(isMultipleOf5).negate();

        // Final Rule: Positive AND (Even OR Mult5) AND NOT(Mult10)
        Predicate<Integer> specialFilter = isPositive
                .and(evenOrMult5)
                .and(notMultipleOf10);

        System.out.print("Filtered Numbers: ");
        for (int n : nums) {
            if (specialFilter.test(n)) {
                System.out.print(n + " ");
            }
        }
        // Output: Filtered Numbers: 4 5 12 15
        // (-10, -4, 0 rejected by isPositive; 7 rejected by evenOrMult5; 10, 20 rejected by notMultipleOf10)
    }
}

```

---

### ✅ Solution 3: Passing `Predicate` as a Method Parameter

```java
import java.util.*;
import java.util.function.Predicate;

public class Task3Solution {
    static class Employee {
        int id;
        String name;
        String role;
        int salary;

        Employee(int id, String name, String role, int salary) {
            this.id = id;
            this.name = name;
            this.role = role;
            this.salary = salary;
        }

        @Override
        public String toString() {
            return name + " (" + role + ", ₹" + salary + ")";
        }
    }

    // ONE Reusable Method that accepts ANY Predicate<Employee> at runtime!
    public static List<Employee> filterEmployees(List<Employee> list, Predicate<Employee> condition) {
        List<Employee> result = new ArrayList<>();
        for (Employee emp : list) {
            if (condition.test(emp)) {
                result.add(emp);
            }
        }
        return result;
    }

    public static void main(String[] args) {
        List<Employee> employees = List.of(
            new Employee(1, "Ashish", "SDE", 120000),
            new Employee(2, "Sumit", "SDE", 85000),
            new Employee(3, "Samyak", "Instructor", 95000),
            new Employee(4, "Vipul", "Architect", 200000)
        );

        // Reusable Predicates:
        Predicate<Employee> isSDE        = e -> e.role.equals("SDE");
        Predicate<Employee> isInstructor = e -> e.role.equals("Instructor");
        Predicate<Employee> isHighPaid   = e -> e.salary >= 100000;

        // Query 1: SDEs earning >= 1,00,000
        System.out.println("High-Paid SDEs: " + filterEmployees(employees, isSDE.and(isHighPaid)));
        // Output: [Ashish (SDE, ₹120000)]

        // Query 2: Either SDE OR Instructor
        System.out.println("Tech Team: " + filterEmployees(employees, isSDE.or(isInstructor)));
        // Output: [Ashish (SDE, ₹120000), Sumit (SDE, ₹85000), Samyak (Instructor, ₹95000)]

        // Query 3: Non-SDEs earning >= 1,00,000
        System.out.println("High-Paid Non-SDEs: " + filterEmployees(employees, isSDE.negate().and(isHighPaid)));
        // Output: [Vipul (Architect, ₹200000)]
    }
}

```

---

### ✅ Solution 4: Dynamic E-Commerce Filter Builder (Real-World Spring Boot Pattern)

```java
import java.util.*;
import java.util.function.Predicate;

public class Task4Solution {
    static class Product {
        String name;
        String category;
        int price;
        boolean inStock;

        Product(String name, String category, int price, boolean inStock) {
            this.name = name;
            this.category = category;
            this.price = price;
            this.inStock = inStock;
        }

        @Override
        public String toString() {
            return name + " [₹" + price + ", " + (inStock ? "In-Stock" : "Out-of-Stock") + "]";
        }
    }

    // Dynamically chains predicates based on whichever optional filters the user provided!
    public static Predicate<Product> buildDynamicFilter(String category, Integer maxPrice, boolean inStockOnly) {
        // Start with a base predicate that accepts everything:
        Predicate<Product> filter = p -> true;

        if (category != null && !category.isBlank()) {
            filter = filter.and(p -> p.category.equalsIgnoreCase(category));
        }
        if (maxPrice != null) {
            filter = filter.and(p -> p.price <= maxPrice);
        }
        if (inStockOnly) {
            filter = filter.and(p -> p.inStock);
        }
        return filter;
    }

    public static void main(String[] args) {
        List<Product> catalog = List.of(
            new Product("HP Victus 15", "Electronics", 65000, true),
            new Product("MacBook Pro", "Electronics", 180000, true),
            new Product("Sony Headphones", "Electronics", 15000, false),
            new Product("Whey Protein", "Fitness", 2500, true)
        );

        // Scenario A: User filters by Category = "Electronics", MaxPrice = 70000, InStockOnly = true
        Predicate<Product> search1 = buildDynamicFilter("Electronics", 70000, true);
        System.out.println("Search 1 Results: " + catalog.stream().filter(search1).toList());
        // Output: [HP Victus 15 [₹65000, In-Stock]] (MacBook is > 70000, Sony is Out-of-Stock)

        // Scenario B: User only checks "InStockOnly = true" (no category or price limit)
        Predicate<Product> search2 = buildDynamicFilter(null, null, true);
        System.out.println("Search 2 Results: " + catalog.stream().filter(search2).toList());
        // Output: [HP Victus 15 [₹65000, In-Stock], MacBook Pro [₹180000, In-Stock], Whey Protein [₹2500, In-Stock]]
    }
}

```

---

Ab batao—kya `Predicate` ka `test()` vs `.and()`/`.or()`/`.negate()` chaining aur runtime parameter passing wala concept ab 100% crystal clear hai? Jab ready ho, agle videos (`Function`, `Consumer`, `Supplier`, `Method Reference`) ke links bhejo!