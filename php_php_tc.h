
#ifndef PHP_PHP_TC_H
#define PHP_PHP_TC_H

extern zend_module_entry php_tc_module_entry;
#define phpext_php_tc_ptr &php_tc_module_entry

#define PHP_PHP_TC_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_PHP_TC_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_PHP_TC_API __attribute__ ((visibility("default")))
#else
#	define PHP_PHP_TC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <tchdb.h>

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(php_tc)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(php_tc)
*/

/* In every utility function you add that needs to use variables 
   in php_php_tc_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PHP_TC_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
typedef struct _php_php_tc{
  zend_object std;
  TCHDB *hdb;
  char *filename;
} php_php_tc;

#ifdef ZTS
#define PHP_TC_G(v) TSRMG(php_tc_globals_id, zend_php_tc_globals *, v)
#else
#define PHP_TC_G(v) (php_tc_globals.v)
#endif

#endif	/* PHP_PHP_TC_H */

