dnl $Id$
dnl config.m4 for extension php_tc

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(php_tc, for php_tc support,
dnl Make sure that the comment is aligned:
dnl [  --with-php_tc             Include php_tc support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(php_tc, whether to enable php_tc support,
Make sure that the comment is aligned:
[  --enable-php_tc           Enable php_tc support])

if test "$PHP_PHP_TC" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-php_tc -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/php_tc.h"  # you most likely want to change this
  dnl if test -r $PHP_PHP_TC/$SEARCH_FOR; then # path given as parameter
  dnl   PHP_TC_DIR=$PHP_PHP_TC
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for php_tc files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PHP_TC_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PHP_TC_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the php_tc distribution])
  dnl fi

  dnl # --with-php_tc -> add include path
  dnl PHP_ADD_INCLUDE($PHP_TC_DIR/include)

  dnl # --with-php_tc -> check for lib and symbol presence
  dnl LIBNAME=php_tc # you may want to change this
  dnl LIBSYMBOL=php_tc # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PHP_TC_DIR/$PHP_LIBDIR, PHP_TC_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PHP_TCLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong php_tc lib version or lib not found])
  dnl ],[
  dnl   -L$PHP_TC_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PHP_TC_SHARED_LIBADD)


  AC_MSG_CHECKING(for pkg-config)
  if test ! -f "$PKG_CONFIG"; then
	PKG_CONFIG=`which pkg-config`
  fi


  PHP_NEW_EXTENSION(php_tc, php_tc.c , $ext_shared)
  PHP_SUBST(PHP_TC_SHARED_LIBADD)

  if test -f "$PKG_CONFIG"; then
	AC_MSG_RESULT(found pkg-config)

  	if $PKG_CONFIG --exists tokyocabinet; then
		AC_MSG_RESULT(php_tc : found tokyocabinet)
		ZIP_LIBS=`$PKG_CONFIG --libs tokyocabinet`
		ZIP_INCS=`$PKG_CONFIG --cflags-only-I tokyocabinet`
		AC_MSG_RESULT(tokyocabinet includes : $ZIP_INCS)
		AC_MSG_RESULT(tokyocabinet links : $ZIP_LIBS)

		PHP_TC_LIBS="$LDFLAGS $ZIP_LIBS"
		PHP_TC_INCS="$CFLAGS $ZIP_INCS"
	
		PHP_EVAL_INCLINE($PHP_TC_INCS)
		PHP_EVAL_LIBLINE($PHP_TC_LIBS, PHP_TC_SHARED_LIBADD)
	else
		AC_MSG_RESULT(not found tokyocabinet)
	fi
  fi
dnl  PHP_ADD_BUILD_DIR([$ext_builddir/libs/tokyocabinet-1.4.48])
fi
