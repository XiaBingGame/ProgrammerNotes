* 单例模式文件系统， 版本1
```
class FileSystem {
public:
    static FileSystem& instance() {
        if(instance_ == NULL) {
            instance_ = new FileSystem;
        }
        return *instance_;
    }

private:
    FileSystem() {}
    static FileSystem* instance_;
};
```
* 单例模式现代版本, C++11保证一个局部静态变量的初始化只进行一次, 哪怕是多线程也是如此.
```
class FileSystem {
public:
    static FileSystem& instance() {
        static FileSystem* instance = new FileSystem();
        return *instance;
    }
private:
    FileSystem() {}
};
```
* 可以继承单例, 让文件封装类跨平台
```
class FileSystem {
public:
    static FileSystem& instance();

    virtual ~FileSystem() {}
    virtual char* read(char* path) = 0;
    virtual void write(char* path, char* text) = 0;

protected:
    FileSystem() {}
};
```
    - 为不同平台定义派生类
```
class PS3FileSystem : public FileSystem {
public:
    virtual char* read(char* path) {
        // Use Sony file IO API...
    }
    virtual void write(char* path) {
        // Use Sony file IO API...
    }
};

class WiiFileSystem : public FileSystem {
public:
    virtual char* read(char* path) {
        // Use Nintendo file IO API...
    }
    virtual void write(char* path) {
        // Use Nontendo file IO API...
    }
};

FileSystem& FileSystem::instance() {
#if PLATFORM == PLAYSTATION3
    static FileSystem* instance = new PS3FileSystem();
#elif PLATFORM == WII
    static FileSystem* instance = new WiiFileSystem();
#endif

    return *FileSystem;
}
```
* 确保单例不提供全局访问
```
class FileSystem {
public:
    FileSystem() {
        assert(!instantiated_);
        instantiated_ = true;
    }

    ~FileSystem() { instantiated_ = false; }

private:
    static bool instantiated_;
};

bool FileSystem::instantiated_ = false;
```
    - 允许创建该实例，但如果创建超过一个实例时， 它会断言并且失败.
* 将全局对象类包装到现有类里面来减少它们的数量
```
class Game {
public:
    static Game& instance() { return instance_; }
    Log& log() { return *log_; }
    FileSystem& fileSystem() { return *files_; }
    AudioPlayer& audioPlayer() { return *audio_; }

private:
    static Game instance_;
    Log *log_;
    FileSystems *files_;
    AudioPlayer *audio_;
};
```