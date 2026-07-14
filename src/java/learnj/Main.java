package learnj;

/**
 * Single entry — same role as {@code src/cpp/main.cpp} and Kotlin {@code learn.MainKt}.
 *
 * <p>Package {@code learnj} avoids clashing with Kotlin {@code learn.*}.
 *
 * <p>Empty argv (IDE Run / Debug with no program args): walk every registered topic
 * so a breakpoint in any placeholder hits.
 *
 * <p>Optional CLI: {@code topic_id}, {@code --list}, {@code --help}.
 */
public final class Main {
    private Main() {}

    public static void main(String[] args) {
        GeneratedTopicLoader.loadAll();

        if (args.length == 0) {
            System.exit(TopicRegistry.runAll(args));
        }

        String first = args[0];
        if ("--help".equals(first) || "-h".equals(first) || "help".equals(first)) {
            System.out.println(
                    """
                    usage: learn_java [topic_id [extra_args...]]
                      (no args)   iterate every topic — IDE Run/Debug with empty args
                      topic_id    run that topic only
                      --list      list registered topic ids
                      --help/-h   this message
                    """.stripIndent());
            return;
        }
        if ("--list".equals(first) || "list".equals(first)) {
            TopicRegistry.listTopics();
            return;
        }

        String[] rest = new String[args.length - 1];
        System.arraycopy(args, 1, rest, 0, rest.length);
        System.exit(TopicRegistry.run(first, rest));
    }
}
