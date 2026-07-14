package learn

/**
 * Single entry — same role as [src/cpp/main.cpp].
 *
 * Empty argv (IDE Run / Debug with no program args):
 * walk every registered topic so a breakpoint in any placeholder hits
 * (C++ Debug `#ifndef NDEBUG` path / F5 workflow).
 *
 * Optional CLI: `topic_id`, `--list`, `--help`. Not required for daily IDE use.
 */
fun main(args: Array<String>) {
    // Force-load topic facades so top-level registerTopic() runs (C++ static-init).
    loadAllTopics()

    if (args.isEmpty()) {
        // C++ Debug main: no args → registry iterates every topic.
        kotlin.system.exitProcess(TopicRegistry.runAll(emptyArray()))
    }

    val first = args[0]
    if (first == "--help" || first == "-h" || first == "help") {
        println(
            """
            |usage: learn_kotlin [topic_id [extra_args...]]
            |  (no args)   iterate every topic — IDE Run/Debug with empty args
            |  topic_id    run that topic only
            |  --list      list registered topic ids
            |  --help/-h   this message
            """.trimMargin()
        )
        return
    }
    if (first == "--list" || first == "list") {
        TopicRegistry.listTopics()
        return
    }

    kotlin.system.exitProcess(TopicRegistry.run(first, args.drop(1).toTypedArray()))
}
