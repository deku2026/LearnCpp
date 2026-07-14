pluginManagement {
    repositories {
        mavenCentral()
        gradlePluginPortal()
    }
    val kotlinVersion = providers.gradleProperty("kotlin.version").getOrElse("2.4.0")
    plugins {
        kotlin("jvm").version(kotlinVersion)
    }
}

plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "1.0.0"
}

rootProject.name = "LearnKotlin"
