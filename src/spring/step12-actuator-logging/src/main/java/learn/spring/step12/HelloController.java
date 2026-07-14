package learn.spring.step12;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step12-actuator-logging",
                "message", "Hello, Step 12 Actuator Logging");
    }
}
