* 使用情境， 当下面的条件都成立时，适用双缓冲模式：
    - 我们需要维护一些被逐步改变着的状态量
    - 同个状态可能会在其被修改的同时被访问到
    - 我们希望避免访问状态的代码能看到具体的工作过程
    - 我们希望能够读取状态但不希望等待写入操作的完成
* 通常双缓冲的交换过程和分配一个指针的速度差不多。
* 示例代码
```
class Framebuffer {
public:
    // 构造函数和方法

private:
    static const int WIDTH = 160;
    static const int HEIGHT = 120;

    char pixels_[WIDTH * HEIGHT];
};

void Framebuffer::clear() {
    for ( int i = 0; i < WIDTH * HEIGHT; i++ ) {
        pixels_[i] = WHITE;
    }
}

void Framebuffer::draw(int x, int y) {
    pixels_[(WIDTH * y) + x] = BLACK;
}

const char* Framebuffer::getPixels() {
    return pixels_;
}

class Scene {
public:
    Scene()
    : current_(&buffers_[0]),
    next_(&buffers_[1]) 
    {}

    void draw() {
        next_->clear();
        next_->draw(1, 1);
        // ...
        next_->draw(4, 3);
        swap();
    }

    Framebuffer& getBuffer() { return *current_; }

private:
    void swap() {
        Framebuffer* temp = current_;
        current_ = next_;
        next_ = temp;
    }

    Framebuffer buffers_[2];
    Framebuffer* current_;
    Framebuffer* next_;
};
```