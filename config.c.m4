dnl $Id$
dnl config.m4 for extension php_test

dnl PHP_ARG_WITH(php_test, for php_test support,
dnl [  --with-php_test             Include php_test support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(php_test, whether to enable php_test support,
 [  --enable-php_test           Enable php_test support])

if test "$PHP_EXT_TEST" != "no"; then
   LIBNAME1=libname1.so  dnl dynamic lib name
   LIBSYMBOL1=lib1function dnl dynamic lib fuctuin name(symbol T)

   LIBNAME2=libname2.so dnl dynamic lib name
   LIBSYMBOL2=lib2function dnl dynamic lib fuction name(symbol T)

dnl check libname1.so, use fuction name
   PHP_CHECK_LIBRARY($LIBNAME1,$LIBSYMBOL1,
   [
     PHP_ADD_LIBRARY_WITH_PATH($LIBNAME1, /usr/lib64, PHP_TESTL_SHARED_LIBADD)
     AC_DEFINE(HAVE_PHP_TESTLLIB,1,[ ])
   ],[
     AC_MSG_ERROR([wrong php_test lib64 version or lib64 not found]) dnl you can write anything when checking lib error display 
   ],[
      -lpthread -ldl -lrt -L/usr/lib64 -llib2 dnl add dynamic lib depend on some othter or system lib
   ])

dnl check libname2.so, use function name
   PHP_CHECK_LIBRARY($LIBNAME2,$LIBSYMBOL2,
   [
     PHP_ADD_LIBRARY_WITH_PATH($LIBNAME2, /lib, PHP_TESTL_SHARED_LIBADD)
     AC_DEFINE(HAVE_PHP_TESTLLIB,1,[ ])
   ],[
     AC_MSG_ERROR([wrong php_test lib64 version or lib64 not found])
   ],[
      -lpthread -ldl -lrt -L/lib
   ])

dnl add dynamic lib to PHP
   PHP_ADD_LIBRARY(name1, /usr/lib64, PHP_TESTL_SHARED_LIBADD)
   PHP_ADD_LIBRARY(name2, /lib, PHP_TESTL_SHARED_LIBADD)
   PHP_SUBST(PHP_TESTL_SHARED_LIBADD)

dnl add c file in Makefile  
   PHP_NEW_EXTENSION(php_test, php_test.c php_test1.c php_test2.c, $ext_shared)
fi
