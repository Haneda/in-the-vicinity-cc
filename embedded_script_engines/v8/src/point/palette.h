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
      Persistent<Context>* context) : palette_(palette), isolate_(isolate), context_(context) 
    { 
    // ����� ������� Persistent!
    HandleScope handle_scope(isolate_);

    Context::Scope scope(isolate_, *context_);
    if (point_blueprint_.IsEmpty()) {
      V8Point point;
      Handle<ObjectTemplate> raw_template = 
          point.CreateBlueprint(isolate, context_);

      // ���������, �� ������ ����� � ������� ������������
      point_blueprint_.Reset(isolate, raw_template);
    }

    ///@Point
    // ����� ����������� �������� ������
    // ������ ����� ������� ������ �������
    // Create an empty map wrapper.
    Handle<ObjectTemplate> templ =
        Local<ObjectTemplate>::New(isolate, point_blueprint_);
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Handle<External> map_ptr = External::New(&this->palette_->point_);

    // Store the map pointer in the JavaScript wrapper.
    result->SetInternalField(0, map_ptr);

    if (point_field_.IsEmpty()) {
      point_field_.Reset(isolate, result);
    }
    //return handle_scope.Close(result);
  }

private:
  Isolate* isolate_;
  Persistent<Context>* context_;

  // Real data
  Palette* palette_;

  // ��� ��� ����� ��� ������ � ������.
  v8::Persistent<v8::ObjectTemplate> point_blueprint_;
  v8::Persistent<v8::Object> point_field_;

  // ��� ��� ����� ��� ������ � �������� �����.
  v8::Persistent<v8::ObjectTemplate> point_array_blueprint_;
};
