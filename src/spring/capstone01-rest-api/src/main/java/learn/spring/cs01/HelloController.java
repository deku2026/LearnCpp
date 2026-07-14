package learn.spring.cs01;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "capstone01-rest-api",
                "message", "Hello, Capstone 01 REST API");
    }
}
