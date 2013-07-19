#include "ruby.h"
#include "unqlite.h"

#define UNUSED(x) (x = x)

VALUE cStore;
VALUE mCaja;
VALUE eStoreError;

static void store_free(void *db)
{
    unqlite_close((unqlite *)db);
    unqlite_lib_shutdown();
}


VALUE
store_new(VALUE class, VALUE path)
{
    Check_Type(path, T_STRING);
    unqlite *db = NULL;
    
    int ret = unqlite_open(&db, StringValuePtr(path), UNQLITE_OPEN_CREATE);
    if (ret != UNQLITE_OK || db == NULL) {
        unqlite_close(db);
        rb_raise(rb_eIOError, "invalid path");
    }
    
    VALUE tdata = Data_Wrap_Struct(class, 0, store_free, db);
    VALUE argv[1];
    argv[0] = path;
    rb_obj_call_init(tdata, 1, argv);
    return tdata;
}

static VALUE
store_init(VALUE self, VALUE path)
{
    Check_Type(path, T_STRING);
    rb_iv_set(self, "@path", path);
    return self;
}

static VALUE
store_path(VALUE self)
{
    return rb_iv_get(self, "@unit");
}

static VALUE
store_store(VALUE self, VALUE key, VALUE value)
{
    Check_Type(key, T_STRING);
    Check_Type(value, T_STRING);
    unqlite *db;
    Data_Get_Struct(self, unqlite, db);

    int key_len = RSTRING_LEN(key);
    char *key_ptr = RSTRING_PTR(key);
    int value_len = RSTRING_LEN(value);
    char *value_ptr = RSTRING_PTR(value);

    int ret = unqlite_kv_store(db, key_ptr, key_len, value_ptr, value_len);

    if (ret < 0) {
        if (ret == UNQLITE_IOERR) {
            unqlite_rollback(db);
        }
        rb_raise(eStoreError, NULL);
    }
    return Qtrue;
}

static VALUE
store_append(VALUE self, VALUE key, VALUE value)
{
    Check_Type(key, T_STRING);
    Check_Type(value, T_STRING);
    unqlite *db;
    Data_Get_Struct(self, unqlite, db);

    int key_len = RSTRING_LEN(key);
    char *key_ptr = RSTRING_PTR(key);
    int value_len = RSTRING_LEN(value);
    char *value_ptr = RSTRING_PTR(value);

    int ret = unqlite_kv_append(db, key_ptr, key_len, value_ptr, value_len);

    if (ret < 0) {
        rb_raise(eStoreError, NULL);
    }
    return Qtrue;
}

static VALUE
store_fetch(VALUE self, VALUE key)
{
    Check_Type(key, T_STRING);
    unqlite *db;
    Data_Get_Struct(self, unqlite, db);

    int size = 0;
    int key_len = RSTRING_LEN(key);
    char *key_ptr = RSTRING_PTR(key);

    // We'll prefetch the data to get the size
    int ret = unqlite_kv_fetch(db, key_ptr, key_len, NULL, &size);
    if (ret != UNQLITE_OK) {
        rb_raise(eStoreError, "key not found");
    }
    char *buf = malloc(size);
    if (buf == NULL) {
        rb_raise(eStoreError, "could not allocate memory");
    }
    ret = unqlite_kv_fetch(db, key_ptr, key_len, buf, &size);
    if (ret != UNQLITE_OK) {
        rb_raise(eStoreError, "error while fetching the data");
    }
    VALUE result = rb_str_new2(buf);
    return result;
}

static VALUE
store_delete(VALUE self, VALUE key)
{
    Check_Type(key, T_STRING);
    unqlite *db;
    Data_Get_Struct(self, unqlite, db);

    int key_len = RSTRING_LEN(key);
    char *key_ptr = RSTRING_PTR(key);

    int ret = unqlite_kv_delete(db, key_ptr, key_len);
    if (ret != UNQLITE_OK) {
        rb_raise(eStoreError, NULL);
    }
    return Qtrue; // TODO
}

void Init_caja_store()
{
    mCaja = rb_define_module("Caja");
    cStore = rb_define_class_under(mCaja, "Store", rb_cObject);
    eStoreError = rb_define_class_under(mCaja, "StoreError", rb_eStandardError);

    rb_define_singleton_method(cStore, "new", store_new, 1);
    rb_define_method(cStore, "initialize", store_init, 1);
    rb_define_method(cStore, "store", store_store, 2);
    rb_define_method(cStore, "append", store_append, 2);
    rb_define_method(cStore, "fetch", store_fetch, 1);
    rb_define_method(cStore, "delete", store_delete, 1);
    rb_define_method(cStore, "path", store_path, 0);
}

