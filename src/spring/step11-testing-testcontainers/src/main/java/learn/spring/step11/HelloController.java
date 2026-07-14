package learn.spring.step11;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step11-testing-testcontainers",
                "message", "Hello, Step 11 Testing Testcontainers");
    }
}
