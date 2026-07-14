import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    kotlin("jvm")
    application
}

repositories {
    mavenCentral()
}

// Custom source root: src/kotlin (parallel to src/cpp). Not the default src/main/kotlin.
sourceSets {
    main {
        kotlin {
            setSrcDirs(listOf("src/kotlin"))
            exclude("**/README.md")
            exclude("**/TOPIC_INDEX.md")
        }
        resources.setSrcDirs(emptyList<String>())
    }
    test {
        kotlin {
            setSrcDirs(emptyList<String>())
        }
    }
}

kotlin {
    jvmToolchain(25)
    compilerOptions {
        jvmTarget.set(JvmTarget.fromTarget("25"))
    }
}

application {
    mainClass.set("learn.MainKt")
}

dependencies {
    testImplementation(kotlin("test"))
}

tasks.test {
    useJUnitPlatform()
}
