// Autogenerated by - D:/home/lugansky-igor/github/in-the-vicinity-cc/embedded_script_engines/tools/point_gen.py

//#include "app/config.h"  // TODO: Need impl.
#include "forge_v8_point.h"  // TODO: Need connect real.

// C++
#include <string>

using std::string;

using v8::Object;
using v8::Handle;
using v8::Local;
using v8::Value;
using v8::ObjectTemplate;
using v8::External;
using v8::Context;
using v8::Isolate;
using v8::Persistent;
using v8::Integer;
using v8::String;
void ForgeV8unknowns::v8_getter_lengthAllParams___(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<unknown*>(ptr)->lengthAllParams___;
  info.GetReturnValue().Set(Integer::New(value));
}

void ForgeV8unknowns::v8_getter_HLTypeCodesQuerySTL(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  std::string value = static_cast<unknown*>(ptr)->HLTypeCodesQuerySTL;
  info.GetReturnValue().Set(String::New(value));
}

void ForgeV8unknowns::v8_getter_printPABInPowerOk_[kMaxPABs](
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<unknown*>(ptr)->printPABInPowerOk_[kMaxPABs];
  info.GetReturnValue().Set(Integer::New(value));
}
