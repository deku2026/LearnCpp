package learn.spring.step03;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step03-bean-lifecycle-scope",
                "message", "Hello, Step 03 Bean Lifecycle Scope");
    }
}
