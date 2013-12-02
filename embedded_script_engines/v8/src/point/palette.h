#pragma once

// Other
#include "v8.h"

// App
#include "point.h"

class Palette {
 public:
  Palette() {}
  Point point_;
  Point array[32];
};

using namespace v8;

class V8Palette {
public:
  // Args:
  //   ������ ������ ����� � Isolate and Context.
  //   ��� ���-���� �������� ������ ���� ����.
  V8Palette(
      Palette* palette,  
      Isolate* isolate,
      Persistent<Context>* context);

  static void GetPointX(Local<String> name,
                 const PropertyCallbackInfo<Value>& info) {
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    // ��� ��� ������� ������!?
    info.GetReturnValue().Set(&(static_cast<V8Palette*>(ptr)->point_field_));
  }

  v8::Handle<Object> MakeBlueprint(
      v8::Isolate* isolate, v8::Persistent<v8::Context>* context) 
    {
    HandleScope handle_scope(isolate);

    Context::Scope scope(isolate, *context);

    Handle<ObjectTemplate> result = ObjectTemplate::New();
    result->SetInternalFieldCount(1);

    // Connect getter/setter
    result->SetAccessor(String::New("point"), GetPointX);

    return handle_scope.Close(result);
  }

//private:
  Isolate* isolate_;
  Persistent<Context>* context_;

  // Real data
  Palette* palette_;

  // ��� ��� ����� ��� ������ � ������.
  v8::Persistent<v8::ObjectTemplate> point_blueprint_;
  v8::Persistent<v8::Object> point_field_;

  // ��� ��� ����� ��� ������ � �������� �����.
  //v8::Persistent<v8::ObjectTemplate> point_array_blueprint_;
};
