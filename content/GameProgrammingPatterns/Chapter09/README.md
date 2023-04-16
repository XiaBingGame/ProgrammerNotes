* 调用 PeekMessage() 处理并从操作系统中分派事件, 不同于 GetMessage(), PeekMessage() 并不阻塞等待用户输入.
* 浏览器平台, 事件循环已根植在浏览器执行模式的底层, 其中事件循环负责显示, 同样要使用它来作为你的游戏循环。调用 requestAnimationFrame() 之类的函数以便浏览器回调你的程序，并维持游戏的运转。
* 最简单的游戏循环(能跑多快就跑多快)
```
while(true) {
    processInput();
    update();
    render();
}
```
* 稳定一个帧率(对于较慢的机器，等待的时间会为负)
```
while(true) {
    double start = getCurrentTime();
    processInput();
    update();
    render();

    sleep(start + MS_PER_FRAME - getCurrentTime());
}
```
* 变值时间步长
```
double lastTime = getCurrentTime();
while (true) {
    double current = getCurrentTime();
    double elpased = current - lastTime;
    processInput();
    update(elpased);
    render();
    lastTime = current;
}
```
* 该代码的优缺点如下：
    * 游戏在不同的硬件上以相同的速率运行
    * 高端机器的玩家能够得到一个更流畅的游戏体验。
    * 容易累积浮点数误差
    * 物理引擎和AI可能使用固定步长，所以变值时间步长可能会不稳定
    * 物理和网络模块在变时步长下变得尤为困难。
* 修改代码, 使用固定步长更新以保证物理引擎和AI的稳定。
```
double previous = getCurrentTime();
double lag = 0.0;
while (true) {
    double current = getCurrentTime();
    double elpased = current - previous;
    previous = current;
    lag += elpased;
    processInput();

    while(lag >= MS_PER_UPDATE) {
        update();
        lag -= MS_PER_UPDATE;
    }
    render();
}
```
* 上面的方法在低端电脑里还是会有跳帧的情况。
* 渲染的频率低于更新的频率。
* 渲染时可以传入lag剩余的量修正目标的位置
```
render(lag / MS_PER_UPDATE);
```
* 假设子弹在距离屏幕左侧20像素的地方以400像素每帧的速度向右移动，假设传入render()的参数是0.5,则绘制时，子弹位于距离屏幕左侧 220 像素的位置处。
* 不同方案的设计决策
    * 使用平台(浏览器)的事件循环
        - 相对简单，无须担心游戏核心循环的代码和优化问题
        - 无需担心何时处理事件，如何捕获事件，如何处理平台与你的输入模型之间不匹配的问题等。
        - 失去对时间的控制
    * 使用游戏引擎的游戏循环
        - 查看游戏引擎是否具有一个紧凑靠谱的游戏循环。
        - 需求不符合时，可能无法获得循环的控制权
    * 自己编写游戏循环
        - 掌控一切
        - 需要实现平台的接口，划分出一些时间处理应用程序框架和操作系统的事件。
* 对于移动设备，可能需要减少CPU的使用，如设置帧率上限(30FPS或60FPS)以减少能耗。