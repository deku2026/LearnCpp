package learn.spring.step07;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step07-validation-problem-details",
                "message", "Hello, Step 07 Validation ProblemDetails");
    }
}
