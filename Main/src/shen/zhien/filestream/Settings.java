package shen.zhien.filestream;

import java.io.IOException;

/**
 * This class provides constants and methods for file stream settings.Use '|' to combine multiple settings into one setting.Such as:<code>{@link #out} | {@link #binary}</code>
 */
public final class Settings {
    private Settings() {}

    /**
     * Read files. If the file does not exist, it will return an error.
     */
    public static final int in = 1;

    /**
     * Only write files. If the file does not exist, it will be created. However, if the file exists, it will be emptied before writing the content.
     */
    public static final int out = 2;

    /**
     * Initial position: End of file.
     */
    public static final int ate = 4;

    /**
     * Write to the file in append mode. If the file does not exist, it will be created.You don't need to use the setting <code>out</code> when using this setting.
     */
    public static final int app = 8;

    /**
     * Clear the file before writing or reading it.If you use this setting, you need to use the setting <code>out</code> , <code>app</code> or <code>in</code>.
     * @see #in
     * @see #out
     * @see #app
     */
    public static final int trunc = 16;

    /**
     * Write or read files in binary format.If you use this setting, you need to use the setting <code>out</code> , <code>app</code> or <code>in</code>.
     * @see #in
     * @see #out
     * @see #app
     */
    public static final int binary = 32;
}
