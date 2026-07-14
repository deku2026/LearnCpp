package learn.spring.part07;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "part07-messaging-async",
                "message", "Hello, Part 07 Messaging Async");
    }
}
