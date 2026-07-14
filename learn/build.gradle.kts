import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    kotlin("jvm")
    application
}

sourceSets {
    main {
        kotlin {
            setSrcDirs(listOf("../src/kotlin"))
            exclude("**/README.md")
            exclude("**/TOPIC_INDEX.md")
        }
        java {
            setSrcDirs(listOf("../src/java"))
            exclude("**/README.md")
            exclude("**/TOPIC_INDEX.md")
        }
        resources.setSrcDirs(emptyList<String>())
    }
    test {
        kotlin.setSrcDirs(emptyList<String>())
        java.setSrcDirs(emptyList<String>())
    }
}

kotlin {
    jvmToolchain(25)
    compilerOptions {
        jvmTarget.set(JvmTarget.fromTarget("25"))
    }
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(25))
    }
}

application {
    mainClass.set("learn.MainKt")
}

// FFM (JEP 472) restricted methods need an explicit grant on the runtime JVM.
tasks.withType<JavaExec>().configureEach {
    jvmArgs("--enable-native-access=ALL-UNNAMED")
}

tasks.register<JavaExec>("runJava") {
    group = "application"
    description = "Run LearnJava main (learnj.Main)"
    classpath = sourceSets["main"].runtimeClasspath
    mainClass.set("learnj.Main")
}

dependencies {
    testImplementation(kotlin("test"))
}

tasks.test {
    useJUnitPlatform()
}
