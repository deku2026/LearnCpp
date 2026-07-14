package learn

/**
 * Topic registry for LearnKotlin — same job as `learn::` in C++.
 *
 * Each placeholder `.kt` calls [registerTopic] at file top-level init time.
 * JVM only loads a class when first referenced, so [loadAllTopics] force-loads
 * every facade before main dispatches (C++ uses static-init + `learn::topic<>`).
 */
typealias TopicFn = (Array<String>) -> Int

object TopicRegistry {
    private val topics = linkedMapOf<String, TopicFn>()

    fun register(id: String, fn: TopicFn) {
        val prev = topics.put(id, fn)
        check(prev == null) { "duplicate topic id: $id" }
    }

    fun run(id: String, args: Array<String>): Int {
        val fn = topics[id] ?: run {
            System.err.println("learn_kotlin: unknown topic '$id'")
            System.err.println("  run with no args to iterate all; or --list")
            return 2
        }
        return fn(args)
    }

    fun list(): List<String> = topics.keys.toList()

    fun size(): Int = topics.size

    fun listTopics() {
        println("learn_kotlin: ${topics.size} topics registered")
        for (id in topics.keys) {
            println("  $id")
        }
    }

    /**
     * No topic id: iterate the whole map (C++ Debug `run_topic` empty-id path).
     * IDE Run/Debug with empty program args lands in every topic that has a breakpoint.
     */
    fun runAll(args: Array<String>): Int {
        var failures = 0
        println("learn_kotlin [debug]: iterating ${topics.size} topics")
        for ((id, fn) in topics) {
            val rc = fn(args)
            if (rc != 0) {
                System.err.println("  ! $id returned $rc")
                failures++
            }
        }
        if (failures > 0) {
            System.err.println("learn_kotlin [debug]: $failures topic(s) returned non-zero")
        }
        return if (failures == 0) 0 else 1
    }
}

/** Self-register helper. Returns a sentinel so it can be a top-level val. */
fun registerTopic(id: String, fn: TopicFn): Any {
    TopicRegistry.register(id, fn)
    return Unit
}
