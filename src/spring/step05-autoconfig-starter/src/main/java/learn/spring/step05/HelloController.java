package learn.spring.step05;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step05-autoconfig-starter",
                "message", "Hello, Step 05 Autoconfig Starter");
    }
}
