
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_php_tc.h"

#include <tcutil.h>
#include <tchdb.h>

zend_class_entry *php_tc_ce;

// destructor
static void php_php_tc_dtor(void *object TSRMLS_DC){
  int ecode;
  php_php_tc *obj = (php_php_tc *)object;
  if(obj->hdb){
    /* close the database */
    if(!tchdbclose(obj->hdb)){
      ecode = tchdbecode(obj->hdb);
      fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
    }
  }
  if(obj->filename){
    efree(obj->filename);
  }
  zend_object_std_dtor(&obj->std TSRMLS_CC);
  efree(obj);
}

// constructor
static zend_object_value php_php_tc_new(zend_class_entry *ce TSRMLS_DC){


  zend_object_value retval;
  php_php_tc *obj;

  obj = (php_php_tc *)ecalloc(1,sizeof(php_php_tc));
  zend_object_std_init( &obj->std, ce TSRMLS_CC);
  object_properties_init(&obj->std, ce);


  retval.handle = zend_objects_store_put(obj,
    (zend_objects_store_dtor_t) zend_objects_destroy_object,
    (zend_objects_free_object_storage_t) php_php_tc_dtor,
    NULL TSRMLS_CC
  );
  retval.handlers = zend_get_std_object_handlers();
  return retval;
}

/*
 constructor for php
 string arg_fname  filename for open dbfile
 bool arg_write_mode  openmode false=read only withdout lock, true=create dbfile
 */
PHP_METHOD(phpTC, __construct) {
  char *arg_fname;
	int arg_fname_len;
  bool arg_write_mode;
  int ecode;// hdb error code
  int open_mode;// hdb open mode
  php_php_tc *object = (php_php_tc *) zend_object_store_get_object(getThis() TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &arg_fname, &arg_fname_len, &arg_write_mode ) == FAILURE) {
    php_error(E_ERROR, "param erorr"); 
		return;
	}
  spprintf(&object->filename , 0, "%s", arg_fname);

  if(arg_write_mode){
    open_mode = HDBOWRITER | HDBOCREAT;        
  }else{
    open_mode = HDBOREADER | HDBONOLCK;
  }


  object->hdb = tchdbnew();
   /* open the database */
  if(!tchdbopen(object->hdb, object->filename, open_mode)){
    ecode = tchdbecode(object->hdb);
    fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
  }

}


PHP_METHOD(phpTC, getFileName){
  php_php_tc *object = (php_php_tc *) zend_object_store_get_object(getThis() TSRMLS_CC);
  RETURN_STRING(object->filename, 1);
}

PHP_METHOD(phpTC, get){
  char *key;
	int key_len, value_len;
	char*value;
  php_php_tc *object = (php_php_tc *) zend_object_store_get_object(getThis() TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
		return;
	}
  /* printf("key %s\n", key);*/
  value = tchdbget2(object->hdb, key);
  if(!value){
    free(value);
    RETURN_FALSE;
    return;
  }
  /* printf("value %s\n", value); */
  RETURN_STRING(value, 1);
  free(value);
}

PHP_METHOD(phpTC, set){
	int key_len, value_len;
	char *key, *value;
  int ecode;// hdb error code
  php_php_tc *object = (php_php_tc *) zend_object_store_get_object(getThis() TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len,  &value, &value_len) == FAILURE) {
		return;
	}

  // record set
  if(!tchdbput2(object->hdb, key, value)){
    ecode = tchdbecode(object->hdb);
    fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));
    return ;
  }
}

/* If you declare any globals in php_php_tc.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(php_tc)
*/

/* True global resources - no need for thread safety here */
static int le_php_tc;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("php_tc.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_php_tc_globals, php_tc_globals)
    STD_PHP_INI_ENTRY("php_tc.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_php_tc_globals, php_tc_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_php_tc_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_php_tc_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "php_tc", arg);
	RETURN_STRINGL(strg, len, 0);
}


PHP_FUNCTION(php_tc_test)
{
  TCHDB *hdb;
  int ecode;
  char *key, *value;

  /* create the object */
  hdb = tchdbnew();

  /* open the database */
  if(!tchdbopen(hdb, "/dev/shm/caskettest.tch", HDBOWRITER | HDBOCREAT)){
    ecode = tchdbecode(hdb);
    fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
  }

  /* store records */
  if(!tchdbput2(hdb, "foo", "hop") ||
     !tchdbput2(hdb, "bar", "step") ||
     !tchdbput2(hdb, "baz", "jump")){
    ecode = tchdbecode(hdb);
    fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));
  }

  /* retrieve records */
  value = tchdbget2(hdb, "foo");
  if(value){
    printf("%s\n", value);
    free(value);
  } else {
    ecode = tchdbecode(hdb);
    fprintf(stderr, "get error: %s\n", tchdberrmsg(ecode));
  }

  /* traverse records */
  tchdbiterinit(hdb);
  while((key = tchdbiternext2(hdb)) != NULL){
    value = tchdbget2(hdb, key);
    if(value){
      printf("%s:%s\n", key, value);
      free(value);
    }
    free(key);
  }

  /* close the database */
  if(!tchdbclose(hdb)){
    ecode = tchdbecode(hdb);
    fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
  }

  /* delete the object */
  // tchdbdel(hdb);



}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_php_tc_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_php_tc_init_globals(zend_php_tc_globals *php_tc_globals)
{
	php_tc_globals->global_value = 0;
	php_tc_globals->global_string = NULL;
}
*/
/* }}} */


const zend_function_entry php_tc_methods[] = {
  PHP_ME(phpTC, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(phpTC, getFileName, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(phpTC, get, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(phpTC, set, NULL, ZEND_ACC_PUBLIC)
  { NULL, NULL, NULL }
};

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(php_tc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce,"phpTC", php_tc_methods);
  php_tc_ce = zend_register_internal_class(&ce TSRMLS_CC);
  php_tc_ce->create_object = php_php_tc_new; 
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(php_tc)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(php_tc)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(php_tc)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(php_tc)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "php_tc support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ php_tc_functions[]
 *
 * Every user visible function must have an entry in php_tc_functions[].
 */
const zend_function_entry php_tc_functions[] = {
	PHP_FE(confirm_php_tc_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(php_tc_test,	NULL)
	PHP_FE_END	/* Must be the last line in php_tc_functions[] */
};
/* }}} */


/* {{{ php_tc_module_entry
 */
zend_module_entry php_tc_module_entry = {
	STANDARD_MODULE_HEADER,
	"php_tc",
	php_tc_functions,
	PHP_MINIT(php_tc),
	PHP_MSHUTDOWN(php_tc),
	PHP_RINIT(php_tc),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(php_tc),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(php_tc),
	PHP_PHP_TC_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHP_TC
ZEND_GET_MODULE(php_tc)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
