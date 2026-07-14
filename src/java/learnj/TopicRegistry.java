package learnj;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Topic registry for LearnJava — same job as Kotlin {@code learn.TopicRegistry}
 * and C++ {@code learn::topic}.
 *
 * <p>Package {@code learnj} avoids clashing with Kotlin {@code learn.*} on the same
 * classpath (both live under {@code src/java} + {@code src/kotlin}).
 *
 * <p>Each placeholder class registers in a {@code static} block. JVM only loads a
 * class when first referenced, so {@link GeneratedTopicLoader#loadAll()}
 * force-loads every facade before main dispatches.
 */
public final class TopicRegistry {
    @FunctionalInterface
    public interface TopicFn {
        int run(String[] args);
    }

    private static final Map<String, TopicFn> TOPICS = new LinkedHashMap<>();

    private TopicRegistry() {}

    public static void register(String id, TopicFn fn) {
        TopicFn prev = TOPICS.put(id, fn);
        if (prev != null) {
            throw new IllegalStateException("duplicate topic id: " + id);
        }
    }

    public static int run(String id, String[] args) {
        TopicFn fn = TOPICS.get(id);
        if (fn == null) {
            System.err.println("learn_java: unknown topic '" + id + "'");
            System.err.println("  run with no args to iterate all; or --list");
            return 2;
        }
        return fn.run(args);
    }

    public static List<String> list() {
        return new ArrayList<>(TOPICS.keySet());
    }

    public static int size() {
        return TOPICS.size();
    }

    public static void listTopics() {
        System.out.println("learn_java: " + TOPICS.size() + " topics registered");
        for (String id : TOPICS.keySet()) {
            System.out.println("  " + id);
        }
    }

    /**
     * No topic id: iterate the whole map (C++ Debug / Kotlin empty-args path).
     * IDE Run/Debug with empty program args lands in every topic that has a breakpoint.
     */
    public static int runAll(String[] args) {
        int failures = 0;
        System.out.println("learn_java [debug]: iterating " + TOPICS.size() + " topics");
        for (Map.Entry<String, TopicFn> e : TOPICS.entrySet()) {
            int rc = e.getValue().run(args);
            if (rc != 0) {
                System.err.println("  ! " + e.getKey() + " returned " + rc);
                failures++;
            }
        }
        if (failures > 0) {
            System.err.println("learn_java [debug]: " + failures + " topic(s) returned non-zero");
        }
        return failures == 0 ? 0 : 1;
    }
}
