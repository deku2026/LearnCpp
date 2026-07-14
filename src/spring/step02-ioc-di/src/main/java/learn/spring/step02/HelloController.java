package learn.spring.step02;

import java.util.Map;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/")
    public Map<String, String> hello() {
        return Map.of(
                "lab", "step02-ioc-di",
                "message", "Hello, Step 02 IoC & DI");
    }
}
