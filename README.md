# SHA256 learning

这是一个尝试自己实现 SHA256 的项目。

经测试，正确得到结果。

## How to use

先在项目文件夹下创建 bin\ 和 obj\ 两个文件夹。

使用如下命令编译代码

```bash
mingw32-make
```

或者（Linux 平台）

```bash
make
```

然后在 bin\ 文件夹下找到 main.exe 文件，使用如下命令调用之

```bash
bin\main.exe 你想要计算的文件路径
```

## Bug

这个项目的代码，如果使用带优化编译（如 -O2）则运行结果不正确，具体原因待弄清。

