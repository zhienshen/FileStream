// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "shen.zhien.filestream.Read.h"
#include "shen.zhien.filestream.Write.h"
#include "shen_zhien_filestream_FileTools.h"
using namespace std;

enum class Type {
    INT,
    STRING,
    BINARY_INT
};

JNIEXPORT jint JNICALL Java_shen_zhien_filestream_Write_WriteFile__Ljava_lang_String_2Ljava_lang_String_2I(JNIEnv* env, jclass cla, jstring file, jstring content, jint setting)
{
    const char* fileChars = nullptr;
    const char* contentChars = nullptr;

    try {
        // 获取Java字符串
        fileChars = env->GetStringUTFChars(file, nullptr);
        if (!fileChars) throw std::runtime_error("Failed to get file name");

        contentChars = env->GetStringUTFChars(content, nullptr);
        if (!contentChars) throw std::runtime_error("Failed to get content");

        // 转换为C++字符串
        const std::string s_file(fileChars);
        const std::string s_content(contentChars);

        // 设置文件打开模式
        std::ios::openmode mode = static_cast<std::ios::openmode>(setting | std::ios::binary);

        // 打开文件
        std::ofstream writer(s_file, mode);
        // 写入类型标识符
        Type w_t = Type::STRING;
        writer.write(reinterpret_cast<const char*>(&w_t), sizeof(Type));
        if (!writer) throw std::runtime_error("Failed to write type");

        // 写入大小
        size_t size = s_content.size();
        writer.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        if (!writer) throw std::runtime_error("Failed to write size");

        // 写入内容
        writer.write(s_content.c_str(), s_content.size());
        if (!writer) throw std::runtime_error("Failed to write content");

        // 确保释放JNI资源
        env->ReleaseStringUTFChars(file, fileChars);
        env->ReleaseStringUTFChars(content, contentChars);
        return 0; // 成功
    }
    catch (const std::exception& e) {
        // 输出错误信息到标准错误
        std::cerr << "JNI Error: " << e.what() << std::endl;

        // 释放JNI资源
        if (fileChars) env->ReleaseStringUTFChars(file, fileChars);
        if (contentChars) env->ReleaseStringUTFChars(content, contentChars);

        // 返回错误码
        return -1; // 一般错误
    }
}

JNIEXPORT jint JNICALL Java_shen_zhien_filestream_Write_WriteFile__Ljava_lang_String_2II(JNIEnv* env, jclass cla, jstring file, jint content, jint setting) {
    const char* fileChars = env->GetStringUTFChars(file, nullptr);
    if (!fileChars) {
        env->ThrowNew(env->FindClass("java/io/IOException"), "Failed to get file name");
        return -1;
    }

    // 使用RAII管理资源
    struct JNIStringReleaser {
        JNIEnv* env;
        const jstring str;
        const char* chars;
        ~JNIStringReleaser() { env->ReleaseStringUTFChars(str, chars); }
    } releaser{ env, file, fileChars };

    try {
        const std::string s_file(fileChars);
        std::ios::openmode mode = static_cast<std::ios::openmode>(setting);

        std::ofstream writer(s_file, mode);

        const bool isBinaryMode = (setting & std::ios::binary) != 0;
        if (isBinaryMode) {
            //先写入类型标识符（binary int）
            auto w_t = Type::BINARY_INT;
            writer.write(reinterpret_cast<const char*>(&w_t), sizeof(Type));

            writer.write(reinterpret_cast<const char*>(&content), sizeof(int));
        }
        else {
            //写入类型标识符（int）
            auto w_t = Type::INT;
            writer.write(reinterpret_cast<const char*>(&w_t), sizeof(Type));

            int c_content = content;

            string s_content(to_string(c_content));

            size_t size = s_content.size();

            writer.write((const char*)&size, sizeof(size_t));

            writer.write(s_content.c_str(), size);
        }

        if (!writer.good()) {
            char errorMsg[256];
            if (strerror_s(errorMsg, sizeof(errorMsg), errno) == 0) {
                throw std::runtime_error("Failed to write to file: " + s_file + ", error: " + std::string(errorMsg));
            }
            else {
                throw std::runtime_error("Failed to write to file: " + s_file + ", unknown error");
            }
        }

        return 0; // 成功
    }
    catch (const std::exception& e) {
        std::cerr << "JNI Error: " << e.what() << std::endl;

        // 抛出Java异常
        jclass exceptionClass = env->FindClass("java/io/IOException");
        if (exceptionClass) {
            env->ThrowNew(exceptionClass, e.what());
        }

        // 根据错误类型返回不同错误码
        if (std::string_view(e.what()).find("open") != std::string_view::npos) {
            return -2; // 文件打开失败
        }
        else if (std::string_view(e.what()).find("write") != std::string_view::npos) {
            return -3; // 文件写入失败
        }
        return -1; // 一般错误
    }
    catch (...) {
        std::cerr << "JNI Error: Unknown exception" << std::endl;
        jclass exceptionClass = env->FindClass("java/lang/RuntimeException");
        if (exceptionClass) {
            env->ThrowNew(exceptionClass, "Unknown exception occurred");
        }
        return -1; // 未知错误
    }
}

JNIEXPORT jstring JNICALL Java_shen_zhien_filestream_Read_ReadFile__Ljava_lang_String_2II(JNIEnv* env, jclass cla, jstring file, jint lines, jint setting)
{
    const char* s_file = env->GetStringUTFChars(file, nullptr);
    std::string result;

    try {
        std::ifstream reader(s_file, setting | ios::binary);
        if (!reader.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        // 使用RAII确保资源释放
        struct ResourceReleaser {
            JNIEnv* env;
            jstring file;  // 保存 jstring
            const char* s_file;
            ~ResourceReleaser() {
                if (env && file && s_file) {
                    env->ReleaseStringUTFChars(file, s_file);
                }
            }
        } releaser{ env, file, s_file };


        Type type;
        for (int i = 0; i < lines;i++)
        {
            reader.read(reinterpret_cast<char*>(&type), sizeof(Type));
            if (reader && reader.peek() != EOF)
            {
                switch (type)
                {
                case Type::BINARY_INT:
                {
                    int num;
                    reader.read(reinterpret_cast<char*>(&num), sizeof(int));
                    if (reader) {
                        result += std::to_string(num);
                    }
                    break;
                }
                case Type::INT:
                {
                    size_t size;
                    reader.read(reinterpret_cast<char*>(&size), sizeof(size_t));

                    char* buffer = new char[size];

                    reader.read(buffer, size);

                    result += string(buffer, size);

                    delete[] buffer;
                    break;
                }
                case Type::STRING:
                {
                    size_t size;
                    reader.read(reinterpret_cast<char*>(&size), sizeof(size_t));
                    if (reader.good() && size > 0) {
                        std::vector<char> buffer(size);
                        reader.read(buffer.data(), size);
                        if (reader.gcount() == static_cast<std::streamsize>(size)) {
                            result.append(buffer.data(), size);
                        }
                    }
                    break;
                }
                default:
                    // 未知类型，跳过
                    reader.setstate(std::ios::failbit);
                    break;
                }
            }
        }

        // 检查流状态
        if (reader.bad())
        {
            throw std::runtime_error("File read error");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception in ReadFile: " << e.what() << std::endl;
        result = "";
    }

    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL Java_shen_zhien_filestream_Read_ReadFile__Ljava_lang_String_2(JNIEnv* env, jclass cla, jstring file)
{
    const char* c_file = nullptr;
    std::string result;

    try
    {
        c_file = env->GetStringUTFChars(file, nullptr);
        if (c_file == nullptr)
        {
            throw std::runtime_error("Failed to get file name");
        }

        std::string s_file(c_file);
        env->ReleaseStringUTFChars(file, c_file); // 提前释放，避免内存泄漏

        std::ifstream reader(s_file, std::ios::in | std::ios::binary);
        if (!reader.is_open())
        {
            throw std::runtime_error("Failed to open file: " + s_file);
        }

        // 读取所有数据块
        Type t;
        while (reader.peek() != EOF && reader.read(reinterpret_cast<char*>(&t), sizeof(Type)))
        {

            switch (t)
            {
            case Type::BINARY_INT:
            {
                int32_t num;
                reader.read(reinterpret_cast<char*>(&num), sizeof(num));

                if (reader.gcount() != sizeof(num)) {
                    jclass exceptionClass = env->FindClass("java/io/EOFException");
                    env->ThrowNew(exceptionClass, "Unexpected end of file while reading integer");
                    break;
                }
                result.append(std::to_string(num));
                break;
            }
            case Type::INT:
            {
                size_t size;
                reader.read(reinterpret_cast<char*>(&size), sizeof(size_t));

                char* buffer = new char[size];

                reader.read(buffer, size);

                result += string(buffer, size);

                delete[] buffer;
                break;
            }
            case Type::STRING:
            {
                size_t size;
                reader.read(reinterpret_cast<char*>(&size), sizeof(size_t));
                if (reader.good() && size > 0) {
                    std::vector<char> buffer(size);
                    reader.read(buffer.data(), size);
                    if (reader.gcount() == static_cast<std::streamsize>(size)) {
                        result.append(buffer.data(), size);
                    }
                }

                break;
            }
            default:
                throw std::runtime_error("未知的Type值: " + static_cast<int>(t));
            }
        }

        // 检查是否因文件末尾正常结束
        if (!reader.eof()) {
            throw std::runtime_error("读取过程中发生错误");
        }

        return env->NewStringUTF(result.c_str());
    }
    catch (const std::exception& e) {
        std::cerr << "JNI Error: " << e.what() << std::endl;

        // 抛出Java异常
        jclass exceptionClass = env->FindClass("java/io/IOException");
        if (exceptionClass) {
            env->ThrowNew(exceptionClass, e.what());
        }

        return env->NewStringUTF(""); // 返回空字符串
    }
    catch (...) {
        std::cerr << "JNI Error: Unknown exception" << std::endl;

        jclass exceptionClass = env->FindClass("java/lang/RuntimeException");
        if (exceptionClass) {
            env->ThrowNew(exceptionClass, "Unknown exception occurred");
        }

        return env->NewStringUTF(""); // 返回空字符串
    }
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_Read_EOF(JNIEnv* env, jclass cla, jstring file, jint setting)
{
    const char* fileChars = nullptr;

    try {
        fileChars = env->GetStringUTFChars(file, nullptr);
        if (!fileChars) throw std::runtime_error("Failed to get file name");

        const std::string s_file(fileChars);

        std::ifstream EOFReader(s_file, setting);

        char Char;
        EOFReader >> Char;

        return EOFReader.eof();
    }
    catch (const std::exception& e) {
        std::cerr << "JNI Error: " << e.what() << std::endl;
        if (fileChars) env->ReleaseStringUTFChars(file, fileChars);
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_FileExists(JNIEnv* env, jclass cla, jstring file) {
    const char* fileChars = env->GetStringUTFChars(file, nullptr);
    if (!fileChars) {
        return JNI_FALSE;
    }
    std::filesystem::path filePath(fileChars);
    bool exists = std::filesystem::exists(filePath);
    env->ReleaseStringUTFChars(file, fileChars);
    return exists ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_shen_zhien_filestream_FileTools_DeleteFile(JNIEnv* env, jclass cla, jstring file) {
    const char* fileChars = env->GetStringUTFChars(file, nullptr);
    if (!fileChars) {
        return -1;
    }
    std::filesystem::path filePath(fileChars);
    try {
        bool success = std::filesystem::remove(filePath);
        env->ReleaseStringUTFChars(file, fileChars);
        return success ? 0 : -2;
    }
    catch (const std::exception& e) {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
        env->ReleaseStringUTFChars(file, fileChars);
        return -3;
    }
}

JNIEXPORT jint JNICALL Java_shen_zhien_filestream_FileTools_RenameFile(JNIEnv* env, jclass cla, jstring oldName, jstring newName) {
    const char* oldNameChars = env->GetStringUTFChars(oldName, nullptr);
    const char* newNameChars = env->GetStringUTFChars(newName, nullptr);
    if (!oldNameChars || !newNameChars) {
        if (oldNameChars) env->ReleaseStringUTFChars(oldName, oldNameChars);
        if (newNameChars) env->ReleaseStringUTFChars(newName, newNameChars);
        return -1;
    }
    std::filesystem::path oldPath(oldNameChars);
    std::filesystem::path newPath(newNameChars);
    try {
        std::filesystem::rename(oldPath, newPath);
        env->ReleaseStringUTFChars(oldName, oldNameChars);
        env->ReleaseStringUTFChars(newName, newNameChars);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        env->ReleaseStringUTFChars(oldName, oldNameChars);
        env->ReleaseStringUTFChars(newName, newNameChars);
        return -2;
    }
}

JNIEXPORT jlong JNICALL Java_shen_zhien_filestream_FileTools_GetFileSize(JNIEnv* env, jclass cla, jstring file) {
    const char* fileChars = env->GetStringUTFChars(file, nullptr);
    if (!fileChars) {
        return -1;
    }
    std::filesystem::path filePath(fileChars);
    try {
        std::uintmax_t size = std::filesystem::file_size(filePath);
        env->ReleaseStringUTFChars(file, fileChars);
        return static_cast<jlong>(size);
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting file size: " << e.what() << std::endl;
        env->ReleaseStringUTFChars(file, fileChars);
        return -2;
    }
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_CreateDirectory(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return JNI_FALSE;
    bool success = std::filesystem::create_directories(pathChars);
    env->ReleaseStringUTFChars(path, pathChars);
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_IsDirectory(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return JNI_FALSE;
    bool isDir = std::filesystem::is_directory(pathChars);
    env->ReleaseStringUTFChars(path, pathChars);
    return isDir ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobjectArray JNICALL Java_shen_zhien_filestream_FileTools_ListDirectory(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return nullptr;

    std::vector<std::string> entries;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(pathChars)) {
            entries.push_back(entry.path().filename().string());
        }
    }
    catch (...) {
        env->ReleaseStringUTFChars(path, pathChars);
        return nullptr;
    }

    env->ReleaseStringUTFChars(path, pathChars);

    jobjectArray result = env->NewObjectArray(
        entries.size(),
        env->FindClass("java/lang/String"),
        env->NewStringUTF("")
    );

    for (size_t i = 0; i < entries.size(); i++) {
        env->SetObjectArrayElement(result, i, env->NewStringUTF(entries[i].c_str()));
    }

    return result;
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_CopyFile(JNIEnv* env, jclass, jstring src, jstring dest, jboolean overwrite) {
    const char* srcChars = env->GetStringUTFChars(src, nullptr);
    const char* destChars = env->GetStringUTFChars(dest, nullptr);
    if (!srcChars || !destChars) {
        if (srcChars) env->ReleaseStringUTFChars(src, srcChars);
        if (destChars) env->ReleaseStringUTFChars(dest, destChars);
        return JNI_FALSE;
    }

    try {
        auto options = overwrite ?
            std::filesystem::copy_options::overwrite_existing :
            std::filesystem::copy_options::none;
        std::filesystem::copy_file(srcChars, destChars, options);
    }
    catch (...) {
        env->ReleaseStringUTFChars(src, srcChars);
        env->ReleaseStringUTFChars(dest, destChars);
        return JNI_FALSE;
    }

    env->ReleaseStringUTFChars(src, srcChars);
    env->ReleaseStringUTFChars(dest, destChars);
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL Java_shen_zhien_filestream_FileTools_GetLastModifiedTime(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return -1;

    try {
        auto timePoint = std::filesystem::last_write_time(pathChars);
        auto duration = timePoint.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        env->ReleaseStringUTFChars(path, pathChars);
        return static_cast<jlong>(millis);
    }
    catch (...) {
        env->ReleaseStringUTFChars(path, pathChars);
        return -1;
    }
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_CanRead(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return JNI_FALSE;

    std::ifstream file(pathChars);
    bool canRead = file.good();
    file.close();

    env->ReleaseStringUTFChars(path, pathChars);
    return canRead ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_shen_zhien_filestream_FileTools_CanWrite(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return JNI_FALSE;

    // 尝试以追加模式打开文件
    std::ofstream file(pathChars, std::ios::app);
    bool canWrite = file.good();
    file.close();

    env->ReleaseStringUTFChars(path, pathChars);
    return canWrite ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL Java_shen_zhien_filestream_FileTools_GetAbsolutePath(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return nullptr;

    std::string absolutePath;
    try {
        absolutePath = std::filesystem::absolute(pathChars).string();
    }
    catch (...) {
        env->ReleaseStringUTFChars(path, pathChars);
        return nullptr;
    }

    env->ReleaseStringUTFChars(path, pathChars);
    return env->NewStringUTF(absolutePath.c_str());
}

JNIEXPORT jlong JNICALL Java_shen_zhien_filestream_FileTools_DeleteDirectory(JNIEnv* env, jclass, jstring path) {
    const char* pathChars = env->GetStringUTFChars(path, nullptr);
    if (!pathChars) return -1;

    try {
        std::uintmax_t count = std::filesystem::remove_all(pathChars);
        env->ReleaseStringUTFChars(path, pathChars);
        return static_cast<jlong>(count);
    }
    catch (...) {
        env->ReleaseStringUTFChars(path, pathChars);
        return -1;
    }
}

JNIEXPORT jstring JNICALL Java_shen_zhien_filestream_FileTools_CreateTempFile(JNIEnv* env, jclass, jstring prefix, jstring suffix, jstring dir) {
    const char* prefixChars = env->GetStringUTFChars(prefix, nullptr);
    const char* suffixChars = env->GetStringUTFChars(suffix, nullptr);
    const char* dirChars = env->GetStringUTFChars(dir, nullptr);

    if (!prefixChars || !suffixChars) {
        if (prefixChars) env->ReleaseStringUTFChars(prefix, prefixChars);
        if (suffixChars) env->ReleaseStringUTFChars(suffix, suffixChars);
        if (dirChars) env->ReleaseStringUTFChars(dir, dirChars);
        return nullptr;
    }

    std::string dirPath = dirChars ? dirChars : std::filesystem::temp_directory_path().string();
    std::string fileName = std::string(prefixChars) +
        std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) +
        std::string(suffixChars);
    std::string fullPath = (std::filesystem::path(dirPath) / fileName).string();

    try {
        std::ofstream file(fullPath);
        file.close();
    }
    catch (...) {
        env->ReleaseStringUTFChars(prefix, prefixChars);
        env->ReleaseStringUTFChars(suffix, suffixChars);
        env->ReleaseStringUTFChars(dir, dirChars);
        return nullptr;
    }

    env->ReleaseStringUTFChars(prefix, prefixChars);
    env->ReleaseStringUTFChars(suffix, suffixChars);
    env->ReleaseStringUTFChars(dir, dirChars);
    return env->NewStringUTF(fullPath.c_str());
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}