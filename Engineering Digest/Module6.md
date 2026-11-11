# SPRING BOOT MASTER NOTES (MODULE 6): Integrating MongoDB & The 3-Tier Backend Architecture (`Controller -> Service -> Repository`)

**Source Video:** [Step-by-Step Tutorial: How to Integrate MongoDB in Your Spring Boot Application (Engineering Digest)](http://www.youtube.com/watch?v=jvSicVdHKT8)

---

## 1. Step 1 & 2: Adding Dependency & Configuring `application.properties`

Spring Boot mein `@EnableAutoConfiguration` ka asli jaadu yahan dikhta hai—sirf 1 dependency `pom.xml` mein daalne aur `application.properties` mein 3 lines likhne se Spring Boot khud MongoDB server ke sath connection pool bana leta hai [[00:12](https://www.youtube.com/watch?v=jvSicVdHKT8&t=12)], [[00:50](https://www.youtube.com/watch?v=jvSicVdHKT8&t=50)]!

### 1.1 `pom.xml` Dependency (`spring-boot-starter-data-mongodb`)

Apne `pom.xml` ke `<dependencies>` block mein yeh dependency add karke Maven Reload (`Sync`) kar lo [[00:25](https://www.youtube.com/watch?v=jvSicVdHKT8&t=25)]:

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-mongodb</artifactId>
</dependency>

```

---

### 1.2 `src/main/resources/application.properties` Configuration

`.properties` file `key=value` format mein hoti hai (aur `#` se comment lagta hai) [[02:55](https://www.youtube.com/watch?v=jvSicVdHKT8&t=175)], [[03:07](https://www.youtube.com/watch?v=jvSicVdHKT8&t=187)]. Dhyan rahe: keys ke naam exact wahi hone chahiye jo Spring Boot ka Auto-Configuration dhoondhta hai [[01:39](https://www.youtube.com/watch?v=jvSicVdHKT8&t=99)]:

```properties
# =========================================================================
# OPTION A: If using Local MongoDB Server (As shown in this video) [00:01:39]
# =========================================================================
spring.data.mongodb.host=localhost
spring.data.mongodb.port=27017
spring.data.mongodb.database=journaldb
# (Optional in production if auth is enabled on DB) [00:02:38]:
# spring.data.mongodb.username=your_username
# spring.data.mongodb.password=your_password

# =========================================================================
# OPTION B: If using MongoDB Atlas Cloud URI (Your setup!)
# =========================================================================
# spring.data.mongodb.uri=mongodb+srv://<user>:<pass>@cluster0.xxxxx.mongodb.net/journaldb?retryWrites=true&w=majority
# spring.data.mongodb.database=journaldb

```

*(Note: Agar `journaldb` naam ka database ya `journal_entries` naam ka collection pehle se MongoDB mein nahi bana hai, toh ghabrane ki zarurat nahi hai—jaise hi hum Postman se pehli entry save karenge, MongoDB on-the-fly database aur collection dono khud create kar dega!)* [[02:32](https://www.youtube.com/watch?v=jvSicVdHKT8&t=152)], [[15:34](https://www.youtube.com/watch?v=jvSicVdHKT8&t=934)]

---

## 2. The Industry-Standard 3-Tier Architecture (`Controller -> Service -> Repository`)

Abhi tak hum saara code `Controller` mein hi likh rahe the. Lekin production software engineering ki best practice kehti hai ki code ko **3 साफ परतों (Layers)** mein baantna chahiye [[04:11](https://www.youtube.com/watch?v=jvSicVdHKT8&t=251)], [[06:49](https://www.youtube.com/watch?v=jvSicVdHKT8&t=409)]:

```text
+-------------------------------------------------------------------------+
| 1. CONTROLLER LAYER (package: controller -> JournalEntryControllerV2)   |
|    Role: Only exposes REST API endpoints (@GetMapping, @PostMapping)    |
|          and forwards the request to the Service layer.                 |
+-------------------------------------------------------------------------+
                                    |
                                    | calls (@Autowired)
                                    v
+-------------------------------------------------------------------------+
| 2. SERVICE LAYER (package: service -> JournalEntryService)              |
|    Role: Contains all BUSINESS LOGIC (validations, setting timestamps,  |
|          calculations) and calls the Repository layer.                  |
+-------------------------------------------------------------------------+
                                    |
                                    | calls (@Autowired)
                                    v
+-------------------------------------------------------------------------+
| 3. REPOSITORY LAYER (package: repository -> JournalEntryRepository)     |
|    Role: An Interface extending MongoRepository<Entity, IdType> that    |
|          directly talks to the MongoDB Database (CRUD queries).         |
+-------------------------------------------------------------------------+

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Karne ko aap saara database aur business logic `Controller` ke andar hi likh sakte ho—code tab bhi chalega [[06:55](https://www.youtube.com/watch?v=jvSicVdHKT8&t=415)]! Lekin best practice yeh hai ki:
> **`Controller` call karega `Service` ko, aur `Service` call kareगी `Repository` ko!** [[07:04](https://www.youtube.com/watch?v=jvSicVdHKT8&t=424)]
> * **`Controller`:** Sirf HTTP requests lega aur response dega [[04:40](https://www.youtube.com/watch?v=jvSicVdHKT8&t=280)].
> * **`Service`:** Saari katha (Business Logic) yahan likhi jayegi [[04:40](https://www.youtube.com/watch?v=jvSicVdHKT8&t=280)].
> * **`Repository`:** Database mein query chalane ka kaam karegi [[07:35](https://www.youtube.com/watch?v=jvSicVdHKT8&t=455)]."*
> 
> 

---

## 3. Mapping the Entity (`@Document`, `@Id` & `ObjectId`)

Hum apni `JournalEntry` POJO class ko MongoDB ke collection se map karne ke liye **2 annotations** aur **2 naye data types** (`ObjectId` aur `LocalDateTime`) lagate hain [[09:24](https://www.youtube.com/watch?v=jvSicVdHKT8&t=564)], [[19:58](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1198)], [[20:43](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1243)]:

```java
package net.engineeringdigest.journalApp.entity;

import org.bson.types.ObjectId;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;
import java.time.LocalDateTime;

// 1. @Document tells Spring Data MongoDB that each instance of this class represents a Document (Row)
//    in the "journal_entries" collection! (If collection name is omitted, it defaults to class name "journalEntry") [00:09:55], [00:12:38]
@Document(collection = "journal_entries")
public class JournalEntry {

    // 2. @Id maps this field to MongoDB's unique Primary Key ("_id") [00:09:55], [00:10:17]
    //    We use 'ObjectId' (from org.bson.types.ObjectId) which is MongoDB's native auto-generated ID type [00:19:58]
    @Id
    private ObjectId id;

    private String title;
    private String content;

    // 3. Java 8 Date-Time API to automatically store when the entry was created [00:20:43]
    private LocalDateTime date;

    // Getters and Setters [00:20:10], [00:20:48]
    public ObjectId getId() { return id; }
    public void setId(ObjectId id) { this.id = id; }

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }

    public String getContent() { return content; }
    public void setContent(String content) { this.content = content; }

    public LocalDateTime getDate() { return date; }
    public void setDate(LocalDateTime date) { this.date = date; }
}

```

### Important Observations from the Video on `_id` & `_class`:

1. **Why We Changed `id` from `String`/`Long` to `ObjectId`:**
* Agar `id` ka type `String` rakho aur Postman se `"id": 2` bhejo, toh MongoDB `"_id": "2"` (String) save karta hai, aur agar Postman se `id` na bhejo toh MongoDB `ObjectId("...")` save kar deta hai—jisse ek hi collection mein alag-alag data type ki IDs ban jaati hain [[16:33](https://www.youtube.com/watch?v=jvSicVdHKT8&t=993)], [[19:28](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1168)]!
* Is inconsistency ko khatam karne ke liye humne `id` ka type **`ObjectId`** (`org.bson.types.ObjectId`) fix kar diya taaki har document ki ID hamesha MongoDB ke native `ObjectId` format mein hi auto-generate ho [[19:37](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1177)], [[19:58](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1198)].


2. **How `.save()` Behaves When `_id` Already Exists (Insert vs. Upsert/Update):**
Jab `repository.save(entry)` chalta hai, toh agar us `_id` ka document DB mein pehle se **nahi** hai toh woh naya document **Insert** karta hai; lekin agar wahi `_id` DB mein pehle se maujood hai, toh woh naya document banane ki jagah purane document ko **Overwrite/Update** kar deta hai [[17:22](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1042)], [[17:42](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1062)]!
3. **What is the Extra `_class` Field Inside MongoDB Documents?**
Jab tum `mongosh` ya Compass mein saved document dekhoge, toh wahan `_id`, `title`, `content`, `date` ke alawa ek extra field dikhegi:
`"_class" : "net.engineeringdigest.journalApp.entity.JournalEntry"` [[15:58](https://www.youtube.com/watch?v=jvSicVdHKT8&t=958)], [[16:03](https://www.youtube.com/watch?v=jvSicVdHKT8&t=963)].
Spring Data MongoDB yeh **Fully Qualified Class Name** khud save karta hai taaki DB se JSON wapas padhte waqt use pata ho ki kis Java class mein deserialize karna hai [[16:09](https://www.youtube.com/watch?v=jvSicVdHKT8&t=969)].

---

## 4. Layer 3: The Repository Interface (`MongoRepository`)

Hum `repository` package ke andar ek **Interface** (Class nahi!) banate hain aur use **`MongoRepository<Entity, IdType>`** se `extends` करा देते हैं [[05:39](https://www.youtube.com/watch?v=jvSicVdHKT8&t=339)], [[07:29](https://www.youtube.com/watch?v=jvSicVdHKT8&t=449)]:

```java
package net.engineeringdigest.journalApp.repository;

import net.engineeringdigest.journalApp.entity.JournalEntry;
import org.bson.types.ObjectId;
import org.springframework.data.mongodb.repository.MongoRepository;

// Generic Parameter 1: JournalEntry (The Entity/POJO mapped to the collection) [00:08:55]
// Generic Parameter 2: ObjectId     (The data type of the @Id primary key field) [00:09:05], [00:20:16]
public interface JournalEntryRepository extends MongoRepository<JournalEntry, ObjectId> {
    // EMPTY! All standard CRUD methods (.save(), .findAll(), .findById(), .deleteById())
    // are inherited automatically from MongoRepository! [00:07:59], [00:12:18]
}

```

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Dhyan se dekhiye: `JournalEntryRepository` ek **Interface** hai aur humne iske upar `@Component` bhi nahi likha hai, na hi isकी koi implementation class banayi hai! Toh phir `@Autowired` karne par iska Bean kahan se aata hai? **Spring Data MongoDB runtime par parde ke peeche khud is interface ki ek implementation class (Dynamic Proxy Bean) generate kar deta hai aur use IoC Container mein daal deta hai!**"* [[11:17](https://www.youtube.com/watch?v=jvSicVdHKT8&t=677)], [[11:26](https://www.youtube.com/watch?v=jvSicVdHKT8&t=686)]

---

## 5. Layer 2: The Service Class (`JournalEntryService` & `Optional<T>`)

Hum `service` package ke andar `JournalEntryService` class banate hain [[04:50](https://www.youtube.com/watch?v=jvSicVdHKT8&t=290)].

* **⚠️ Don't Forget `@Component` (or `@Service`):** Kyunki Controller ke andar hum `JournalEntryService` ko `@Autowired` karenge, isliye `JournalEntryService` ke upar **`@Component`** lagana mandatory hai—warna woh IoC Container mein register nahi hogi aur `NullPointerException` aayega [[14:03](https://www.youtube.com/watch?v=jvSicVdHKT8&t=843)], [[14:12](https://www.youtube.com/watch?v=jvSicVdHKT8&t=852)]!

```java
package net.engineeringdigest.journalApp.service;

import net.engineeringdigest.journalApp.entity.JournalEntry;
import net.engineeringdigest.journalApp.repository.JournalEntryRepository;
import org.bson.types.ObjectId;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.Optional;

@Component // Registers this Service class as a Bean inside the IoC Container! [00:14:08]
public class JournalEntryService {

    @Autowired // Injects the runtime implementation of JournalEntryRepository [00:10:55]
    private JournalEntryRepository journalEntryRepository;

    // 1. CREATE / UPDATE ENTRY [00:11:44]
    public void saveEntry(JournalEntry journalEntry) {
        journalEntryRepository.save(journalEntry);
    }

    // 2. READ ALL ENTRIES [00:18:26]
    public List<JournalEntry> getAll() {
        return journalEntryRepository.findAll();
    }

    // 3. READ BY ID (Returns Optional<JournalEntry>) [00:21:49]
    public Optional<JournalEntry> findById(ObjectId id) {
        return journalEntryRepository.findById(id);
    }

    // 4. DELETE BY ID [00:22:36]
    public void deleteById(ObjectId id) {
        journalEntryRepository.deleteById(id);
    }
}

```

### What is `Optional<JournalEntry>` Returned by `findById()`?

* Jab hum kisi `id` se database mein document dhoondhte hain, toh ho sakta hai us `id` ki entry database mein maujood ho, ya ho sakta hai na ho [[22:11](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1331)].
* `NullPointerException` se bachane ke liye Java 8 mein **`Optional<T>`** aaya:
> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"`Optional` kuch nahi hai bhai—**`Optional` ek बक्सा (Box) hai jiske andar data ho bhi sakta hai aur nahi bhi ho sakta!** Controller mein hum `.orElse(null)` laga dete hain: yaani agar बक्से mein entry mili toh woh return kar do, warna `null` return kar do!"* [[22:16](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1336)], [[22:20](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1340)]



---

## 6. Layer 1: The Controller Class (`JournalEntryControllerV2`)

```java
package net.engineeringdigest.journalApp.controller;

import net.engineeringdigest.journalApp.entity.JournalEntry;
import net.engineeringdigest.journalApp.service.JournalEntryService;
import org.bson.types.ObjectId;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.List;

@RestController
@RequestMapping("/journal")
public class JournalEntryControllerV2 {

    @Autowired // Injects JournalEntryService Bean [00:13:28]
    private JournalEntryService journalEntryService;

    // 1. GET ALL: GET http://localhost:8080/journal [00:18:50]
    @GetMapping
    public List<JournalEntry> getAll() {
        return journalEntryService.getAll();
    }

    // 2. CREATE: POST http://localhost:8080/journal [00:20:28], [00:21:22]
    @PostMapping
    public JournalEntry createEntry(@RequestBody JournalEntry myEntry) {
        // Automatically set current server timestamp before saving [00:20:33]
        myEntry.setDate(LocalDateTime.now());
        journalEntryService.saveEntry(myEntry);
        return myEntry; // Returns the saved entry (with generated id & date) [00:21:22]
    }

    // 3. GET BY ID: GET http://localhost:8080/journal/id/{myId} [00:22:04]
    @GetMapping("id/{myId}")
    public JournalEntry getJournalEntryById(@PathVariable ObjectId myId) {
        // Unboxes Optional: returns the JournalEntry if present, otherwise returns null [00:22:20]
        return journalEntryService.findById(myId).orElse(null);
    }

    // 4. DELETE BY ID: DELETE http://localhost:8080/journal/id/{myId} [00:22:58]
    @DeleteMapping("id/{myId}")
    public boolean deleteJournalEntryById(@PathVariable ObjectId myId) {
        journalEntryService.deleteById(myId);
        return true;
    }

    // 5. UPDATE BY ID (Partial Update Logic): PUT http://localhost:8080/journal/id/{id} [00:23:28]
    @PutMapping("/id/{id}")
    public JournalEntry updateJournalById(@PathVariable ObjectId id, @RequestBody JournalEntry newEntry) {
        // Step 1: Fetch the existing (old) entry from MongoDB [00:23:32]
        JournalEntry old = journalEntryService.findById(id).orElse(null);

        if (old != null) {
            // Step 2: Only update 'title' if newEntry has a non-null & non-empty title! [00:24:16], [00:28:57]
            old.setTitle(newEntry.getTitle() != null && !newEntry.getTitle().equals("")
                    ? newEntry.getTitle()
                    : old.getTitle());

            // Step 3: Only update 'content' if newEntry has a non-null & non-empty content! [00:24:33]
            old.setContent(newEntry.getContent() != null && !newEntry.getContent().equals("")
                    ? newEntry.getContent()
                    : old.getContent());
        }

        // Step 4: Save the modified 'old' object back (since its _id already exists, MongoDB updates it in-place!) [00:24:53]
        journalEntryService.saveEntry(old);
        return old;
    }
}

```

*(Postman Tip from `[00:26:02]`: Jab Postman mein `GET /journal` chalaoge, toh Jackson `ObjectId` ko JSON mein `{"timestamp": ..., "date": ...}` bana kar dikhata hai, isliye `GET /journal/id/{myId}` test karne ke liye 24-character ki hex ID string `mongosh` ya Compass se copy karke URL mein daalna—aage chal kar hum ise code mein fix bhi karenge)* [[26:06](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1566)].

---

## 7. Pro Skill: How to Debug a Spring Boot API in IntelliJ IDEA

**Source:** [Integrating MongoDB in Spring Boot](http://www.youtube.com/watch?v=jvSicVdHKT8) ([[27:27](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1647)] – [[29:16](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1756)])

Video mein `PUT` API chalate waqt Vipul se galti se `!newEntry.getTitle().equals("")` में **`!` (NOT operator)** chhoot gaya tha, jiski wajah se title `"Evening"` se `"Night"` update nahi ho raha tha [[27:22](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1642)], [[28:53](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1733)]. Use dhoondhne ke liye `System.out.println` lagane ki jagah **IntelliJ Debugger** use kiya gaya:

1. **Place a Breakpoint (Red Dot):** Jis line par code ko rok kar variables check karne hain, uske line number ke bagal mein click karke **Red Dot (Breakpoint)** laga do [[27:40](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1660)].
2. **Start in Debug Mode (Bug Icon 🐞):** Main class par `Run` (▶️) ki jagah **`Debug` (🐞)** par click karo [[27:57](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1677)].
3. **Hit Endpoint from Postman:** Jaise hi Postman mein `Send` dabaoge, code us Breakpoint wali line par **pause (ruk)** jayega aur IntelliJ khud samne khul jayega [[28:06](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1686)].
4. **Step Over (`F8`) & Inspect Live Variables:** **Step Over** button daba kar code ko 1-1 line aage badhao—IntelliJ har line ke bagal mein `old` aur `newEntry` ke andar ki exact values live dikha dega, jisse turant pakad mein aa gaya ki `old.title` change hi nahi hua kyunki `!` missing tha [[28:23](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1703)], [[28:57](https://www.youtube.com/watch?v=jvSicVdHKT8&t=1737)]!

> **🎯 How to Articulate in an Interview (English):**
> *"In a layered Spring Boot application with MongoDB, the `@RestController` handles HTTP routing and delegates business logic to a `@Component`/`@Service` class, which in turn interacts with the database through a repository interface extending `MongoRepository<T, ID>`. At runtime, Spring Data automatically generates a proxy implementation for the repository interface. Domain entities are mapped to MongoDB collections using `@Document` and `@Id` with BSON's `ObjectId`."*

---

Ab apne `journalApp` mein MongoDB Atlas (ya local MongoDB) jod kar `POST`, `GET`, `PUT`, aur `DELETE` chala kar dekh lo—aur jab agla video (Lombok & `ResponseEntity` / HTTP Status Codes) dekho, toh uska link bhejo!