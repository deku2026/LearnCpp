package learnj.topics.p8_native;

import java.lang.foreign.Arena;
import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.Linker;
import java.lang.foreign.SymbolLookup;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandle;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import learnj.TopicRegistry;

/**
 * LearnJava practice: FFM overview (JEP 454 · final in Java 22+).
 * <p>
 * Doc      : 第8部分-本地互操作-JNI-FFM-Vector-API.md
 * Stage    : p8_native
 * Step     : s04
 * Item     : ffm_overview_jep454
 * Topic id : part8/native/s04/ffm_overview_jep454
 * Heading  : FFM 总览（JEP 454 · Java 22 定稿）
 * <p>
 * Loads CMake {@code learn_ffm}, downcalls {@code add*}, prints {@code pid=}, then
 * calls {@code learn_ffm_mul_add_i32} every 200ms so a C++ debugger can attach.
 * Needs {@code --enable-native-access=ALL-UNNAMED} (LearnJava Main / runJava).
 */
public final class s04_ffm_overview_jep454 {
    static {
        TopicRegistry.register("part8/native/s04/ffm_overview_jep454", args -> {
            try {
                return runDemo();
            } catch (Throwable t) {
                t.printStackTrace(System.err);
                return 1;
            }
        });
    }

    private s04_ffm_overview_jep454() {
    }

    private static int runDemo() throws Throwable {
        Path lib = resolveLearnFfmLibrary();
        long pid = ProcessHandle.current().pid();
        System.out.println("FFM (JEP 454) — library: " + lib.toAbsolutePath());
        System.out.println("pid=" + pid);

        Linker linker = Linker.nativeLinker();
        try (Arena arena = Arena.ofConfined()) {
            SymbolLookup lookup = SymbolLookup.libraryLookup(lib, arena);

            MethodHandle abiVersion = downcall(
                    linker, lookup, "learn_ffm_abi_version", ValueLayout.JAVA_INT);
            MethodHandle addI32 = downcall(
                    linker,
                    lookup,
                    "learn_ffm_add_i32",
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT);
            MethodHandle addI64 = downcall(
                    linker,
                    lookup,
                    "learn_ffm_add_i64",
                    ValueLayout.JAVA_LONG,
                    ValueLayout.JAVA_LONG,
                    ValueLayout.JAVA_LONG);
            MethodHandle addF64 = downcall(
                    linker,
                    lookup,
                    "learn_ffm_add_f64",
                    ValueLayout.JAVA_DOUBLE,
                    ValueLayout.JAVA_DOUBLE,
                    ValueLayout.JAVA_DOUBLE);
            MethodHandle add3 = downcall(
                    linker,
                    lookup,
                    "learn_ffm_add3_i32",
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT);
            MethodHandle mulAdd = downcall(
                    linker,
                    lookup,
                    "learn_ffm_mul_add_i32",
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT,
                    ValueLayout.JAVA_INT);

            int version = (int) abiVersion.invokeExact();
            int sum32 = (int) addI32.invokeExact(40, 2);
            long sum64 = (long) addI64.invokeExact(1_000_000_000_000L, 24L);
            double sumF = (double) addF64.invokeExact(1.5, 2.25);
            int sum3 = (int) add3.invokeExact(1, 2, 3);
            int mad = (int) mulAdd.invokeExact(6, 7, 8);

            System.out.printf(Locale.ROOT, "learn_ffm_abi_version()      = %d%n", version);
            System.out.printf(Locale.ROOT, "learn_ffm_add_i32(40, 2)     = %d%n", sum32);
            System.out.printf(Locale.ROOT, "learn_ffm_add_i64(...)       = %d%n", sum64);
            System.out.printf(Locale.ROOT, "learn_ffm_add_f64(1.5, 2.25) = %s%n", sumF);
            System.out.printf(Locale.ROOT, "learn_ffm_add3_i32(1, 2, 3)  = %d%n", sum3);
            System.out.printf(Locale.ROOT, "learn_ffm_mul_add_i32(6,7,8) = %d%n", mad);

            if (version != 1
                    || sum32 != 42
                    || sum64 != 1_000_000_000_024L
                    || sum3 != 6
                    || mad != 50
                    || Double.compare(sumF, 3.75) != 0) {
                throw new IllegalStateException("unexpected native results");
            }

            System.out.println("OK — looping mul_add every 200ms; attach C++ debugger to pid above.");
            for (int i = 0; ; i++) {
                int r = (int) mulAdd.invoke(i, 1, 0);
                System.out.printf(Locale.ROOT, "mul_add(%d,1,0)=%d%n", i, r);
                Thread.sleep(200);
            }
        }
    }

    private static MethodHandle downcall(
            Linker linker, SymbolLookup lookup, String name, ValueLayout ret, ValueLayout... args) {
        return linker.downcallHandle(
                lookup.findOrThrow(name), FunctionDescriptor.of(ret, args));
    }

    /** Resolve {@code learn_ffm} under {@code build/&lt;preset&gt;/bin} (or learn.ffm.* / LEARN_FFM_DIR). */
    static Path resolveLearnFfmLibrary() {
        String libProp = System.getProperty("learn.ffm.lib");
        if (libProp != null && !libProp.isBlank()) {
            Path p = Path.of(libProp).toAbsolutePath().normalize();
            if (!Files.isRegularFile(p)) {
                throw new IllegalStateException("learn.ffm.lib not a file: " + p);
            }
            return p;
        }

        String mapped = System.mapLibraryName("learn_ffm");
        List<Path> candidates = new ArrayList<>();

        String dirProp = System.getProperty("learn.ffm.dir");
        if (dirProp != null && !dirProp.isBlank()) {
            candidates.add(Path.of(dirProp).resolve(mapped));
        }
        String env = System.getenv("LEARN_FFM_DIR");
        if (env != null && !env.isBlank()) {
            candidates.add(Path.of(env).resolve(mapped));
        }

        Path userDir = Path.of(System.getProperty("user.dir", ".")).toAbsolutePath().normalize();
        for (Path root : List.of(userDir, userDir.getParent() == null ? userDir : userDir.getParent())) {
            Path build = root.resolve("build");
            if (Files.isDirectory(build)) {
                try (DirectoryStream<Path> presets = Files.newDirectoryStream(build)) {
                    for (Path preset : presets) {
                        candidates.add(preset.resolve("bin").resolve(mapped));
                    }
                } catch (Exception ignored) {
                    // fall through
                }
            }
        }

        for (Path c : candidates) {
            if (c != null && Files.isRegularFile(c)) {
                return c.toAbsolutePath().normalize();
            }
        }

        throw new IllegalStateException(
                "Cannot find "
                        + mapped
                        + ". Build learn_ffm (scripts/build-ffm-native.cmd). Tried: "
                        + candidates.stream().filter(Objects::nonNull).map(Path::toString).toList());
    }
}
