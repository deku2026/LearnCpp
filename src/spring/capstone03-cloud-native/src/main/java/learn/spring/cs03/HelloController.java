package learn.spring.cs03;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "capstone03-cloud-native",
                "message", "Hello, Capstone 03 Cloud Native");
    }
}
