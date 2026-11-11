# SPRING BOOT MASTER NOTES (MODULE 4): Building REST APIs & CRUD Operations (`JournalApp` Kickoff)

**Source Video:** [Creating REST API using Spring Boot in Hindi | A Step-by-Step Tutorial (Engineering Digest)](http://www.youtube.com/watch?v=rxT5RFYxjSg)

---

## 1. Project Setup & Refactoring Best Practices (`JournalApp`)

Is video se hum हवा में isolated examples karne ki jagah ek end-to-end production project (**`journalApp`**) banana start kar rahe hain, jismein aage chal kar MongoDB, Spring Security (Login/Logout), aur sab kuch integrate hoga [[00:11](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=11)].

* **GAV Coordinates Used on Spring Initializr:**
* **Group ID (Reverse Domain Name):** `net.engineeringdigest` (Agar company ki website `engineeringdigest.net` hai, toh Group ID reverse mein `net.engineeringdigest` banti hai) [[01:45](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=105)].
* **Artifact ID (Unique Project Name):** `journalApp` [[02:05](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=125)].
* **Dependency Added:** `Spring Web` (REST APIs banane aur Embedded Tomcat server ke liye) [[02:59](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=179)].


* **IntelliJ Refactoring Tip (`Shift + F6`):**
Spring Initializr ne main class ka naam `JournalAppApplication` rakh diya tha (do baar `App` aane se naam ajeeb lag raha tha) [[03:35](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=215)]. Agar kabhi kisi class ya variable ka naam badalna ho, toh manually edit mat karo—**Right Click $\rightarrow$ `Refactor` $\rightarrow$ `Rename**` use karo taaki poore project mein jahan-jahan woh class use ho rahi hai (`SpringApplication.run(JournalApplication.class, args)` سمیت), har jagah naam automatically update ho jaye [[03:41](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=221)].

---

## 2. What is a REST API? (The Core Formula)

* **Full Form:** **RE**presentational **S**tate **T**ransfer — **A**pplication **P**rogramming **I**nterface [[04:24](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=264)].
* **Representation:** Ek aisi common format/language (jaise **JSON**) jise Client (Frontend/Mobile App) aur Server (Backend) dono samajh sakein [[07:29](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=449)].
* **State Transfer:** Client aur Server ke beech data/information (state) ka len-den [[07:35](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=455)].



> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Jab main bolta hoon ki hum **REST API** banane जा रहे हैं, toh uska matlab sirf URL nahi hota! Sirf URL (`172.1.1.19:8080/Netflix/plans`) se aap server ko access nahi kar paoge—aapko saath mein **HTTP Verb** bhi dena padega! Yaani:*
> 
> $$\text{REST API} = \text{HTTP Verb (GET / POST / PUT / DELETE)} + \text{URL (Endpoint)}$$
> 
> 
> 
> *Dono mil kar ek REST API banate hain!"* [[07:05](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=425)]

### The 4 Standard HTTP Verbs (CRUD Mapping)

| HTTP Verb | Spring Boot Annotation | Database / Resource Action (CRUD) |
| --- | --- | --- |
| **`GET`** | `@GetMapping` | **Read:** Server se data sirf dekhna/mangwana hai [[06:35](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=395)]. |
| **`POST`** | `@PostMapping` | **Create:** Server par koi naya resource/entry create karni hai [[06:41](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=401)]. |
| **`PUT`** | `@PutMapping` | **Update:** Pehle se maujood kisi resource ko modify/update karna hai [[06:53](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=413)]. |
| **`DELETE`** | `@DeleteMapping` | **Delete:** Kisi resource ko server se hatana/delete karna hai [[06:57](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=417)]. |

> **🎯 How to Articulate in an Interview (English):**
> *"REST (Representational State Transfer) is an architectural style where a client and server exchange data representations—typically in JSON format—over stateless HTTP calls. A RESTful API endpoint is uniquely identified by the combination of an **HTTP Verb** (`GET`, `POST`, `PUT`, `DELETE`) defining the action and a **URI path** identifying the resource."*

---

## 3. `@RestController` & Your First Health-Check Endpoint

Hum apne base package (`net.engineeringdigest.journalApp`) ke andar ek naya sub-package banate hain: **`controller`** [[08:22](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=502)].

### `@RestController` Ki 2 Superpowers:

1. **Creates a Spring Bean:** Yeh `@Component` ka hi ek specialized रूप (child annotation) hai, toh IoC Container is class ka object (Bean) khud bana leta hai [[08:50](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=530)].
2. **Automatic JSON Serialization (`@ResponseBody` built-in):** Is class ke andar ke methods jo bhi Java Object ya `List` return karenge, `@RestController` use automatically **JSON** mein convert karke HTTP response mein bhej dega [[10:22](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=622)]!

```java
package net.engineeringdigest.journalApp.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HealthCheck {

    @GetMapping("/health-check") // Maps HTTP GET -> http://localhost:8080/health-check [00:10:47]
    public String healthCheck() {
        return "Ok"; // [00:10:09]
    }
}

```

* **Why Use Postman Instead of a Browser?**
Browser ke address bar se jab tum कोई URL hit karte ho, toh woh hamesha **`GET` request** hi bhejta hai [[11:34](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=694)]. Agar tumhe `POST`, `PUT`, ya `DELETE` requests test karni hain ya JSON Request Body bhejni hai, toh uske liye **Postman** use hota hai [[11:20](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=680)]. *(Agar Postman mein `/health-check` par `GET` ki jagah `POST` select karke Send karoge, toh **`405 Method Not Allowed`** error aayega kyunki humne sirf `@GetMapping` likha hai)* [[12:03](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=723)].

---

## 4. Creating the POJO / Entity Class (`JournalEntry`)

Journal entry koi simple `String` nahi hoti—usmein `id`, `title`, aur `content` teen cheezein hongi [[12:28](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=748)]. Isliye hum base package ke andar ek **`entity`** package banate hain aur usmein **`JournalEntry`** class likhte hain [[12:25](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=745)]:

```java
package net.engineeringdigest.journalApp.entity;

// POJO: Plain Old Java Object [00:12:58]
public class JournalEntry {
    private long id;        // [00:12:40]
    private String title;
    private String content;

    // Getters and Setters (Required by Jackson to convert JSON <-> Java Object!) [00:12:53]
    public long getId() { return id; }
    public void setId(long id) { this.id = id; }

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }

    public String getContent() { return content; }
    public void setContent(String content) { this.content = content; }
}

```

* **What is a POJO?** **P**lain **O**ld **J**ava **O**bject—ek simple Java class jismein private fields aur unke Getters/Setters hote hain (bina kisi heavy framework restriction ke) [[12:58](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=778)].
* **Why Getters/Setters Are Mandatory Here:** Jab Postman se JSON aata hai ya wapas JSON jaata hai, toh Spring Boot ke andar **Jackson** library invok hoti hai jo inhi Getters aur Setters ko call karke JSON ko Java Object mein convert karti hai.

---

## 5. Deep-Dive: The 3 Core Parameter & Routing Annotations

Code likhne se pehle in **3 annotations** ka exact role aur syntax samajh lo:

### 5.1 `@RequestMapping("/journal")` on the Class

* Agar tum Controller class ke upar `@RequestMapping("/journal")` laga dete ho, toh us class ke andar likhe **har method ke URL ke aage `/journal` automatically jud jayega** [[14:28](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=868)]!
* **Same URL + Different HTTP Verb Rule:**
Agar `getAll()` par `@GetMapping` (bina kisi sub-path ke) laga hai aur `createEntry()` par `@PostMapping` (bina kisi sub-path ke) laga hai, toh **dono ka URL `/journal` hi hai** [[16:06](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=966)]!
* Jab `GET /journal` aayega $\rightarrow$ `getAll()` chalega [[16:11](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=971)].
* Jab `POST /journal` aayega $\rightarrow$ `createEntry()` chalega [[16:16](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=976)].
* **⚠️ Ambiguous Mapping Crash:** Lekin agar ek hi class mein **do methods par `@GetMapping` bina alag path ke** laga diya (dono `GET /journal` ban gaye), toh **Spring Boot application start hi nahi hogi (`Ambiguous mapping` error dekar crash ho jayegi)** [[18:58](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1138)]! Same cheez tab hogi agar `getById` aur `deleteById` dono par galti se `@GetMapping("id/{myId}")` laga diya [[23:18](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1398)].



---

### 5.2 `@RequestBody` (Reading JSON Payload from Postman)

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"`@RequestBody` ka matlab hai: Hey Spring! Postman (Client) ne HTTP Request ki **Body** ke andar jo JSON bheja hai, use uthao aur meri Java POJO class (`JournalEntry`) ke object mein convert karke `myEntry` variable ke andar daal do!"* [[17:12](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1032)]

```json
// JSON sent in Postman Body (raw -> JSON) [00:16:58]:
{
    "id": 1,
    "title": "Morning",
    "content": "Morning was good"
}

```

```java
// Spring automatically maps the JSON above into 'myEntry' [00:17:41]:
public boolean createEntry(@RequestBody JournalEntry myEntry)

```

---

### 5.3 `@PathVariable` vs. `@RequestParam` (Query Parameter)

Jab URL ke zariye koi single value (jaise `id = 2`) bhejni ho, toh 2 tareeke hote hain [[20:34](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1234)]:

| Feature | 1. Path Variable (`@PathVariable`) | 2. Request / Query Parameter (`@RequestParam`) |
| --- | --- | --- |
| **URL Look** | `localhost:8080/journal/id/2` *(Slash `/` ke baad)* [[20:58](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1258)] | `localhost:8080/journal?name=vipul` *(Question mark `?` ke baad `key=value`)* [[20:46](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1246)] |
| **Controller Mapping** | `@GetMapping("id/{myId}")` *(Curly braces `{}` batate hain ki `myId` dynamic variable hai)* [[21:32](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1292)] | `@GetMapping("/journal")` |
| **Method Parameter** | `(@PathVariable Long myId)` [[21:41](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1301)] | `(@RequestParam String name)` |

---

## 6. Complete In-Memory CRUD Controller Code (`JournalEntryController`)

Kyunki abhi humne Database (MongoDB) connect nahi kiya hai, isliye humne Controller ke andar ek `Map<Long, JournalEntry> journalEntries = new HashMap<>();` le liya hai jo ek temporary in-memory table ki tarah kaam karega (server restart karne par yeh Map khali ho jayega) [[13:39](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=819)], [[22:02](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1322)]:

```java
package net.engineeringdigest.journalApp.controller;

import net.engineeringdigest.journalApp.entity.JournalEntry;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/journal") // Base URL prefix for all endpoints in this controller [00:14:28]
public class JournalEntryController {

    // Temporary In-Memory "Database Table" (Key = ID, Value = JournalEntry object) [00:13:44]
    private Map<Long, JournalEntry> journalEntries = new HashMap<>();

    // =========================================================================
    // 1. READ ALL: GET http://localhost:8080/journal [00:14:03]
    // =========================================================================
    @GetMapping
    public List<JournalEntry> getAll() {
        return new ArrayList<>(journalEntries.values());
    }

    // =========================================================================
    // 2. CREATE: POST http://localhost:8080/journal (With JSON in Request Body) [00:15:28]
    // =========================================================================
    @PostMapping
    public boolean createEntry(@RequestBody JournalEntry myEntry) {
        journalEntries.put(myEntry.getId(), myEntry); // [00:17:55]
        return true;
    }

    // =========================================================================
    // 3. READ BY ID: GET http://localhost:8080/journal/id/2 [00:19:34]
    // =========================================================================
    @GetMapping("id/{myId}")
    public JournalEntry getJournalEntryById(@PathVariable Long myId) {
        return journalEntries.get(myId); // Fetches entry from Map in O(1) time [00:21:54]
    }

    // =========================================================================
    // 4. DELETE BY ID: DELETE http://localhost:8080/journal/id/2 [00:22:58]
    // =========================================================================
    @DeleteMapping("id/{myId}")
    public JournalEntry deleteJournalEntryById(@PathVariable Long myId) {
        return journalEntries.remove(myId); // Removes and returns the deleted entry [00:23:14], [00:23:54]
    }

    // =========================================================================
    // 5. UPDATE BY ID: PUT http://localhost:8080/journal/id/2 (Path ID + JSON Body) [00:23:58]
    // =========================================================================
    @PutMapping("/id/{id}")
    public JournalEntry updateJournalById(@PathVariable Long id, @RequestBody JournalEntry myEntry) {
        return journalEntries.put(id, myEntry); // Overwrites value at 'id' and returns old entry [00:24:28]
    }
}

```

*(Note on `PUT` Return Value at `[00:26:05]`: `HashMap.put(id, myEntry)` Map mein nayi value daal deta hai aur **purani value** return karta hai—isliye Postman mein `PUT` chalate waqt response mein purana object dikhta hai, par agle `GET` call mein updated `"I am happy"` dikhne lagta hai!)* [[26:05](https://www.youtube.com/watch?v=rxT5RFYxjSg&t=1565)]

> **🎯 How to Articulate in an Interview (English):**
> *"In Spring Boot, `@RequestMapping` at the class level defines a base URI prefix for all handler methods inside a `@RestController`. Incoming data is bound to method parameters using `@PathVariable` (to extract dynamic segments like `{id}` from the URI path), `@RequestParam` (to extract query string parameters after `?`), and `@RequestBody` (which uses Jackson's `HttpMessageConverter` to deserialize the incoming JSON payload into a Java POJO)."*

---

Yeh rahe tumhare scheduled `REST API` video ke complete notes! Isko apne project mein code karke Postman se `GET`, `POST`, `PUT`, aur `DELETE` chala kar dekh lena, aur jab agla video dekho toh link bhejo!
