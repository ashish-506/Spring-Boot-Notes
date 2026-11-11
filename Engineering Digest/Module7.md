# SPRING BOOT MASTER NOTES (MODULE 7): `ResponseEntity`, HTTP Status Codes & Project Lombok

**Compiled Source Videos (Engineering Digest — Videos 10 & 11):**

1. [ResponseEntity in Spring Boot in Hindi | Handling HttpStatus while creating REST API](http://www.youtube.com/watch?v=tWBhE1Cn8D0)
2. [Mastering Project Lombok in Java: Simplify Your Code Like a Pro](http://www.youtube.com/watch?v=xmRjF9qycIk)

---

## PART A: HTTP Status Codes & `ResponseEntity<T>`

**Source:** [ResponseEntity in Spring Boot](http://www.youtube.com/watch?v=tWBhE1Cn8D0)

### 1. Abhi Tak Ke Controller Code Mein Kya Dikkat Thi? (The "Everything is `200 OK`" Bug)

Pichle video tak humare Controller ke saare endpoints (`getAll`, `createEntry`, `getById`, `deleteById`, `updateById`) har haalat mein Postman ko **`200 OK`** status code bhej rahe the,:

* Jab hum `POST /journal` se naya document create kar rahe the, tab bhi **`200 OK`** aa raha tha (jabki ideal REST standard ke hisaab se **`201 Created`** aana chahiye).
* Jab hum `GET /journal/id/{myId}` mein koi **galat/non-existent ID** bhej rahe the aur DB se `null` (khali body) aa rahi thi, tab bhi **`200 OK`** aa raha tha (jabki **`404 Not Found`** aana chahiye),!
* Jab hum `DELETE /journal/id/{myId}` chala rahe the, tab bhi **`200 OK`** aa raha tha (jabki **`204 No Content`** aana chahiye).

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Jab कल को React Frontend aapki API ko call karega, toh woh response body padhne se pehle **HTTP Status Code** check karega! Agar server par kuch phat gaya ya ID nahi mili aur aapne phir bhi `200 OK` bhej diya, toh Frontend ko kaise pata chalega ki kaam hua ya fail ho gaya? Isliye Response Body ke saath sahi **3-Digit HTTP Status Code** bhejna mandatory hai!"*,

---

### 2. The 5 Categories of HTTP Status Codes (Complete Cheat Sheet)

Har HTTP Status Code ek **3-digit number** hota hai jiska pehla digit uski category batata hai,:

| Category | Meaning | Most Important Status Codes to Know for Interviews & Production |
| --- | --- | --- |
| **`1xx`** *(Informational)* | Request mil gayi hai, server abhi process kar raha hai (Rarely used directly). | `100 Continue`, `102 Processing` |
| **`2xx`** *(Success)* | Request successfully samajh li gayi aur process ho gayi. | • **`200 OK`:** Standard success (e.g., `GET` ya `PUT` mein data mil gaya).<br>• **`201 CREATED`:** `POST` request se DB mein naya resource ban gaya.<br>• **`204 NO CONTENT`:** Action successful raha par response body mein kuch nahi bhej rahe (e.g., `DELETE`). |
| **`3xx`** *(Redirection)* | Client ko resource tak pahunchne ke liye extra step (redirect) lena padega. | • **`301 Moved Permanently`:** Purani API URL hamesha ke liye badal gayi hai.<br>• **`302 Found`:** URL temporarily doosri jagah shift hui hai (`Location` header ke sath).<br>• **`304 Not Modified`:** Client ke paas jo cached data hai wahi use kar lo. |
| **`4xx`** *(Client Error)* | **Client (Frontend/Postman) ki galti**—galat JSON, galat URL, ya bina login ke request. | • **`400 BAD REQUEST`:** Client ne galat syntax/payload bheja.<br>• **`401 UNAUTHORIZED`:** Authentication fail (Login nahi kiya ya galat password).<br>• **`403 FORBIDDEN`:** Login toh hai par is resource ki permission/role nahi hai.<br>• **`404 NOT FOUND`:** Jo ID ya URL maanga gaya woh exist nahi karta. |
| **`5xx`** *(Server Error)* | **Backend (Humare Spring Boot code) ki galti**—exception phat gaya ya DB down hai. | • **`500 INTERNAL SERVER ERROR`:** Code mein unhandled exception phat gaya.<br>• **`502 BAD GATEWAY`:** Proxy/Gateway ko upstream server se invalid response mila.<br>• **`503 SERVICE UNAVAILABLE`:** Server overloaded hai ya maintenance par hai. |
---

### 3. What is `ResponseEntity<T>` and Why Do We Use Wildcard `ResponseEntity<?>`?

* **What is `ResponseEntity<T>`?**
`ResponseEntity<T>` Spring Framework ki ek built-in wrapper class hai jiske zariye hum Controller method se **3 cheezein customize karke ek sath** return kar sakte hain,:
1. **HTTP Status Code** (`HttpStatus.OK`, `HttpStatus.CREATED`, `HttpStatus.NOT_FOUND`, etc.)
2. **Response Body** (Humara data object, jaise `JournalEntry` ya `List<JournalEntry>`)
3. **HTTP Headers** (Optional custom headers)


* **Why `ResponseEntity<?>` (Wildcard Question Mark)?**
* Agar tum return type `ResponseEntity<JournalEntry>` likhte ho, toh तुम us method se `JournalEntry` ke alawa kisi aur class ka object return nahi kar sakte,.
* Agar tum **`ResponseEntity<?>` (Java Generics Wildcard)** likh dete ho, toh tumhari marzi—success hone par `JournalEntry` return karo, error hone par `String` error message return karo, ya `User` object return karo, ya bina body ke sirf Status Code return karo,!



---

### 4. Upgraded Controller Code Using `ResponseEntity<?>` (`JournalEntryControllerV2`)

```java
package net.engineeringdigest.journalApp.controller;

import net.engineeringdigest.journalApp.entity.JournalEntry;
import net.engineeringdigest.journalApp.service.JournalEntryService;
import org.bson.types.ObjectId;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/journal")
public class JournalEntryControllerV2 {

    @Autowired
    private JournalEntryService journalEntryService;

    // =========================================================================
    // 1. GET ALL: Returns 200 OK (with List) if entries exist, else 404 NOT FOUND [00:16:35]
    // =========================================================================
    @GetMapping
    public ResponseEntity<?> getAll() {
        List<JournalEntry> all = journalEntryService.getAll();
        if (all != null && !all.isEmpty()) {
            return new ResponseEntity<>(all, HttpStatus.OK); // 200 OK [00:16:50]
        }
        return new ResponseEntity<>(HttpStatus.NOT_FOUND);   // 404 Not Found [00:16:58]
    }

    // =========================================================================
    // 2. CREATE: Returns 201 CREATED (with saved entry), or 400 BAD REQUEST on exception [00:13:47], [00:14:06]
    // =========================================================================
    @PostMapping
    public ResponseEntity<JournalEntry> createEntry(@RequestBody JournalEntry myEntry) {
        try {
            myEntry.setDate(LocalDateTime.now());
            journalEntryService.saveEntry(myEntry);
            return new ResponseEntity<>(myEntry, HttpStatus.CREATED); // 201 Created [00:13:58]
        } catch (Exception e) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);      // 400 Bad Request [00:14:28]
        }
    }

    // =========================================================================
    // 3. GET BY ID: Returns 200 OK if Optional has data, else 404 NOT FOUND [00:12:00]
    // =========================================================================
    @GetMapping("id/{myId}")
    public ResponseEntity<JournalEntry> getJournalEntryById(@PathVariable ObjectId myId) {
        Optional<JournalEntry> journalEntry = journalEntryService.findById(myId);
        if (journalEntry.isPresent()) {
            return new ResponseEntity<>(journalEntry.get(), HttpStatus.OK); // 200 OK [00:12:20]
        }
        return new ResponseEntity<>(HttpStatus.NOT_FOUND);                  // 404 Not Found [00:13:19]
    }

    // =========================================================================
    // 4. DELETE BY ID: Returns 204 NO CONTENT [00:14:34]
    // =========================================================================
    @DeleteMapping("id/{myId}")
    public ResponseEntity<?> deleteJournalEntryById(@PathVariable ObjectId myId) {
        journalEntryService.deleteById(myId);
        return new ResponseEntity<>(HttpStatus.NO_CONTENT); // 204 No Content [00:14:45]
    }

    // =========================================================================
    // 5. UPDATE BY ID: Returns 200 OK if found & updated, else 404 NOT FOUND [00:15:38]
    // =========================================================================
    @PutMapping("/id/{id}")
    public ResponseEntity<?> updateJournalById(@PathVariable ObjectId id, @RequestBody JournalEntry newEntry) {
        JournalEntry old = journalEntryService.findById(id).orElse(null);
        if (old != null) {
            old.setTitle(newEntry.getTitle() != null && !newEntry.getTitle().equals("")
                    ? newEntry.getTitle()
                    : old.getTitle());
            old.setContent(newEntry.getContent() != null && !newEntry.getContent().equals("")
                    ? newEntry.getContent()
                    : old.getContent());
            journalEntryService.saveEntry(old);
            return new ResponseEntity<>(old, HttpStatus.OK); // 200 OK [00:16:03]
        }
        return new ResponseEntity<>(HttpStatus.NOT_FOUND);   // 404 Not Found [00:16:12]
    }
}

```

*(Bonus Java 8 Functional Tip mentioned at `[00:13:35]`: `getJournalEntryById` ke 5-line `if (journalEntry.isPresent())` ko tum Java 8 Lambda se 1 line mein bhi likh sakte ho: `return journalEntry.map(entry -> new ResponseEntity<>(entry, HttpStatus.OK)).orElseGet(() -> new ResponseEntity<>(HttpStatus.NOT_FOUND));`)*.

> **🎯 How to Articulate in an Interview (English):**
> *"`ResponseEntity<T>` represents the entire HTTP response in Spring Boot, giving us full control over the **HTTP Status Code**, **Response Body**, and **Response Headers**. Instead of returning raw POJOs that default to `200 OK`, wrapping responses in `ResponseEntity` allows us to return accurate REST semantics like `201 Created` on POST, `204 No Content` on DELETE, and `404 Not Found` when an `Optional` query returns empty."*

---

---

## PART B: Project Lombok (Zero Boilerplate POJOs)

**Source:** [Mastering Project Lombok in Java](http://www.youtube.com/watch?v=xmRjF9qycIk)

### 1. Project Lombok Kya Hai Aur Yeh Kaise Kaam Karta Hai?

* **The Problem:** Har Entity/POJO class (`JournalEntry`) mein 4 fields likhne ke baad 40 lines ke Getters, Setters, `NoArgsConstructor`, `AllArgsConstructor`, `toString()`, aur `equals()`/`hashCode()` likhne padte hain jisse class gandi aur cluttered ho jaati hai,.
* **How Lombok Solves It (Compile-Time Bytecode Generation):**
> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Project Lombok kaise kaam karta hai? Aap apni Java file mein Getters aur Setters nahi likhte, sirf upar `@Getter`, `@Setter`, ya `@Data` annotation laga dete ho! Jab aap code **compile** karte ho, toh **Compilation Time ke dauran Lombok ka Annotation Processor automatically woh saare methods generate karke compiled `.class` (bytecode) file ke andar daal deta hai**! Yaani `.java` file ekdum saaf-suthri dikhti hai, aur `.class` file ke andar saare methods maujood rehte hain!"*,



---

### 2. Adding Lombok in `pom.xml` & IntelliJ Setup

1. **`pom.xml` Dependency:**
```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <optional>true</optional>
</dependency>

```


*(Note: Agar tumne `start.spring.io` se Spring Boot 3/4 liya hai toh `<version>` likhne ki zarurat nahi hai, parent POM khud compatible version sambhal lega. Agar IntelliJ mein annotation processing prompt aaye toh `Enable Annotation Processing` par click kar dena)*.

---

### 3. Essential Lombok Annotations Cheat Sheet

| Lombok Annotation | What It Generates in `.class` File at Compile Time |
| --- | --- |
| **`@Getter`** / **`@Setter`** | Saari fields ke लिए `getFieldName()` aur `setFieldName(...)` methods. |
| **`@NoArgsConstructor`** | Bina kisi argument wala default constructor: `public JournalEntry() {}`,. |
| **`@AllArgsConstructor`** | Saari fields ko as parameter lene wala constructor,. |
| **`@ToString`** | Clean `toString()` method. |
| **`@EqualsAndHashCode`** | `equals(Object)` aur `hashCode()` methods. |
| **`@Builder`** | Object creation ke liye **Builder Design Pattern** (`JournalEntry.builder().title("...").build()`). |
| **`@Slf4j`** | Service/Controller mein logging karne ke liye `log` object (`log.info(...)`, `log.error(...)`). |
| **`@Data`** *(The All-in-One Combo)* | **`@Getter` + `@Setter` + `@RequiredArgsConstructor` + `@ToString` + `@EqualsAndHashCode**` (Ek annotation se پانچوں kaam!),. |

---

### 4. Refactored `JournalEntry.java` Using Lombok (`@Data`)

Ab humari 50-line ki `JournalEntry` class ghat kar sirf **15 lines** ki reh gayi,:

```java
package net.engineeringdigest.journalApp.entity;

import lombok.Data;
import lombok.NoArgsConstructor;
import org.bson.types.ObjectId;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;

import java.time.LocalDateTime;

@Document(collection = "journal_entries")
@Data               // Automatically generates Getters, Setters, toString, equals, hashCode at compile-time! [00:04:15]
@NoArgsConstructor  // Good practice to include along with @Data for Jackson/Spring deserialization
public class JournalEntry {

    @Id
    private ObjectId id;
    private String title;
    private String content;
    private LocalDateTime date;
}

```

> **🎯 How to Articulate in an Interview (English):**
> *"Project Lombok is a Java library that hooks into the compiler via the Java Annotation Processing API (JSR 269) to eliminate boilerplate code. Annotations like `@Data`, `@Getter`, `@Setter`, and `@Builder` instruct Lombok to weave the corresponding methods directly into the Abstract Syntax Tree (AST) and compiled `.class` bytecode during compilation, keeping our source `.java` files clean and readable without any runtime performance overhead."*

---

Dono videos ke notes ready hain! Apne `journalApp` mein Lombok ki dependency daal kar `JournalEntry` par `@Data` laga lo aur Controller mein `ResponseEntity` update kar lo. Jab agle video (`@DocumentReference` / Linking Users with Journal Entries) ka link ready ho toh bhejo!