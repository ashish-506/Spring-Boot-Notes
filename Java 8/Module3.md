# JAVA 8 MASTER NOTES: `Function<T, R>` Functional Interface (`apply`, `andThen`, `compose` & `identity`)

**Source Video:** [Java 8 Function Interface Explained: How to Use apply(), andThen(), compose() and identity() (Engineering Digest)](http://www.youtube.com/watch?v=COc5i6hG1j0)

---

## 1. The Core Mental Model: `Predicate<T>` vs. `Function<T, R>`

Pichle video ke `Predicate<T>` aur is video ke `Function<T, R>` mein sabse bada farq samajhne ke liye yeh mental model lock karo:

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"`Predicate` aur `Function` dono Functional Interfaces hain, par dono ka kaam alag hai! **`Predicate` sirf condition check karta hai** (jaise 'kya iski age 10 saal se zyada hai?' $\rightarrow$ `true` ya `false`). Lekin **`Function` condition check nahi karta—woh kuch KAAM karke laata hai!** Aapne use lowercase string di, woh uppercase bana kar la dega; aapne string di, woh usकी length nikaal kar la dega; aapne 100 students ki list di, woh pass hue students ki nayi list bana kar la dega!"* [[00:43](https://www.youtube.com/watch?v=COc5i6hG1j0&t=43)]

### Internal Blueprint of `java.util.function.Function<T, R>`

`Function<T, R>` ke andar **2 Generic Types (`<T, R>`)** aur kul **4 methods** hote hain (1 Abstract + 2 Default + 1 Static) [[00:19](https://www.youtube.com/watch?v=COc5i6hG1j0&t=19)]:

* **`T` (Input Type):** Function ke argument mein kis type ka data jayega [[01:36](https://www.youtube.com/watch?v=COc5i6hG1j0&t=96)].
* **`R` (Return Type):** Function kaam karne ke baad kis type ka result wapas lautayega [[01:44](https://www.youtube.com/watch?v=COc5i6hG1j0&t=104)]. *(Note: `T` aur `R` alag-alag types bhi ho sakte hain jaise `<String, Integer>`, aur same type bhi ho sakte hain jaise `<String, String>`).*

```java
package java.util.function;

@FunctionalInterface
public interface Function<T, R> {

    // 1. THE SINGLE ABSTRACT METHOD (Takes input of type T, returns output of type R):
    R apply(T t);

    // 2. TWO DEFAULT METHODS (For Function Chaining / Pipelines):
    default <V> Function<T, V> andThen(Function<? super R, ? extends V> after)  { ... }
    default <V> Function<V, R> compose(Function<? super V, ? extends T> before) { ... }

    // 3. ONE STATIC METHOD (Returns a function that always returns its input argument as-is):
    static <T> Function<T, T> identity() {
        return t -> t;
    }
}

```

> **🎯 How to Articulate in an Interview (English):**
> *"`Function<T, R>` is a built-in functional interface in `java.util.function` that represents a transformation operation taking an input of type `T` and producing a result of type `R` via its single abstract method `R apply(T t)`. While `Predicate<T>` is used for filtering (`Stream.filter()`), `Function<T, R>` is the backbone of data transformation (`Stream.map()`)."*

---

## 2. How to Use `Function<T, R>` and `.apply()` (3 Step-by-Step Examples)

Jaise `Predicate` ko chalane ke liye `.test(x)` call karte the, waise hi **`Function` ko chalane ke liye `.apply(x)` call karte hain** [[02:44](https://www.youtube.com/watch?v=COc5i6hG1j0&t=164)].

### Example 1 & 2: Basic Transformations (`String -> Integer` & `String -> String`)

```java
import java.util.function.Function;

public class FunctionBasicDemo {
    public static void main(String[] args) {
        // Example 1: Input = String (T), Output = Integer (R) [00:01:57]
        // Lambda 'x -> x.length()' implements 'Integer apply(String x)'
        Function<String, Integer> getLength = x -> x.length();
        System.out.println(getLength.apply("Vipul")); // Prints: 5 [00:02:51]

        // Example 2: Input = String (T), Output = String (R) (First 3 characters) [00:03:06]
        Function<String, String> getFirstThreeChars = s -> s.substring(0, 3);
        System.out.println(getFirstThreeChars.apply("Vipul")); // Prints: Vip [00:03:26]
    }
}

```

---

### Example 3: Using One `Function` Inside Another (`List<Student> -> List<Student>`)

Video mein ek `Student` class banayi gayi aur ek `Function<List<Student>, List<Student>>` banaya gaya jo andar se upar wale `function2` (`s -> s.substring(0, 3)`) ko call karke sirf un students ki list return karta hai jinka naam `"vip"` se shuru hota hai [[04:21](https://www.youtube.com/watch?v=COc5i6hG1j0&t=261)]:

```java
import java.util.*;
import java.util.function.Function;

public class StudentFunctionDemo {
    static class Student {
        private int id;
        private String name;

        public Student(int id, String name) {
            this.id = id;
            this.name = name;
        }
        public int getId()      { return id; }
        public String getName() { return name; }

        @Override
        public String toString() {
            return "Student{id=" + id + ", name='" + name + "'}";
        }
    }

    public static void main(String[] args) {
        // Helper Function: Extracts first 3 letters of any string [00:03:19]
        Function<String, String> firstThreeLetters = s -> s.substring(0, 3);

        // Main Function: Takes List<Student> -> Returns filtered List<Student> [00:04:09]
        // Because the body has multiple lines, we MUST use curly braces {} and explicit 'return'! [00:04:50]
        Function<List<Student>, List<Student>> studentsWithVipAsPrefix = li -> {
            List<Student> result = new ArrayList<>(); // [00:05:53]
            for (Student s : li) {
                // Calling firstThreeLetters.apply(...) inside another Function! [00:05:27]
                if (firstThreeLetters.apply(s.getName()).equalsIgnoreCase("vip")) {
                    result.add(s);
                }
            }
            return result; // [00:06:10]
        };

        Student s1 = new Student(2, "Vipul");  // [00:06:15]
        Student s2 = new Student(2, "Viplav"); // [00:06:25]
        Student s3 = new Student(2, "Arnav");

        List<Student> students = Arrays.asList(s1, s2, s3); // [00:06:36]
        List<Student> filteredStudents = studentsWithVipAsPrefix.apply(students); // [00:06:49]

        System.out.println(filteredStudents);
        // Output: [Student{id=2, name='Vipul'}, Student{id=2, name='Viplav'}] [00:07:19]
    }
}

```

---

## 3. Function Chaining: `.andThen()` vs. `.compose()`

Jab do functions ko ek pipeline mein jodna ho (ek function ka output doosre function ka input ban jaye), tab **`andThen`** aur **`compose`** use hote hain [[07:52](https://www.youtube.com/watch?v=COc5i6hG1j0&t=472)].

### 3.1 String Chaining Example

```java
Function<String, String> function1 = s -> s.toUpperCase();      // Converts to UPPERCASE [00:08:19]
Function<String, String> function2 = s -> s.substring(0, 3);    // Takes first 3 chars [00:08:28]

// Direct Chaining: First function1 runs ("vipul" -> "VIPUL"), then function2 runs ("VIPUL" -> "VIP") [00:09:15]
System.out.println(function1.andThen(function2).apply("Vipul")); // Prints: VIP [00:09:29]

```

---

### 3.2 Mathematical Order Test: `f1.andThen(f2)` vs. `f1.compose(f2)`

String wale example mein pehle uppercase karo ya pehle substring lo, answer `"VIP"` hi aata hai [[09:29](https://www.youtube.com/watch?v=COc5i6hG1j0&t=569)]. Farq samajhne ke liye Vipul ne **2 mathematical functions** liye [[09:39](https://www.youtube.com/watch?v=COc5i6hG1j0&t=579)]:

* **`function1`:** `x -> 2 * x` *(Double the number)* [[09:46](https://www.youtube.com/watch?v=COc5i6hG1j0&t=586)]
* **`function2`:** `x -> x * x * x` *(Cube the number)* [[10:02](https://www.youtube.com/watch?v=COc5i6hG1j0&t=602)]

Ab input **`3`** dekar teeno cases dekho:

```java
Function<Integer, Integer> function1 = x -> 2 * x;       // Doubles x
Function<Integer, Integer> function2 = x -> x * x * x;   // Cubes x

// CASE 1: function1.andThen(function2).apply(3) [00:10:18]
// Step 1 (function1 runs FIRST): 2 * 3 = 6
// Step 2 (function2 runs NEXT on 6): 6 * 6 * 6 = 216 [00:11:48]
System.out.println(function1.andThen(function2).apply(3)); // Output: 216 [00:10:44]

// CASE 2: function2.andThen(function1).apply(3) [00:10:32]
// Step 1 (function2 runs FIRST): 3 * 3 * 3 = 27
// Step 2 (function1 runs NEXT on 27): 2 * 27 = 54 [00:12:02]
System.out.println(function2.andThen(function1).apply(3)); // Output: 54 [00:10:50]

// CASE 3: function1.compose(function2).apply(3) [00:11:22]
// 'compose' reverses the order! Argument function (function2) runs FIRST, then caller (function1)! [00:12:19]
// Step 1 (function2 runs FIRST): 3 * 3 * 3 = 27
// Step 2 (function1 runs NEXT on 27): 2 * 27 = 54
System.out.println(function1.compose(function2).apply(3)); // Output: 54 [00:12:30]

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> * **`f1.andThen(f2)`:** Pehle **`f1`** chalega, uska jo output aayega uspar **`f2`** chalega ($f_2(f_1(x))$) [[11:36](https://www.youtube.com/watch?v=COc5i6hG1j0&t=696)].
> * **`f1.compose(f2)`:** Ulta kaam karta hai! Pehle bracket ke andar wala **`f2`** chalega, aur uske output par **`f1`** chalega ($f_1(f_2(x))$)—yaani `f1.compose(f2)` aur `f2.andThen(f1)` **100% same result** dete hain [[12:26](https://www.youtube.com/watch?v=COc5i6hG1j0&t=746)]! Real code mein confusion se bachne ke liye hamesha **`andThen()`** hi use karo [[15:08](https://www.youtube.com/watch?v=COc5i6hG1j0&t=908)].
> 
> 

---

## 4. Static Method: `Function.identity()` (And Where It Is Actually Used!)

* **What It Does:** `Function.identity()` ek `static` method hai (isliye seedha interface ke naam `Function.identity()` se call hota hai) [[13:00](https://www.youtube.com/watch?v=COc5i6hG1j0&t=780)].
* **Behavior:** Yeh ek aisa `Function<T, T>` return karta hai jo **jo input doge, bina kuch badle exact wahi output wapas lauta dega** (`t -> t`) [[13:45](https://www.youtube.com/watch?v=COc5i6hG1j0&t=825)]!

```java
Function<String, String> identityFunction = Function.identity(); // [00:13:16]
System.out.println(identityFunction.apply("Vipul")); // Prints: Vipul [00:13:39]

```

### Non-Rabbit-Hole Practical Insight: Asli Project Mein `Function.identity()` Kahan Kaam Aata Hai?

Video mein `[00:14:31]` par Vipul ne kaha ki *"Yaar yeh toh kuch kaam hi nahi kar रहा, iska use Streams mein dikhega"* [[14:37](https://www.youtube.com/watch?v=COc5i6hG1j0&t=877)].
Spring Boot mein iska **#1 Real-World Use Case** tab aata hai jab tumhare paas `List<Student>` ho aur tumhe use `Map<Integer, Student>` (`id -> Student object itself`) mein convert karna ho:

```java
// Converting List<Student> into Map<Integer, Student> where:
// Key   = s.getId()
// Value = The Student object itself (Function.identity() means 's -> s')!
Map<Integer, Student> studentMap = students.stream()
        .collect(Collectors.toMap(
            s -> s.getId(),       // Key Mapper Function: Student -> Integer (ID)
            Function.identity()   // Value Mapper Function: Student -> Student (As-is!)
        ));

```

> **🎯 How to Articulate in an Interview (English):**
> *"`Function` supports pipeline composition via two default methods: `f1.andThen(f2)` executes `f1` first and passes its output to `f2`, whereas `f1.compose(f2)` executes `f2` first and passes its result to `f1`. The static `Function.identity()` method returns a pass-through function (`t -> t`), commonly used in `Collectors.toMap()` when storing the stream element itself as the map value."*

---

---

## 5. Progressive Practice Arena: 3 Hands-On Tasks (Basic $\rightarrow$ Production Level)

Ab neeche diye gaye **3 tasks** ko dekho aur unke solutions se compare karo taaki `Function<T, R>`, `andThen()`, aur `Predicate` + `Function` ka combo dimaag mein pakka ho jaye:

### 🟢 Task 1 (Level: Easy) — Text Sanitizer Pipeline Using `.andThen()`

* **Problem Statement:**
3 chote `Function` banao:
1. `trimSpaces` (`Function<String, String>`): Leading aur trailing spaces hataye (`s.trim()`).
2. `replaceSpacesWithUnderscore` (`Function<String, String>`): Beech ke spaces ko `'_'` se replace kare (`s.replace(" ", "_")`).
3. `toLower` (`Function<String, String>`): String ko lowercase kare (`s.toLowerCase()`).


* In teeno ko `.andThen()` se chain karke ek `slugGenerator` function banao jo `"   Java 8 Master Notes   "` ko `"java_8_master_notes"` mein badal de!



### 🟡 Task 2 (Level: Medium) — Combining `Predicate<T>` + `Function<T, R>` (Salary Increment Engine)

* **Problem Statement:**
Tumhare paas `Employee(String name, int rating, int salary)` ki list hai.
1. Ek `Predicate<Employee> isEligibleForBonus` banao jo check kare ki `rating >= 4` hai ya nahi.
2. Ek `Function<Employee, Integer> calculateNewSalary` banao jo employee ki salary mein **20% hike** (`(int)(e.salary * 1.20)`) karke nayi salary return kare.
3. Loop chala kar sirf eligible employees par `calculateNewSalary.apply(e)` lagao!



### 🔴 Task 3 (Level: Production Backend) — Entity-to-DTO Converter (`Function<UserEntity, UserDTO>`)

* **Problem Statement:**
Spring Boot backend mein jab hum Database se `UserEntity` nikaalte hain, toh usmein `password` aur `internalSecret` bhi hota hai jo hum Frontend ko API response mein **nahi** bhejte. Hum use `UserDTO` (Data Transfer Object) mein convert karte hain.
* Ek `Function<UserEntity, UserDTO> toDto` likho jo `UserEntity(id, name, email, password)` ko `UserDTO(id, name, email)` mein transform kare, aur phir `users.stream().map(toDto).toList()` chala kar dikhao!



---

## 6. Complete Verified Solutions

### ✅ Solution 1: Text Sanitizer Pipeline (`.andThen()`)

```java
import java.util.function.Function;

public class FunctionTask1 {
    public static void main(String[] args) {
        Function<String, String> trimSpaces     = s -> s.trim();
        Function<String, String> spacesToSnake  = s -> s.replace(" ", "_");
        Function<String, String> toLower        = s -> s.toLowerCase();

        // Chaining all 3 functions into a single pipeline:
        Function<String, String> slugGenerator = trimSpaces
                .andThen(spacesToSnake)
                .andThen(toLower);

        String rawTitle = "   Java 8 Master Notes   ";
        System.out.println(slugGenerator.apply(rawTitle));
        // Output: java_8_master_notes
    }
}

```

---

### ✅ Solution 2: `Predicate<T>` + `Function<T, R>` Combo

```java
import java.util.*;
import java.util.function.Function;
import java.util.function.Predicate;

public class FunctionTask2 {
    static class Employee {
        String name;
        int rating;
        int salary;

        Employee(String name, int rating, int salary) {
            this.name = name;
            this.rating = rating;
            this.salary = salary;
        }
    }

    public static void main(String[] args) {
        List<Employee> team = List.of(
            new Employee("Ashish", 5, 100000),
            new Employee("Rohit", 3, 80000),
            new Employee("Samyak", 4, 90000)
        );

        // 1. Predicate: Checks condition (Returns boolean)
        Predicate<Employee> isEligibleForBonus = e -> e.rating >= 4;

        // 2. Function: Performs calculation (Takes Employee, Returns new Integer salary)
        Function<Employee, Integer> calculateNewSalary = e -> (int) (e.salary * 1.20);

        for (Employee emp : team) {
            if (isEligibleForBonus.test(emp)) {
                int updatedSalary = calculateNewSalary.apply(emp);
                System.out.println(emp.name + " promoted to ₹" + updatedSalary);
            }
        }
        // Output:
        // Ashish promoted to ₹120000
        // Samyak promoted to ₹108000
    }
}

```

---

### ✅ Solution 3: Spring Boot Entity-to-DTO Mapper (`Function` + `.stream().map()`)

```java
import java.util.*;
import java.util.function.Function;

public class FunctionTask3 {
    // Database Entity (Contains sensitive password!)
    static class UserEntity {
        int id; String name; String email; String password;
        UserEntity(int id, String name, String email, String password) {
            this.id = id; this.name = name; this.email = email; this.password = password;
        }
    }

    // Safe API Response DTO (No password field!)
    static class UserDTO {
        int id; String name; String email;
        UserDTO(int id, String name, String email) {
            this.id = id; this.name = name; this.email = email;
        }
        @Override
        public String toString() { return "UserDTO{id=" + id + ", name='" + name + "', email='" + email + "'}"; }
    }

    public static void main(String[] args) {
        List<UserEntity> dbUsers = List.of(
            new UserEntity(1, "Ashish", "ashish@niat.com", "secretHash123"),
            new UserEntity(2, "Sumit", "sumit@icpc.com", "pass999")
        );

        // Function<T, R>: Input = UserEntity, Output = UserDTO
        Function<UserEntity, UserDTO> entityToDtoMapper =
                user -> new UserDTO(user.id, user.name, user.email);

        // In Spring Boot Services, Stream.map() takes a Function<T, R>!
        List<UserDTO> apiResponse = dbUsers.stream()
                                           .map(entityToDtoMapper)
                                           .toList();

        System.out.println(apiResponse);
        // Output: [UserDTO{id=1, name='Ashish', email='ashish@niat.com'}, UserDTO{id=2, name='Sumit', email='sumit@icpc.com'}]
    }
}

```

---

Isko apni `.md` file mein add kar lo. Jab agle links (`Consumer`, `Supplier`, `BiPredicate`/`BiFunction`, `Method Reference`) ready hon toh bhejo!