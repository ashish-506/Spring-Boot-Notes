# SPRING BOOT MASTER NOTES (MODULE 5): MongoDB Basics, ORM vs. JPA vs. Hibernate & Spring Data Architecture

**Compiled Source Videos (Engineering Digest — Videos 7 & 8):**

1. [A Beginner's Walkthrough to MongoDB: Key Concepts Simplified](http://www.youtube.com/watch?v=3Dc8YxC0p-s)
2. [Understanding ORM, JPA, and Spring Data JPA: A Step-by-Step Tutorial](http://www.youtube.com/watch?v=ddrCOI0buBA)

---

## 1. Why Do We Need a Database Now? (In-Memory Map vs. Persistent Storage)

**Source:** [A Beginner's Walkthrough to MongoDB](http://www.youtube.com/watch?v=3Dc8YxC0p-s)

Pichle video mein humne `JournalEntryController` ke andar ek simple `HashMap` (`private Map<Long, JournalEntry> journalEntries`) liya tha.

* **The Problem:** Woh Map RAM (In-Memory) mein rehta hai—jaise hi hum apni Spring Boot application ko restart karte hain, saari journal entries gayab ho jaati hain (kyunki woh **Persistent Storage** nahi hai).
* **The Solution:** Hum **MongoDB** (ya MySQL) jaisa database use karenge taaki server 100 baar bhi restart ho, humara data hard disk/cloud par permanently save rahe.

---

## 2. MongoDB Core Terminologies (MySQL vs. MongoDB Mapping)

**Source:** [A Beginner's Walkthrough to MongoDB](http://www.youtube.com/watch?v=3Dc8YxC0p-s)

Agar tumने kabhi Relational Database (MySQL) dekha hai, toh MongoDB (NoSQL) ke terms usse seedha aise map hote hain:

| Relational DB (MySQL / PostgreSQL) | NoSQL Document DB (MongoDB) | Example (`School` DB) |
| --- | --- | --- |
| **Database** | **Database** | `school` |
| **Table** | **Collection** | `students` (Table ki jagah Collection hota hai) |
| **Row** (Record / Tuple) | **Document** (JSON / BSON object) | `{ "_id": ObjectId("..."), "name": "Ram", "age": 20 }` |
| **Column** (Attribute) | **Field** (Key) | `name`, `age`, `_id` |
| **Primary Key** (`id`) | **`_id` (ObjectId)** | Har document insert hote hi MongoDB khud ek unique `_id` generate kar deta hai. |

### Quick MongoDB Shell (`mongosh`) Commands Shown in Video

*(Note: Yeh commands sirf overview ke liye hain—Spring Boot mein humein terminal se yeh commands nahi chalani padengi, saara kaam Java code se hoga)*:

```javascript
// 1. Show all databases:
show dbs

// 2. Switch to (or create on-the-fly) a database named 'school':
use school

// 3. Insert a JSON document into 'students' collection (creates collection automatically):
db.students.insertOne({ "name": "Ram", "age": 20 })
db.students.insertOne({ "name": "Shyam", "age": 200 })

// 4. Fetch all documents in 'students' collection:
db.students.find().pretty()

// 5. Fetch with a filter condition (WHERE name = 'Ram'):
db.students.find({ "name": "Ram" })

// 6. Delete a document matching a filter:
db.students.deleteOne({ "name": "Ram" })

```

---

## 3. The Most Asked Interview Topic: ORM vs. JPA vs. Hibernate vs. Spring Data JPA

**Source:** [Understanding ORM, JPA, and Spring Data JPA](http://www.youtube.com/watch?v=ddrCOI0buBA)

SDE Interviews mein 90% candidates in 4 terms (`ORM`, `JPA`, `Hibernate`, `Spring Data JPA`) mein ulajh jaate hain. Video mein Vipul ne in चारों ki exact परतें (layers) kholi hain:

---

### Layer 1: ORM (Object-Relational Mapping) — *The Concept / Technique*

* **Kya Hai Yeh?** ORM koi software ya library nahi hai—**ORM ek Technique (सोच/तरीका) hai** jiske zariye hum Java ke **Objects (Classes)** ko Relational Database (MySQL/Postgres) ke **Tables** se map karte hain.
* **Kaise Kaam Karta Hai:**
* Java Class (`User`) $\longleftrightarrow$ Database Table (`users`)
* Class ke Fields (`id`, `name`) $\longleftrightarrow$ Table ke Columns (`id`, `name`)
* Class ka 1 Object (`new User(...)`) $\longleftrightarrow$ Table ki 1 Row


* **Fayda:** Developer ko raw SQL queries (`INSERT INTO...`, `SELECT * FROM...`) haath se nahi likhni padti; aap Java object par operation karte ho aur parde ke peeche database table mein kaam ho jaata hai.

---

### Layer 2: JPA (Java/Jakarta Persistence API) — *The Specification (Set of Rules)*

* **Breakdown:** **J**ava + **P**ersistence *(permanently store karna)* + **A**PI *(Set of rules/interfaces)*.
* **Kya Hai Yeh?** JPA ORM ko achieve karne ke liye **sirf ek Specification (Set of Rules, Interfaces & Annotations jaise `@Entity`, `@Id`)** hai.

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"**JPA khud koi implementation NAHI hai!** JPA sirf ek Interface / Set of Rules hai jo batata hai ki ORM kaise hona chahiye—JPA khud database se baat nahi kar sakta! JPA ke rules ko asal mein chalane (implement karne) ke liye humein ek **Persistence Provider (ya ORM Tool)** jaise **Hibernate** chahiye hota hai!"*

---

### Layer 3: Persistence Providers / ORM Tools (`Hibernate`, `EclipseLink`, `OpenJPA`) — *The Real Engine*

* Kyunki JPA sirf ek interface (rulebook) hai, us interface ko implement karne wali actual libraries ko **Persistence Providers** (ya **ORM Tools**) kehte hain.
* **Examples:** **Hibernate** (Sabse popular aur Spring Boot ka default), **EclipseLink**, aur **OpenJPA**.
* Jab tum JPA ke annotations (`@Entity`) lagate ho, toh parde ke peeche **Hibernate** hi woh asli engine hai jo SQL queries generate karke MySQL database par chalata hai!

---

### Layer 4: Spring Data JPA — *The Top-Level Simplifier*

* Plain JPA/Hibernate use karne par bhi humein `EntityManager` wagairah ka thoda boilerplate code likhna padta tha.
* **Spring Data JPA** JPA ke upar bani ek **Higher-Level Abstraction layer** hai (dhyan rahe: Spring Data JPA khud koi JPA implementation nahi hai—woh andar se **Hibernate** ko hi use karta hai!).
* Yeh humein `JpaRepository` jaisi superpowers deta hai jisse **bina 1 line ka code ya SQL likhe** sirf interface banane se saare CRUD operations (`save()`, `findById()`, `deleteById()`) free mein mil jaate hain!

```text
+-------------------------------------------------------------------+
|  4. SPRING DATA JPA  (High-level abstraction: JpaRepository)      |
+-------------------------------------------------------------------+
                                 | uses
                                 v
+-------------------------------------------------------------------+
|  2. JPA (Jakarta Persistence API)  (Only Interfaces/Rules/@Entity)|
+-------------------------------------------------------------------+
                                 | implemented by
                                 v
+-------------------------------------------------------------------+
|  3. HIBERNATE (Persistence Provider / ORM Tool - Generates SQL)   |
+-------------------------------------------------------------------+
                                 | achieves
                                 v
+-------------------------------------------------------------------+
|  1. ORM (Concept: Mapping Java Objects <-> Relational DB Tables)  |
+-------------------------------------------------------------------+

```

> **🎯 How to Articulate in an Interview (English):**
> *"**ORM (Object-Relational Mapping)** is the conceptual technique of mapping Java objects to relational database tables. **JPA (Jakarta Persistence API)** is merely a specification—a set of interfaces and annotations defining how ORM should work in Java, without any concrete implementation. **Hibernate** is the actual ORM tool (Persistence Provider) that implements the JPA specification under the hood. Finally, **Spring Data JPA** is a high-level abstraction built on top of JPA/Hibernate that eliminates boilerplate DAO code by generating queries automatically from repository interfaces."*

---

## 4. Crucial Twist: Kya MongoDB Mein JPA / Hibernate Use Hota Hai?

**Source:** [Understanding ORM, JPA, and Spring Data JPA](http://www.youtube.com/watch?v=ddrCOI0buBA)

> **🔥 VIPUL'S EMPHASIS / MUST REMEMBER:**
> *"Abhi tak humne jitni kahaani padhi (**ORM, JPA, Hibernate, Spring Data JPA**), woh **SIRF Relational Databases (MySQL, PostgreSQL)** ke liye hoti hai jahan Tables aur Pre-defined Schema hota hai! **MongoDB ke sath JPA ya Hibernate use NAHI hota!** Kyunki MongoDB ek schema-less NoSQL Document database hai!"*

* **Toh MongoDB Mein Kya Use Hota Hai?**
MongoDB ke liye Spring ne bilkul `Spring Data JPA` ka judwa bhai banaya hai jiska naam hai: **`Spring Data MongoDB`** (`spring-boot-starter-data-mongodb` dependency)!
* `Spring Data MongoDB` khud hi MongoDB ke liye Persistence Provider aur abstraction dono ka kaam karta hai (`MongoRepository` aur `@Document` ke zariye).
* **Sabse Bada Fayda:** `Spring Data JPA` (MySQL) aur `Spring Data MongoDB` (MongoDB) dono mein code likhne ka tareeka **95% IDENTICAL** hai! Dono mein database se baat karne ke liye wahi **2 mediums** use hote hain:

---

### The 2 Ways to Interact with Database in Spring Data (Both JPA & MongoDB):

| Medium | Kya Hota Hai Aur Kab Use Karte Hain? | Example Preview |
| --- | --- | --- |
| **1. Query Method DSL** *(Domain Specific Language)* | **For Simple & Medium Queries:** Ismein aapko koi query nahi likhni padti—sirf Java interface ke andar **Method Naming Convention** follow karke method ka naam likhna hota hai (jaise `findByUserName(String name)`), aur Spring Boot naam padh kar khud query bana leta hai! | `List<User> findByAgeGreaterThan(int age);` |
| **2. Criteria API** *(with `MongoTemplate` / `EntityManager`)* | **For Complex Dynamic Queries:** Jab query bohot complex ho ya multiple dynamic filters lagane hon, tab hum Java code ke zariye programmatic criteria (`Criteria.where("age").gt(20)`) likhte hain. | `Query query = new Query(Criteria.where("name").is("Vipul"));` |

> **🎯 How to Articulate in an Interview (English):**
> *"JPA is strictly designed for relational databases with tabular schemas, so it is not used with NoSQL databases like MongoDB. For MongoDB, we use **Spring Data MongoDB**, which provides an identical programming model (`MongoRepository`) supporting both **Query Method DSL** (deriving queries automatically from method names) for standard operations and the **Criteria API** (`MongoTemplate`) for complex, dynamic queries."*

---

Dono videos ke saare concepts aur interview articulations cover ho gaye hain. Ab agla video (jismein `spring-boot-starter-data-mongodb` dependency daal kar `JournalApp` ko MongoDB se connect kiya gaya hai) dekho aur uska link bhejo!