pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
}

plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "1.0.0"
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "LearnKotlin"

include(":learn")

include(":android")
project(":android").projectDir = file("src/android")

file("src/android")
    .listFiles()
    ?.filter { dir ->
        dir.isDirectory &&
            dir.name != "gradle" &&
            dir.name != "build" &&
            dir.name != ".gradle" &&
            file("${dir.path}/build.gradle.kts").exists() &&
            file("${dir.path}/src/main/AndroidManifest.xml").exists()
    }
    ?.sortedBy { it.name }
    ?.forEach { dir ->
        val path = ":android:${dir.name}"
        include(path)
        project(path).projectDir = dir
    }

include(":spring")
project(":spring").projectDir = file("src/spring")

file("src/spring")
    .listFiles()
    ?.filter { dir ->
        dir.isDirectory &&
            dir.name != "gradle" &&
            dir.name != "build" &&
            dir.name != ".gradle" &&
            file("${dir.path}/build.gradle.kts").exists() &&
            file("${dir.path}/src/main/java").exists()
    }
    ?.sortedBy { it.name }
    ?.forEach { dir ->
        val path = ":spring:${dir.name}"
        include(path)
        project(path).projectDir = dir
    }
