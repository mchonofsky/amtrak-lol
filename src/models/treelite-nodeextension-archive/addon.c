// addon.c
#include "addon.h"
#include "header.h"
#include <assert.h>
#include <node_api.h>
#include <stdio.h>

#define PREDICTION_ARGC 9

static napi_value Predict(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = PREDICTION_ARGC;
  napi_value args[PREDICTION_ARGC];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  if (argc < PREDICTION_ARGC) {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype[PREDICTION_ARGC];
  union Entry values[PREDICTION_ARGC]; 
  double holder;

  for (int ii = 0; ii < PREDICTION_ARGC; ii++) {
      status = napi_typeof(env, args[ii], &valuetype[ii]);
      // requires numeric input (for now) or null
      // could be just one valuetype
      assert(status == napi_ok);
      if (valuetype[ii] == napi_null || valuetype[ii] == napi_undefined)
      {
          values[ii].missing = -1;
      }
      else if (valuetype[ii] == napi_number) {
        status = napi_get_value_double(env, args[0], &holder);
        values[ii].fvalue = (float) holder;
        assert(status == napi_ok);
      }
      else if (valuetype[ii] != napi_number) {
          napi_throw_type_error(env, NULL, "Wrong arguments");
          return NULL;
      }
  }
  
  napi_value sum;
  status = napi_create_double(env, predict(&values[0], 0), &sum);
  assert(status == napi_ok);

  return sum;
}
static napi_value Add(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_number || valuetype1 != napi_number) {
    napi_throw_type_error(env, NULL, "Wrong arguments");
    return NULL;
  }

  double value0;
  status = napi_get_value_double(env, args[0], &value0);
  assert(status == napi_ok);

  double value1;
  status = napi_get_value_double(env, args[1], &value1);
  assert(status == napi_ok);

  napi_value sum;
  status = napi_create_double(env, value0 + value1, &sum);
  assert(status == napi_ok);

  return sum;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("add", Add);
  napi_property_descriptor addDescriptor2 = DECLARE_NAPI_METHOD("predict", Predict);
  status = napi_define_properties(env, exports, 1, &addDescriptor);
  status = napi_define_properties(env, exports, 1, &addDescriptor2);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
