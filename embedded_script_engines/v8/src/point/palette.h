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

  static void GetPointValue(
      Local<String> name,
      const PropertyCallbackInfo<Value>& info) 
    {
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    // ���������� �����!
    Palette* palette = static_cast<Palette*>(ptr);

    // ����� ������� V8Palette ��� ������� ����� �������
    Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(Isolate::GetCurrent(), point_blueprint_);
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Handle<External> map_ptr = External::New(&palette->point_);

    // Store the map pointer in the JavaScript wrapper.
    result->SetInternalField(0, map_ptr);

    // ��� ��� ������� ������!?
    // ������ ������ �� ���!
    info.GetReturnValue().Set<v8::Object>(result);
  }

  v8::Handle<v8::ObjectTemplate> MakeBlueprint(
      v8::Isolate* isolate, 
      v8::Persistent<v8::Context>* context);

  Handle<Object> ForgePalette(
      Palette* palette,
      Isolate* isolate, 
      Persistent<Context>* context,
      Persistent<ObjectTemplate>* blueprint);

//private:
  Isolate* isolate_;
  Persistent<Context>* context_;

  // Real data
  Palette* palette_;

  // ��� ��� ����� ��� ������ � ������.
  static v8::Persistent<v8::ObjectTemplate> point_blueprint_;

  // ��� ��� ����� ��� ������ � �������� �����.
  //v8::Persistent<v8::ObjectTemplate> point_array_blueprint_;
};
