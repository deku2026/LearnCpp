package learn.spring.part08;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "part08-distributed-cloud",
                "message", "Hello, Part 08 Distributed Cloud");
    }
}
