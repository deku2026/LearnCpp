package learn.spring.part11;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "part11-performance-aot-native",
                "message", "Hello, Part 11 Performance AOT Native");
    }
}
