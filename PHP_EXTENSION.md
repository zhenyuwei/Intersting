# PHP 扩展

关联文件config.c.m4和config.cpp.m4

Linux PHP 扩展添加方式，文件config.c.m4和config.cpp.m4分别为C工程和C++工程扩展，根据文件内描述添加对应的PHP扩展

文件添加完成后，执行phpize生成Makefile文件，打开Makefile文件查看在config.m4中添加的C或者C++文件

执行make && make install

执行make之前，请查看代码中.c或者.cpp文件内是否有宏定义，是否需要在Makefile中添加，如果需要，则在COMMON_CFLAGS字段后添加
