package learn.spring.cs02;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "capstone02-modulith",
                "message", "Hello, Capstone 02 Modulith");
    }
}
