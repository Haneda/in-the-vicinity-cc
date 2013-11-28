// C++
#include <string>

// Other
#include <gtest/gtest.h>
#include <v8.h>

// App
#include "process.h"
#include "point.h"

using std::string;

using v8::Isolate;
using v8::String;
using v8::Handle;
using v8::HandleScope;

using v8::Context;
using v8::Persistent;

class PointV8Engine {
 public:
  static PointV8Engine* CreateForOwn(
      Isolate* isolate, 
      Handle<String> source,
      V8Point* point,
      Point* real_point) 
    {
    PointV8Engine* engine = new PointV8Engine(isolate, source, point);
    engine->Initialize(real_point);
    return engine;
  }
  

 protected:
  PointV8Engine(
    Isolate* isolate, 
    Handle<String> source,
    V8Point* point) 
    : isolate_(isolate), source_(source), point_(point) { }

  void Initialize(Point* real_point) {
    HandleScope handle_scope(GetIsolate());

    // Create a template for the global object where we set the
    // built-in global functions.
    Handle<ObjectTemplate> global = ObjectTemplate::New();

    // Expose logger
    global->Set(String::New("log"), 
        FunctionTemplate::New(LogCallback));

    //@Point:
    // Create context?
    // Each processor gets its own context so different processors don't
    // affect each other. Context::New returns a persistent handle which
    // is what we need for the reference to remain after we return from
    // this method. That persistent handle has to be disposed in the
    // destructor.
    v8::Handle<v8::Context> context = 
        Context::New(GetIsolate(), NULL, global);
    context_.Reset(GetIsolate(), context);

    // Enter the new context so all the following operations take place
    // within it.
    Context::Scope context_scope(context);

    //@Point
    // Install
    Handle<Object> output_obj = WrapPoint(real_point);
    context->Global()->Set(String::New("point_zero"), output_obj);

    //@Point
    // Run Script
    bool success = ExecuteScript(source_, isolate_);
  }

 private:
  Isolate* GetIsolate() { return isolate_; }
  Isolate* isolate_;  // �������� �������� ��������
  Handle<String> source_;
  
  // "A context is an execution environment that allows separate, 
  // unrelated, JavaScript code to run in a single instance of 
  // V8. You must explicitly specify the context in which you 
  // want any JavaScript code to be run."
  //
  // ��� �������������� ������ �� �����.
  Persistent<Context> context_;

  // ����� � �� ����� �����
  V8Point* point_;

  // Blueprints
  static Persistent<ObjectTemplate> point_template_;


  // ������� � Persistent - ������� ���� wrap-������� �� ������� � V8Point
  // �� ������� �� ����� � �����. ����� ���� �������� �� scope/context
  Handle<Object> WrapPoint(Point* point) {
    HandleScope handle_scope(GetIsolate());
    if (point_template_.IsEmpty()) {
      Handle<ObjectTemplate> raw_template = 
          point_->CreateBlueprint(GetIsolate());

      // ���������, �� ������ ����� � ������� ������������
      point_template_.Reset(GetIsolate(), raw_template);
    }

    // ����� ����������� �������� ������
    // ������ ����� ������� ������ �������
    // Create an empty map wrapper.
    Handle<ObjectTemplate> templ =
        Local<ObjectTemplate>::New(GetIsolate(), point_template_);
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Handle<External> map_ptr = External::New(point);

    // Store the map pointer in the JavaScript wrapper.
    result->SetInternalField(0, map_ptr);
    return handle_scope.Close(result);
  }
};



Persistent<ObjectTemplate> PointV8Engine::point_template_;

TEST(PointEngine, Create) {
  v8::V8::InitializeICU();
  string file = "..\\scripts\\point.js";
  EXPECT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();

  // ������ ����� ������� - ��� ��� �� ���� ���� ��� V8
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  EXPECT_NE(true, source.IsEmpty());

  // Point
  V8Point v8_point;
<<<<<<< HEAD
  Point point_real(1, 2);
=======
	//Point point;
>>>>>>> 405fe7e5bbe62b090b92f648c15998459e98fe59

  // Engine
  PointV8Engine* engine = PointV8Engine::CreateForOwn(
      isolate, source, &v8_point, &point_real);

  EXPECT_EQ(199, point_real.x_);
  EXPECT_EQ(42, point_real.y_);

  delete engine;
}