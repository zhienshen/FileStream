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
     * @param lines The number of lines to read from the file. If set to 0, it will read all lines.
     * @param setting The reading settings. Use the settings defined in the {@link Settings} class to specify how the file should be read.Please use '|' to combine multiple settings into one setting.Such as:<code>{@link Settings#out} | {@link Settings#binary}</code>
     * @return A string containing the content read from the file, or null if the file does not exist or the file is null or when reading file, it has something wrong.
     * @see Settings
     */
    public static native String ReadFile(String File, int lines, int setting);

    /**
     * Read the all content of the file.
     * This method is a convenience method that reads the entire content of a file without any delimiters or settings.
     * If you use this method, you shouldn't use the {@link Settings} class or any delimiter.
     * @param File The file path from which the content will be read.
     * @return A string containing the content read from the file, or null if the file does not exist or the file is null or when reading file, it has something wrong.
     */
    public static native String ReadFile(String File);

    /**
     * Checks if the end of the file what you are reading has been reached.
     *
     * @return true if the end of the file has been reached, false otherwise.
     * @see Settings
     */
    public static native boolean EOF();

    /**
     * Checks if the end of the file has been reached.You can use this method to check if the file is empty.
     *
     * @param File The file path to check.
     * @return true if the end of the file has been reached, false otherwise.
     */
    public static native boolean EOF(String File);

    /**
     *
     */
    public static native void ResetFile();

}
