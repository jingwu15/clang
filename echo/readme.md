Linux 中如何快速查看 C 库函数的头文件以及相应的函数信息？
使用man帮助或grep

1）man命令 函数名，则能够打印该函数的所有说明，当然这个函数必须是Linux-C本身就有的函数。

2）grep "keyword" /usr/include/*.h |grep "typedef"

以上方法可以查找关键字为keyword的结构体、类型、函数原型的定义，typedef可以替换为define尝试查找，几次重复查找找到的定义，就能最终确定。

如：grep “time_t” /usr/include/*.h |grep “typedef”可以查找到“typedef_time_t time_t;”

Locatekeyword.h 可以查找名为keyword 的头文件所在地目录树。

 

Linux下如何查找一个函数在哪个库中

有一些常识或者说平时的积累是必要的，终究常用的库很少。
当我遇到一个函数不知道所属的库的时候，我会先找到头文件所属的包，然后看看那个包（或者相关包）的.a或者.so文件有哪些，然后：
objdump -x 库文件 | grep 函数名
确认函数名是定义而不是引用以后，就知道需要链接这个库了。

比如：查找htons对应的库文件

1）、man htons，里面讲的是arpa/inet.h；
2）、假设你用的是rpm类型的系统。
2.1)、centos/redhat下查看某个文件或命令属于哪个rpm包：

$ yum provides /etc/passwd
或者
$ rpm -qf /etc/passwd
2.2)、ubuntu及衍生版：

sudo dpkg -S whereis或
sudo dpkg-query -S /usr/bin/whereis
不过该命令不如rpm -qf强大，如/etc/passwd就查不出。

2.3)、gentoo及衍生版

#未安装的文件
e-file qmake
#已安装的文件查看
equery b <filename>
或qfile <filename>
如果通过上面还是搜索不到对应的包文件，那就baidu或google搜索了一下。
3）、貌似这个glibc-headers只是个包含头文件的软件包，看看其他类似的软件包是什么：rpm -qa | grep glibc
4）、我看见了glibc，glibc-devel，glibc-common，觉得glibc-devel肯定是提供.a（静态链接）的
5）、rpm -q --filesbypkg glibc-devel| grep \.a$ | awk '{print $2}' |xargs grep htons找到所有带htons的静态库文件
6）、只有一个：/usr/lib/libc.a
7）、如果有好几个的话，用objdump -x看看符号表

查看 Linux 下 *.a 库文件中文件、函数、变量等情况
在 Linux 下经常需要链接一些 *.a 的库文件，那怎么查看这些 *.a 中包含哪些文件、函数、变量：
1. 查看文件：ar -t*.a
2. 查看函数、变量：nm*.a

查找运行程序依赖那些动态库
在linux中查询程序依赖那些动态库， ldd是list, dynamic, dependencies的缩写， 意思是， 列出动态库依赖关系。
是查看运行程序的执行库文件。

