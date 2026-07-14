package learn.spring.part10;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "part10-deploy-delivery",
                "message", "Hello, Part 10 Deploy Delivery");
    }
}
