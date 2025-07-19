package shen.zhien.filestream;

/**
 * This class provides various file operations such as checking existence, deleting, renaming,
 * getting size, creating directories, listing directory contents, copying files, and more.
 * It uses native methods to perform these operations.
 */
public class FileTools {
    private FileTools() { }

    /**
     * Checks if the specified file or directory exists.
     *
     * @param file the path to the file or directory to check
     * @return true if the file or directory exists, false otherwise
     */
    public static native boolean FileExists(String file);

    /**
     * Deletes the specified file or empty directory.
     *
     * @param file the path to the file or directory to delete
     * @return 0 if the operation succeeds, a negative error code otherwise
     */
    public static native int DeleteFile(String file);

    /**
     * Renames the specified file or directory.
     *
     * @param oldName the current path of the file or directory
     * @param newName the new path after renaming
     * @return 0 if the operation succeeds, a negative error code otherwise
     */
    public static native int RenameFile(String oldName, String newName);

    /**
     * Returns the size of the specified file in bytes.
     *
     * @param file the path to the file
     * @return the size of the file in bytes, or -1 if the file does not exist
     *         or an error occurs
     */
    public static native long GetFileSize(String file);

    /**
     * Creates a new directory at the specified path, including any necessary but nonexistent parent directories.
     *
     * @param path the path of the directory to create
     * @return true if the directory was created successfully, false otherwise
     */
    public static native boolean CreateDirectory(String path);

    /**
     * Checks if the specified path is an existing directory.
     *
     * @param path the path to check
     * @return true if the path exists and is a directory, false otherwise
     */
    public static native boolean IsDirectory(String path);

    /**
     * Lists the files and directories in the specified directory.
     *
     * @param path the path of the directory to list
     * @return an array of strings naming the files and directories in the directory,
     *         or an empty array if the directory is empty or the operation fails
     */
    public static native String[] ListDirectory(String path);

    /**
     * Copies an existing file to a new location.
     *
     * @param source the path to the file to copy
     * @param destination the path to the target file
     * @param overwrite whether to overwrite an existing file at the destination
     * @return true if the file was copied successfully, false otherwise
     */
    public static native boolean CopyFile(String source, String destination, boolean overwrite);

    /**
     * Returns the time the file was last modified.
     *
     * @param path the path to the file
     * @return the time the file was last modified, measured in milliseconds
     *         since the epoch (00:00:00 GMT, January 1, 1970),
     *         or -1 if the operation fails
     */
    public static native long GetLastModifiedTime(String path);

    /**
     * Tests whether the file specified by the path exists and can be read.
     *
     * @param path the path to the file
     * @return true if and only if the file specified by the path exists and can be read; false otherwise
     */
    public static native boolean CanRead(String path);

    /**
     * Tests whether the application can modify the file specified by the path.
     *
     * @param path the path to the file
     * @return true if and only if the file system actually contains a file denoted by this
     *         path name and the application is allowed to write to the file; false otherwise
     */
    public static native boolean CanWrite(String path);

    /**
     * Returns the absolute pathname string of the specified path.
     *
     * @param path the path to be resolved
     * @return the absolute pathname string, or null if the operation fails
     */
    public static native String GetAbsolutePath(String path);

    /**
     * Deletes a directory and all its contents recursively.
     *
     * @param path the path to the directory to delete
     * @return the number of files and directories deleted, or -1 if the operation fails
     */
    public static native long DeleteDirectory(String path);

    /**
     * Creates a new empty file in the specified directory, using the given prefix and suffix strings to generate its name.
     *
     * @param prefix the prefix string to be used in generating the file's name; must be at least three characters long
     * @param suffix the suffix string to be used in generating the file's name; may be null, in which case the suffix ".tmp" will be used
     * @param directory the directory in which the file is to be created, or null if the default temporary-file directory is to be used
     * @return the absolute pathname string of the new temporary file, or null if the operation fails
     */
    public static native String CreateTempFile(String prefix, String suffix, String directory);
}
