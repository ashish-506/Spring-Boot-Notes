# SPRING BOOT MASTER NOTES (MODULE 9): Database Transactions, ACID Properties, `@Transactional` & `PlatformTransactionManager`

**Source Video:** [@Transactional Annotation in Spring Boot Example in Hindi | @Enabletransactionmanagement (Engineering Digest)](http://www.youtube.com/watch?v=6oxyNgZSz9s)

---

## 1. The Big Picture: The Partial-Failure / Data Inconsistency Bug

Pichle video me humne `saveEntry(JournalEntry journalEntry, String userName)` likha tha jo **2 alag-alag collections** (`journal_entries` aur `users`) me data save karta hai [[00:22](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=22)].

Ab dekho agar in दो steps ke beech me koi runtime error aa jaye toh kya blunder hota hai (`[00:01:12]` – `[00:04:04]`):

```java
public void saveEntry(JournalEntry journalEntry, String userName) {
    User user = userService.findByUserName(userName);
    journalEntry.setDate(LocalDateTime.now());
    
    // STEP 1: Saves entry in 'journal_entries' collection (SUCCEEDS!) [00:02:54]
    JournalEntry saved = journalEntryRepository.save(journalEntry); 
    
    user.getJournalEntries().add(saved);
    
    // DELIBERATE BUG: userName is marked @NonNull in User.java! [00:01:34]
    // Setting it to null immediately throws a NullPointerException here! [00:01:41], [00:03:30]
    user.setUserName(null); 
    
    // STEP 2: Saves updated user in 'users' collection (NEVER EXECUTES!) [00:03:49]
    userService.saveEntry(user); 
}

```

### What Happened in the Database? (`[00:03:53]`)

1. `journal_entries` collection me `"ABCD"` naam ki nayi entry **save ho gayi** [[03:53](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=233)].
2. Lekin `user.setUserName(null)` par `NullPointerException` phat gaya, jiski wajah se `userService.saveEntry(user)` wali line chali hi nahi [[03:49](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=229)]!
3. **Result (Data Inconsistency / Orphan Record):** `journal_entries` me woh entry laawaris padi hai, par `users` collection me `vipul` ki list me uska koi reference hi nahi aaya [[03:59](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=239)]!

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Jab ek method ke andar multiple database operations ho rahe hon, toh woh **Transaction** hona chahiye! Transaction ka matlab hai: **Ya toh saare operations ek saath successfully complete honge (Commit), aur agar unme se ek bhi fail hua toh pehle jo operations chal chuke hain woh bhi wapas pehle jaisi state me chale jayenge (Rollback)!**"* [[04:14](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=254)], [[04:29](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=269)]

---

## 2. ACID Properties Achieved by Spring Transactions (`[00:05:43]` – `[00:06:57]`)

Jab hum kisi method ko transactional banate hain, toh Spring Boot har method call ke liye ek **Transactional Context (Container/Session)** kholta hai [[05:51](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=351)], [[10:16](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=616)]:

* **1. Atomicity (All-or-Nothing) `[00:06:13]`:**
Poore method ke andar likhe saare DB operations ko **ek single unit (atomic operation)** maana jayega [[06:08](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=368)]. Agar `userService.saveEntry(user)` fail hua, toh `journalEntryRepository.save(journalEntry)` ne jo document insert kiya tha, use bhi **Rollback (undo)** kar diya jayega [[04:59](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=299)], [[06:13](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=373)].
* **2. Isolation (Concurrent Requests Don't Mix) `[00:06:26]`:**
Maan lo do alag users (`Ram` aur `Shyam`) ek hi microsecond par `POST /journal` hit karte hain [[06:35](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=395)]. Spring Boot dono requests ke liye **2 alag-alag isolated Transactional Contexts (baksa)** banayega [[06:40](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=400)]. `Ram` ke transaction me kya ho raha hai ya woh fail ho raha hai, uska `Shyam` ke transaction par 0% asar padega [[06:47](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=407)].

---

## 3. Deep-Dive: Every New Annotation & Interface Used (What Problem It Solves & What Happens Without It!)

Spring Boot + MongoDB me Transaction chalane ke liye **3 cheezon ki तिगड़ी (Trio)** lagti hai. Ek bhi chhoota toh transaction kaam nahi karega:

---

### 3.1 `@Transactional` (From `org.springframework.transaction.annotation.Transactional`) [[04:38](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=278)]

* **Kahan Lagta Hai:** `JournalEntryService` ke `saveEntry(...)` method ke upar (या class level par) [[04:38](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=278)].
* **Kaunsi Problem Solve Karta Hai?**
Yeh Spring ko batata hai ki is method ke start hote hi ek **Database Transaction Session** shuru karo [[04:48](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=288)]. Agar poora method bina kisi runtime exception ke khatam ho jaye toh **Commit** kar do, aur agar method se koi unhandled RuntimeException bahar nikle toh saare DB changes **Rollback** kar do [[04:59](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=299)].
* **❌ Agar Isko Use NA Karein Toh Kya Hoga?**
Har `repository.save()` apni alag independent auto-commit query ban kar chalega. Pehla save commit ho jayega aur doosra fail hone par pehla rollback nahi hoga (Orphan data in DB) [[03:59](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=239)].

---

### 3.2 `@EnableTransactionManagement` [[05:20](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=320)]

* **Kahan Lagta Hai:** Main `@SpringBootApplication` class (`JournalApplication.java`) ke upar, ya kisi `@Configuration` class ke upar [[05:20](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=320)], [[15:16](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=916)].
* **Kaunsi Problem Solve Karta Hai?**
Sirf `@Transactional` likhne se kuch nahi hota [[05:04](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=304)]! `@EnableTransactionManagement` Spring ko order deta hai ki poore project me scan karke woh saare methods dhoondho jin par `@Transactional` laga hai, aur unke charo taraf **Transactional Proxy (Transactional Context)** khada kar do [[05:28](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=328)], [[05:51](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=351)].
* **❌ Agar Isko Use NA Karein Toh Kya Hoga?**
Agar transaction management enabled nahi hai, toh `@Transactional` sirf ek بے असर (inactive) sticker ban kar reh jayega—Spring use ignore kar dega aur koi rollback nahi hoga [[05:04](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=304)].

---

### 3.3 `@Bean` of `PlatformTransactionManager` (`MongoTransactionManager`) [[07:12](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=432)] – [[10:47](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=647)]

* **Kahan Lagta Hai:** `JournalApplication.java` (ya `TransactionConfig.java`) ke andar ek method ke upar [[08:45](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=525)], [[15:07](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=907)].
* **Kaunsi Problem Solve Karta Hai? (The Architecture Breakdown):**
1. **`PlatformTransactionManager` (The Interface) `[00:07:12]`:** Spring Framework ke andar commit aur rollback karne ka main Boss yeh interface hai [[07:20](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=440)]. Lekin kyunki duniya me alag-alag databases hain (MySQL, MongoDB, Postgres), isliye `PlatformTransactionManager` sirf ek interface hai [[07:29](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=449)].
2. **`MongoTransactionManager` (The Implementation Class) `[00:07:33]`:** MongoDB ke andar session kholna, commit karna, aur abort/rollback karna is concrete class ke andar likha hota hai [[07:41](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=461)].
3. **`MongoDatabaseFactory` (`SimpleMongoClientDatabaseFactory`) `[00:09:29]`:** `MongoTransactionManager` ko database se connect hone aur sessions banane ke liye `MongoDatabaseFactory` chahiye hoti hai [[09:41](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=581)], [[10:11](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=611)]. Jab humne `application.properties` me MongoDB ki details daali thi, tab Spring Boot ne parde ke peeche `SimpleMongoClientDatabaseFactory` naam ki Bean pehle se bana kar IoC Container me rakh li thi—woh automatically is `@Bean` method ke parameter me inject ho jaati hai [[15:47](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=947)], [[16:04](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=964)]!


* **❌ Agar Yeh `@Bean` Create NA Karein Toh Kya Hoga?**
Spring Boot by default MongoDB ke liye `MongoTransactionManager` ki Bean **auto-create NAHI karta**! Agar tumne `@Transactional` lagaya aur yeh `@Bean` nahi banayi, toh `saveEntry` call hote hi server **`NoSuchBeanDefinitionException: No qualifying bean of type 'org.springframework.transaction.PlatformTransactionManager' available`** dekar crash ho jayega [[07:59](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=479)], [[15:40](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=940)]!

```text
+-------------------------------------------------------------------------+
| @Transactional on saveEntry()                                           |
+-------------------------------------------------------------------------+
                                    |
                                    | intercepted by
                                    v
+-------------------------------------------------------------------------+
| @EnableTransactionManagement                                            |
| (Looks for a Bean of type: PlatformTransactionManager in IoC Container) |
+-------------------------------------------------------------------------+
                                    |
                                    | delegates commit() / rollback() to
                                    v
+-------------------------------------------------------------------------+
| @Bean MongoTransactionManager (implements PlatformTransactionManager)   |
|       └── Uses: MongoDatabaseFactory (SimpleMongoClientDatabaseFactory) |
|                 to open/commit/abort MongoDB Client Sessions!           |
+-------------------------------------------------------------------------+

```

---

## 4. Complete Code Setup (`JournalApplication.java` & `JournalEntryService.java`)

### 4.1 Registering the `PlatformTransactionManager` Bean in `JournalApplication.java` (`[00:08:56]` – `[00:10:47]`)

Kyunki `@SpringBootApplication` ke andar pehle se hi **`@Configuration`** maujood hota hai, hum seedha apni main class ke andar `@Bean` method likh sakte hain (ya alag `config/TransactionConfig.java` class bana kar us par `@Configuration` aur `@EnableTransactionManagement` laga sakte hain) [[08:32](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=512)], [[15:07](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=907)]:

```java
package net.engineeringdigest.journalApp;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.data.mongodb.MongoDatabaseFactory;
import org.springframework.data.mongodb.MongoTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

@SpringBootApplication
@EnableTransactionManagement // 1. Tells Spring to scan for @Transactional methods [00:05:20]
public class JournalApplication {

    public static void main(String[] args) {
        SpringApplication.run(JournalApplication.class, args);
    }

    // 2. Registers MongoTransactionManager as the PlatformTransactionManager Bean in IoC Container [00:08:51]
    //    Method name ('add' / 'falana' / 'transactionManager') doesn't matter; Spring matches by Return Type! [00:10:36]
    @Bean
    public PlatformTransactionManager add(MongoDatabaseFactory dbFactory) {
        return new MongoTransactionManager(dbFactory); // [00:09:10], [00:10:21]
    }
}

```

---

### 4.2 Deadly Trap in `@Transactional`: Swallowing Exceptions in `try-catch`! (`[00:14:04]` – `[00:14:43]`)

Video ke `[00:14:04]` par Vipul ne ek bohot bada production trap bataya hai:

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER (THE `try-catch` ROLLBACK TRAP):**
> *"Agar aapne `@Transactional` method ke andar `try-catch` laga diya aur `catch` block me sirf `System.out.println(e)` karke chhod diya, **toh Rollback NAHI HOGA!** Kyun? Kyunki aapne exception ko andar hi dabaa (catch kar) liya, toh `@Transactional` proxy ko lagega ki method bina kisi error ke मक्खन ki tarah chal gaya aur woh changes ko **Commit** kar dega! Isliye `catch` block ke andar **`throw new RuntimeException("...", e);`** karna 100% zaroori hai taaki `@Transactional` ko pata chale ki kuch phata hai aur use Rollback karna hai!"* [[14:09](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=849)], [[14:35](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=875)]

```java
//Inside service/JournalEntryService.java [00:04:38], [00:14:09]:

@Transactional // Treats the entire method as a single atomic unit [00:04:48]
public void saveEntry(JournalEntry journalEntry, String userName) {
    try {
        User user = userService.findByUserName(userName);
        journalEntry.setDate(LocalDateTime.now());
        JournalEntry saved = journalEntryRepository.save(journalEntry);
        user.getJournalEntries().add(saved);
        // user.setUserName(null); // <-- If uncommented, throws NPE and rolls back 'saved' journalEntry!
        userService.saveEntry(user);
    } catch (Exception e) {
        System.out.println(e);
        // CRITICAL: Must re-throw RuntimeException so @Transactional triggers a ROLLBACK! [00:14:09], [00:14:22]
        throw new RuntimeException("An error occurred while saving the entry.", e);
    }
}

```

---

## 5. Why Local MongoDB Failed with Error Code 20 (& Why Your Atlas Setup Already Works!)

**Source:** `[00:12:21]` – `[00:13:48]`

Jab Vipul ne apne local Ubuntu MongoDB (`localhost:27017`) par `@Transactional` chala kar request bheji, toh console me yeh error phata [[12:26](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=746)]:

> **`Command failed with error 20 (IllegalOperation): 'Transaction numbers are only allowed on a replica set member or mongos'`** [[12:29](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=749)]

### Why Did This Happen?

* **MongoDB's Architectural Rule:** MongoDB me Multi-Document Transactions **sirf tabhi chalte hain jab MongoDB ek Replica Set (multiple synced nodes/copies) par chal raha ho** [[12:36](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=756)], [[12:44](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=764)]!
* Jab hum laptop par default local MongoDB (`localhost:27017`) chalate hain, toh woh **Standalone (Single Instance)** mode me chalta hai jisme replication nahi hoti—isliye local MongoDB par `@Transactional` error 20 de deta hai [[12:36](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=756)].
* **Good News for You:** Kyunki tumne pehle hi local MongoDB ki jagah **MongoDB Atlas (Cloud)** connect kar rakha hai (jisme free tier `M0` cluster bhi by default **3-Node Replica Set** hota hai — yaad karo tumhare pichle console screenshot me `Discovered replica set primary` likha aa raha tha!), **tumhare project me `@Transactional` abhi se 100% bina kisi error ke मक्खन ki tarah chalega!** [[13:14](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=794)], [[13:42](https://www.youtube.com/watch?v=6oxyNgZSz9s&t=822)]

> **🎯 How to Articulate in an Interview (English):**
> *"In Spring Boot, `@Transactional` ensures atomicity and isolation across multiple database operations by binding them to a single transactional context managed by `PlatformTransactionManager` (specifically `MongoTransactionManager` for MongoDB, which uses `MongoDatabaseFactory` to manage client sessions). If an unhandled `RuntimeException` escapes the `@Transactional` method, the transaction manager automatically rolls back all prior operations in that session. Note that MongoDB requires a **Replica Set** topology (such as MongoDB Atlas) to support multi-document ACID transactions."*

---

Kyunki tum pehle se MongoDB Atlas par ho, tumhe agला video (jisme sirf Atlas account banana sikhaya hai) bas 2x par skip/skim karna padega. Ab apne code me `@EnableTransactionManagement` aur `PlatformTransactionManager` ki `@Bean` daal kar test kar lo, aur agla link bhejo!