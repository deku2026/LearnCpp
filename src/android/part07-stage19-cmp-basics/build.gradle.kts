plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.plugin.compose")
}

android {
    namespace = "learn.android.stage19"
    compileSdk {
        version = release(36) {
            minorApiLevel = 1
        }
    }

    defaultConfig {
        applicationId = "learn.android.stage19"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "0.1.0-placeholder"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro",
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    buildFeatures {
        compose = true
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.10.1")
    implementation("androidx.lifecycle:lifecycle-runtime-ktx:2.6.1")
    implementation("androidx.activity:activity-compose:1.8.0")
    implementation(platform("androidx.compose:compose-bom:2024.09.00"))
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.ui:ui-graphics")
    implementation("androidx.compose.ui:ui-tooling-preview")
    implementation("androidx.compose.material3:material3")
    debugImplementation("androidx.compose.ui:ui-tooling")
}

// Click-to-run from IDEA (ArcChat-style Gradle run config)
tasks.register<Exec>("runDebug") {
    group = "application"
    description = "installDebug and launch MainActivity"
    dependsOn("installDebug")
    val adbHome = System.getenv("ANDROID_HOME") ?: System.getenv("ANDROID_SDK_ROOT") ?: ""
    val adb = listOf(
        file("$adbHome/platform-tools/adb.exe"),
        file("$adbHome/platform-tools/adb"),
    ).firstOrNull { it.exists() }
        ?: throw GradleException("adb not found; set ANDROID_HOME")
    commandLine(adb.absolutePath, "shell", "am", "start", "-n", "learn.android.stage19/.MainActivity")
}
