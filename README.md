# FileStream Java Library



## Introduction



The `FileStream` Java library offers a comprehensive set of classes and methods for performing diverse file operations, such as reading from and writing to files, along with managing files and directories. It leverages native C++ code via a `.dll` library to ensure efficient file handling.


**Note**: Currently, this library is only compatible with Windows operating systems due to the use of the `.dll` library.


## Features





*   **File Reading**: Read content from files with customizable settings and delimiters.


*   **File Writing**: Write content to files with different writing modes.


*   **File and Directory Management**: Check file existence, delete files and directories, rename files, get file size, create directories, list directory contents, copy files, and more.


*   **Binary and Text Modes**: Support for both binary and text file operations.


## Prerequisites





*   Java Development Kit (JDK) 17 or higher


*   Windows operating system (due to the use of `.dll` library)


*   IntelliJ IDEA (recommended for development and running)


## Installation



### Using IntelliJ IDEA&#xA;



1.  **Clone the repository**:




```
git clone https://github.com/zhienshen/FileStream.git
```



2.  **Open in IntelliJ IDEA**:


*   Launch IntelliJ IDEA.


*   Select `File` > `Open` and navigate to the cloned project directory.


*   Click `OK` to import the project.


3.  **Configure JDK**:


*   Go to `File` > `Project Structure` > `Project Settings` > `Project`.


*   Set the **Project SDK** to your installed JDK version.

## Usage



### Loading the Native Library&#xA;

Before using the library, you need to load the native `.dll` library. You can do this by calling the `load()` method in the `NativeLoader` class:




```
import shen.zhien.filestream.NativeLoader;


public class Main {


   public static void main(String\[] args) {


       try {


           NativeLoader.load();


           // Your code here


       } catch (UnsatisfiedLinkError e) {


           System.err.println("Failed to load the native library: " + e.getMessage());


       }


   }


}
```

### File Reading&#xA;

To read content from a file, you can use the `Read` class. Here is an example:




```
import shen.zhien.filestream.Read;


import shen.zhien.filestream.Settings;


public class ReadExample {


   public static void main(String\[] args) {


       String filePath = "path/to/your/file.txt";


       int setting = Settings.in;


       String content = Read.ReadFile(filePath, setting);


       if (content != null) {


           System.out.println("File content: " + content);


       } else {


           System.out.println("Failed to read the file.");


       }


   }


}
```

### File Writing&#xA;

To write content to a file, you can use the `Write` class. Here is an example:




```
import shen.zhien.filestream.Write;


import shen.zhien.filestream.Settings;


public class WriteExample {


   public static void main(String\[] args) {


       String filePath = "path/to/your/file.txt";


       String content = "Hello, World!";


       int setting = Settings.out;


       int result = Write.WriteFile(filePath, content, setting);


       if (result == 0) {


           System.out.println("File written successfully.");


       } else {


           System.out.println("Failed to write the file.");


       }


   }


}
```

### File and Directory Management&#xA;

The `FileTools` class provides various methods for managing files and directories. Here is an example of checking if a file exists:




```
import shen.zhien.filestream.FileTools;


public class FileToolsExample {


   public static void main(String\[] args) {


       String filePath = "path/to/your/file.txt";


       boolean exists = FileTools.FileExists(filePath);


       if (exists) {


           System.out.println("The file exists.");


       } else {


           System.out.println("The file does not exist.");


       }


   }


}
```

## API Documentation



### `Settings` Class&#xA;

This class provides constants for file stream settings. You can combine multiple settings using the `|` operator.




*   `in`: Read files. If the file does not exist, it will return an error.


*   `out`: Only write files. If the file does not exist, it will be created. However, if the file exists, it will be emptied before writing the content.


*   `ate`: Initial position: End of file.


*   `app`: Write to the file in append mode. If the file does not exist, it will be created. You don't need to use the setting `out` when using this setting.


*   `trunc`: Clear the file before writing or reading it. If you use this setting, you need to use the setting `out`, `app` or `in`.


*   `binary`: Write or read files in binary format. If you use this setting, you need to use the setting `out`, `app` or `in`.


### `FileTools` Class&#xA;

This class provides methods for managing files and directories.




*   `FileExists(String file)`: Checks if the specified file or directory exists.


*   `DeleteFile(String file)`: Deletes the specified file or empty directory.


*   `RenameFile(String oldName, String newName)`: Renames the specified file or directory.


*   `GetFileSize(String file)`: Returns the size of the specified file in bytes.


*   `CreateDirectory(String path)`: Creates a new directory at the specified path.


*   `IsDirectory(String path)`: Checks if the specified path is an existing directory.


*   `ListDirectory(String path)`: Lists the files and directories in the specified directory.


*   `CopyFile(String source, String destination, boolean overwrite)`: Copies an existing file to a new location.


*   `GetLastModifiedTime(String path)`: Returns the time the file was last modified.


*   `CanRead(String path)`: Tests whether the file specified by the path exists and can be read.


*   `CanWrite(String path)`: Tests whether the application can modify the file specified by the path.


*   `GetAbsolutePath(String path)`: Returns the absolute pathname string of the specified path.


*   `DeleteDirectory(String path)`: Deletes a directory and all its contents recursively.


*   `CreateTempFile(String prefix, String suffix, String directory)`: Creates a new empty file in the specified directory.


### `Read` Class&#xA;

This class provides methods for reading content from files.




*   `ReadFile(String File, int setting)`: Reads content from a file with the default delimiter (line break).


*   `ReadFile(String File, char delim, int setting)`: Reads content from a file with a custom delimiter.


*   `EOF(String File, int setting)`: Checks if the end of the file has been reached.


### `Write` Class&#xA;

This class provides methods for writing content to files.




*   `WriteFile(String File, String Content, int setting)`: Writes a string to a file.


*   `WriteFile(String File, int Content, int setting)`: Writes an integer to a file.


### `NativeLoader` Class&#xA;

This class is responsible for loading the native `.dll` library.




*   `load()`: Loads the `FileStream.dll` library.


*   `cleanupTempDir()`: Prints a message indicating that the temporary files cannot be automatically deleted.


*   `getExtractedDllPath()`: Returns the path of the extracted `.dll` file (for debugging).


## Troubleshooting





*   **UnsatisfiedLinkError**: If you encounter an `UnsatisfiedLinkError`, make sure that the `.dll` library is correctly loaded. You can check the error message for more details.


*   **File Operations Failures**: If file operations fail, check the file path, file permissions, and the settings used.


## Contributing



Contributions are welcome! If you find any issues or have suggestions for improvement, please open an issue or submit a pull request.

