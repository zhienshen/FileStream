package shen.zhien.filestream;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class NativeLoader {
    private static final String LIBRARY_NAME = "FileStream";
    private static final String NATIVE_FOLDER_PATH_IN_JAR = "/libs/windows/x86_64/";
    private static final String TEMP_DIR_PREFIX = "filestream_native_";
    private static final AtomicBoolean cleanupRegistered = new AtomicBoolean(false);
    private static File tempDir;
    private static File extractedDll;

    /**
     * 加载 FileStream.dll 库
     * @throws UnsatisfiedLinkError 如果加载失败
     */
    public static synchronized void load() {
        try {
            // 检查是否已加载
            if (extractedDll != null && extractedDll.exists()) {
                return;
            }

            // 尝试直接加载（用于开发环境）
            System.loadLibrary(LIBRARY_NAME);
            return;
        } catch (UnsatisfiedLinkError e) {
            // 继续从 JAR 加载
        }

        String fileName = System.mapLibraryName(LIBRARY_NAME);
        String pathInJar = NATIVE_FOLDER_PATH_IN_JAR + fileName;

        try (InputStream inputStream = NativeLoader.class.getResourceAsStream(pathInJar)) {
            if (inputStream == null) {
                throw new UnsatisfiedLinkError("无法在 JAR 中找到库: " + pathInJar);
            }

            // 创建临时目录
            if (tempDir == null) {
                tempDir = createTempDirectory();
            }

            // 创建临时文件
            extractedDll = new File(tempDir, fileName);
            extractedDll.deleteOnExit();

            // 将 JAR 中的库复制到临时文件
            Files.copy(inputStream, extractedDll.toPath(), StandardCopyOption.REPLACE_EXISTING);

            // 加载临时文件
            System.load(extractedDll.getAbsolutePath());
        } catch (IOException | UnsatisfiedLinkError ex) {
            throw new UnsatisfiedLinkError("加载 FileStream.dll 失败: " + ex.getMessage());
        }
    }

    private static File createTempDirectory() throws IOException {
        String tempDirPath = System.getProperty("java.io.tmpdir");
        File baseDir = new File(tempDirPath);

        // 创建以 TEMP_DIR_PREFIX 为前缀的目录
        File dir = Files.createTempDirectory(
                baseDir.toPath(),
                TEMP_DIR_PREFIX
        ).toFile();

        // 设置 JVM 退出时删除目录
        dir.deleteOnExit();
        return dir;
    }

    public static void cleanupTempDir() {
        System.err.println("FileStream.NativeLoader:Due to technical reasons, temporary files cannot be automatically deleted. Please manually delete the temporary files under \"" + System.getProperty("java.io.tmpdir") + "\".");
    }

    /**
     * 获取提取的 DLL 文件路径（用于调试）
     */
    public static String getExtractedDllPath() {
        return extractedDll != null ? extractedDll.getAbsolutePath() : null;
    }
}