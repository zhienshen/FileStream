package shen.zhien.filestream;

/**
 * This class provides methods for writing content to files.
 */
public final class Write {

    private Write() {}

    /**
     * Writes content to a file.
     *
     * @param File    The file path where the content will be written.
     * @param Content The content to write to the file.
     * @param setting The writing settings.Use the settings defined in the {@link Settings} class to specify how the file should be written.Please use '|' to combine multiple settings into one setting.Such as:<code>{@link Settings#out} | {@link Settings#binary}</code>
     * @return An integer indicating the result of the write operation.
     * <p>
     *     0 on success, or a negative value on failure.
     * <p>
     * @see shen.zhien.filestream.Settings
     * @see Settings
     * @see #WriteFile(String, String, int)
     */
    public static native int WriteFile(String File, String Content, int setting);

    /**
     * Writes content to a file.
     *
     * @param File    The file path where the content will be written.
     * @param Content The content to write to the file.
     * @param setting The writing settings.Use the settings defined in the {@link Settings} class to specify how the file should be written.Please use '|' to combine multiple settings into one setting.Such as:<code>{@link Settings#out} | {@link Settings#binary}</code>
     * @return An integer indicating the result of the write operation.
     * <p>
     *     0 on success, or a negative value on failure.
     * <p>
     * @see shen.zhien.filestream.Settings
     * @see Settings
     * @see #WriteFile(String, String, int)
     */
    public static native int WriteFile(String File, int Content, int setting);

}
