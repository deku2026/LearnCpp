package learn.spring.step08;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step08-aop-tx-events",
                "message", "Hello, Step 08 AOP TX Events");
    }
}
