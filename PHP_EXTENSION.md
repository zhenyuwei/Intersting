# PHP 扩展

关联文件config.c.m4和config.cpp.m4

Linux PHP 扩展添加方式，文件config.c.m4和config.cpp.m4分别为C工程和C++工程扩展，根据文件内描述添加对应的PHP扩展

文件添加完成后，执行phpize生成Makefile文件，打开Makefile文件查看在config.m4中添加的C或者C++文件

执行make && make install

执行make之前，请查看代码中.c或者.cpp文件内是否有宏定义，是否需要在Makefile中添加，如果需要，则在COMMON_CFLAGS字段后添加

## PHP扩展步骤

1. 脚本生成相关文件，PHP源码的ext目录下，执行 ./ext_skel --extname=test，执行后得到test文件夹

2. 修改配置文件，打开php_test.h文件，在PHP_FUNCTION(comfirm_test_complied);下加入需要的php方法声明，例如这里给出的PHP_FUNCTION(test_php);保存文件并退出

3. 修改config.m4文件，根据需求删除dnl注释的内容，参考上述给出的config.c.m4和config.cpp.m4文件修改

4. 修改函数文件test.c，修改内容为const zend_function_entry_urinecheck_funtions[] = { PHP_FE(confirm_test_complied, NULL) 

     PHP_FE(test_php,NULL) 

     {NULL,NULL,NULL}

     //PHP_FE_END /*Must be the last line in urinecheck_functions[] */
   };

5. 在test.c文件底部加入主体函数

   PHP_FUNCTION(test_php){

	int c = functions(a,b); //funtions 可以为第三方库	

	RETURN_LONG(c);

   }

6. 执行完成后编译，使用phpize生产必要的Makefile文件

7. 执行make

8. 执行make test,如果Tests passed: (100.0%) (100.0%)这一项值提示为100%，证明测试成功了，可以直接生成使用了。如果没通过，查看test里的错误信息，然后解决问题

9. make install后会在当前目录lib下生成扩展库，通过添加php.ini文件的extension_dir调用


### 添加CPP文件

添加C++文件和C文件存在差异，如何添加C++文件，需要修改cpp文件中自带的include，在include前加上extern "C"修饰，例如下面例子：

extern "C"{

#ifdef HAVE_CONFIG_H

#include "config.h"

#endif

#include "php.h"

#include "php_ini.h"

}

ZEND_GET_MODULE也要用extern “C”修饰，这里注意的是，修饰 ZEND_GET_MODULE ，用的是宏

#ifdef COMPILE_DL_CHATSPAM

BEGIN_EXTERN_C()

ZEND_GET_MODULE(chatspam)

END_EXTERN_C()

#endif

被extern "C"修饰的变量和函数是按照C语言方式编译和连接的

把需要用的c方法的声明引入进来

### 优化

全局变量

//file php_test.h

ZEND_BEGIN_MODULE_GLOBALS(ptest)

char *dict_path;

ZEND_END_MODULE_GLOBALS(ptest)

//file test.cpp 

//取消这行的注释

ZEND_DECLARE_MODULE_GLOBALS(pptest)

读取ini配置

STD_PHP_INI_ENTRY("ptest.path", "", PHP_INI_ALL,
 OnUpdateString, path, zend_test_globals, test_globals)

 //MINIT取消注释

 REGISTER_INI_ENTRIES();

初始化时载入字典

True global resources 处声明全局变量、将变量在MINIT阶段初始化、function中直接调用
