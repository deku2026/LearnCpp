# JVM scaffold full review checklist

## 0. Scope dimensions considered

- Doc practice-unit coverage (步骤/§/小节) without missing stages
- Semantic filenames/topic ids (no hash/random slugs)
- Java: public class name == file basename (case-sensitive, Linux)
- Java: package declaration matches directory under src/java
- Kotlin: package matches directory under src/kotlin/topics
- Unique topic ids; unique JVM FQNs; loader Class.forName completeness
- No generator scripts left that invent names
- Gradle sourceSets + main/runJava entrypoints
- IDEA .run configs (main class, module, default=false)
- CI steps compileKotlin/compileJava + both --list smokes (3 OS)
- Runtime: --list counts; no-args iterate; single topic dispatch
- Isolation: learn.* (Kotlin) vs learnj.* (Java) no clash
- TOPIC_INDEX counts vs disk
- UTF-8 / no tools/ gen scripts

## A. Doc coverage

[PASS] Kotlin: all stages cover practice H2 (disk 605, doc practice ~597)
[PASS] Java: all stages cover practice H2 (disk 548, doc practice ~545)

## B. Naming quality

[PASS] Kotlin: no hash-like filenames
[PASS] Java: no hash-like filenames

## C. Structure integrity (Linux-safe)

[PASS] Kotlin: package == learn.topics.<stage_dir> for all files
[PASS] Java: package == learnj.topics.<stage_dir>
[PASS] Java: public class name == filename stem (case-sensitive)
[PASS] Java: 548 unique FQNs
[PASS] Kotlin: 605 unique facade FQNs
[PASS] Kotlin: 605 unique topic ids
[PASS] Java: 548 unique topic ids
[PASS] Kotlin loader complete (605)
[PASS] Java loader complete (548)
[PASS] No tools/*.py generators present
[PASS] Entry points separated: learn.MainKt vs learnj.Main
[PASS] Gradle sourceSets: src/kotlin + src/java
[PASS] Gradle run + runJava mains set
[PASS] .run/LearnKotlin_Main.run.xml main=learn.MainKt default=false module ok
[PASS] .run/LearnJava_Main.run.xml main=learnj.Main default=false module ok
[PASS] CI linux-ci.yml: compileKotlin/compileJava/runJava --list present
[PASS] CI macos-ci.yml: compileKotlin/compileJava/runJava --list present
[PASS] CI windows-ci.yml: compileKotlin/compileJava/runJava --list present
[PASS] Kotlin TOPIC_INDEX total=605
[PASS] Java TOPIC_INDEX total=548

## Summary

- FAIL count: **0**

All automated structural checks passed. Runtime compile/smoke still required.


## D. Runtime matrix (local, CI-equivalent + more)

- [PASS] clean compileKotlin
- [PASS] compileJava after Kotlin (CI step order)
- [PASS] run --args="--list" → learn_kotlin: 605
- [PASS] runJava --args="--list" → learn_java: 548
- [PASS] single topic Kotlin `part2/stage02/s01/package_import`
- [PASS] single topic Java `part2/stage01/s01/jdk_jre_jvm`
- [PASS] no-args iterate Kotlin (605)
- [PASS] no-args iterate Java (548)

## E. How this review was planned (dimensions)

Not only "Linux class name". Full matrix:

1. Product/docs: every practice H2 has a topic; stages mapped
2. Naming: semantic sXX_item / no hash
3. Compile graph: package path, class=file, FQN unique, loader complete
4. Isolation: learn vs learnj
5. Tooling: Gradle, .run, CI yml
6. Runtime: list / single / all
7. Cross-OS: case-sensitive identifiers
