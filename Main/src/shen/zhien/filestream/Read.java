package shen.zhien.filestream;

/**
 * This class provides methods for reading content from files.
 */
public final class Read {

    private Read() {}

    /**
     * Reads content from a file.The default delimiter is a line break character.
     *
     * @param File    The file path from which the content will be read.
     * @param setting The reading settings. Use the settings defined in the {@link Settings} class to specify how the file should be read.Please use '|' to combine multiple settings into one setting.Such as:<code>{@link Settings#out} | {@link Settings#binary}</code>
     * @return A string containing the content read from the file, or null if the file does not exist or the file is null or when reading file, it has something wrong.
     * @see Settings
     * @see #ReadFile(String, char, int)
     */
    public static native String ReadFile(String File, int setting);

        /**
     * Reads content from a file.Use custom delimiter.
     *
     * @param File    The file path from which the content will be read.
     * @param delim   The delimiter used to separate content in the file. This can be any character that is used to split the content into parts.
     * @param setting The reading settings. Use the settings defined in the {@link Settings} class to specify how the file should be read.Please use '|' to combine multiple settings into one setting.Such as:<code>{@link Settings#out} | {@link Settings#binary}</code>
     * @return A string containing the content read from the file, or null if an error occurs.
     * @see Settings
     * @see #ReadFile(String, int)
     */
    public static native String ReadFile(String File, char delim, int setting);

    public static native boolean EOF(String File, int setting);

}
