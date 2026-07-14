# LearnJava sources (`src/java`)

Source dir is set in root `build.gradle.kts` (`sourceSets.main.java.srcDirs`).
Parallel to `src/cpp/` and `src/kotlin/`. Open **repo root** in IDEA.

- Run config: `.run/LearnJava_Main.run.xml` (direct main) or `.run/LearnJava.run.xml` (Gradle `runJava`)
- Entry: `learnj.Main` (package `learnj` — isolated from Kotlin `learn.*`)
- Registry: `learnj.TopicRegistry` + `learnj.GeneratedTopicLoader`
- Topics: `topics/**` — one class per JavaStudy practice step (`learnj.topics`)

```bat
gradlew.bat runJava
gradlew.bat runJava --args="--list"
gradlew.bat runJava --args="part2/stage01/s01/jdk_jre_jvm"
```
