plugins {
    // JVM labs
    kotlin("jvm") version "2.4.0" apply false
    // Android labs — match working MyApplication wizard (AGP 9.0.1)
    id("com.android.application") version "9.0.1" apply false
    id("org.jetbrains.kotlin.plugin.compose") version "2.0.21" apply false
}

tasks.register("assembleAndroidDebug") {
    group = "build"
    description = "assembleDebug for all :android:* lab modules"
    dependsOn(
        provider {
            subprojects
                .filter { it.path.startsWith(":android:") && it.path != ":android" }
                .map { "${it.path}:assembleDebug" }
        },
    )
}
