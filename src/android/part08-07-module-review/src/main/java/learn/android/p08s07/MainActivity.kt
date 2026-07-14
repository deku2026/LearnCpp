package learn.android.p08s07

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp

/**
 * Placeholder HelloWorld for future practice of:
 * 第8部分-07-模块Review方法与清单.md
 *
 * Independent app scaffold (AGP 9 built-in Kotlin + Compose).
 * Toolchain: JDK 25, bytecode target JVM 21 (hot reload friendly).
 */
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            MaterialTheme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    PlaceholderScreen(title = "Part 08.07 Module Review")
                }
            }
        }
    }
}

@Composable
fun PlaceholderScreen(title: String, modifier: Modifier = Modifier) {
    Box(
        modifier = modifier
            .fillMaxSize()
            .padding(24.dp),
        contentAlignment = Alignment.Center,
    ) {
        Text(
            text = "Hello, Part 08.07 Module Review",
            style = MaterialTheme.typography.headlineSmall,
        )
    }
}

@Preview(showBackground = true)
@Composable
private fun PlaceholderScreenPreview() {
    MaterialTheme {
        PlaceholderScreen(title = "Part 08.07 Module Review")
    }
}
