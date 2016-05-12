#include <node.h>
#include <uv.h>
#include <iostream>
#include <unistd.h>
#include "file_manager.hpp"

using namespace std;

namespace file_store_internal {
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Integer;
    using v8::Boolean;
    using v8::Value;
    using v8::Persistent;
    using v8::InvocationCallback;

    struct FileManagerWork {
        uv_work_t  request;
        Persistent<InvocationCallback> success_callback;
        Persistent<InvocationCallback> error_callback;
        std::string file_root;
    };

    struct ReadFileWork: public FileManagerWork {
        std::vector<unsigned char> result;
        std::string file_path;
        int error;
    };

    static void ReadFileAsync(uv_work_t* req) {
        ReadFileWork* work = static_cast<ReadFileWork*>(req->data);
        try {
            work->error = 0;
            work->result = read_file(work->file_root, work->file_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void ReadFileAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        ReadFileWork *work = static_cast<ReadFileWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Value> argv[] = {String::NewFromUtf8(isolate, (char*) &work->result[0])};
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoReadFileAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        ReadFileWork * work = new ReadFileWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[2]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[3]));

        uv_queue_work(uv_default_loop(), &work->request, ReadFileAsync, ReadFileAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct WriteFileWork: public FileManagerWork {
        std::vector<unsigned char> data;
        std::string file_path;
        int error;
    };

    static void WriteFileAsync(uv_work_t* req) {
        WriteFileWork* work = static_cast<WriteFileWork*>(req->data);
        try {
            work->error = 0;
            write_file(work->file_root, work->file_path, work->data);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void WriteFileAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        WriteFileWork *work = static_cast<WriteFileWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), 0, NULL);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoWriteFileAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        WriteFileWork * work = new WriteFileWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        v8::String::Utf8Value tmp(args[1]->ToString());
        work->data.insert(work->data.end(), *tmp, (*tmp)+tmp.length());
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[3]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[4]));

        uv_queue_work(uv_default_loop(), &work->request, WriteFileAsync, WriteFileAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct CopyFileToWork: public FileManagerWork {
        std::string ext_path;
        std::string file_path;
        int error;
    };

    static void CopyFileToAsync(uv_work_t* req) {
        CopyFileToWork* work = static_cast<CopyFileToWork*>(req->data);
        try {
            work->error = 0;
            copy_file_to_storage(work->file_root, work->file_path, work->ext_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void CopyFileToAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        CopyFileToWork *work = static_cast<CopyFileToWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), 0, NULL);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoCopyFileToAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        CopyFileToWork * work = new CopyFileToWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->ext_path = std::string(*v8::String::Utf8Value(args[2]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[3]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[4]));

        uv_queue_work(uv_default_loop(), &work->request, CopyFileToAsync, CopyFileToAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct CopyFileFromWork: public FileManagerWork {
        std::string ext_path;
        std::string file_path;
        int error;
    };

    static void CopyFileFromAsync(uv_work_t* req) {
        CopyFileFromWork* work = static_cast<CopyFileFromWork*>(req->data);
        try {
            work->error = 0;
            copy_file_from_storage(work->file_root, work->file_path, work->ext_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void CopyFileFromAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        CopyFileFromWork *work = static_cast<CopyFileFromWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), 0, NULL);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoCopyFileFromAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        CopyFileFromWork * work = new CopyFileFromWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->ext_path = std::string(*v8::String::Utf8Value(args[2]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[3]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[4]));

        uv_queue_work(uv_default_loop(), &work->request, CopyFileFromAsync, CopyFileFromAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct LinkWork: public FileManagerWork {
        std::string file_path;
        std::string other_path;
        int error;
    };

    static void LinkAsync(uv_work_t* req) {
        LinkWork* work = static_cast<LinkWork*>(req->data);
        try {
            work->error = 0;
            link(work->file_root, work->file_path, work->other_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void LinkAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        LinkWork *work = static_cast<LinkWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), 0, NULL);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoLinkAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        LinkWork * work = new LinkWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->other_path = std::string(*v8::String::Utf8Value(args[2]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[3]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[4]));

        uv_queue_work(uv_default_loop(), &work->request, LinkAsync, LinkAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct DeleteFileWork: public FileManagerWork {
        std::string file_path;
        int error;
    };

    static void DeleteFileAsync(uv_work_t* req) {
        DeleteFileWork* work = static_cast<DeleteFileWork*>(req->data);
        try {
            work->error = 0;
            delete_file(work->file_root, work->file_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void DeleteFileAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        DeleteFileWork *work = static_cast<DeleteFileWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), 0, NULL);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoDeleteFileAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        DeleteFileWork * work = new DeleteFileWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[2]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[3]));

        uv_queue_work(uv_default_loop(), &work->request, DeleteFileAsync, DeleteFileAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    struct ExistsWork: public FileManagerWork {
        bool result;
        std::string file_path;
        int error;
    };

    static void ExistsAsync(uv_work_t* req) {
        ExistsWork* work = static_cast<ExistsWork*>(req->data);
        try {
            work->error = 0;
            work->result = exists(work->file_root, work->file_path);
        } catch (OSError& e) {
            work->error = e.get_err();
        }
    }

    static void ExistsAsyncComplete(uv_work_t *req, int status) {
        Isolate* isolate = Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate);
        ExistsWork *work = static_cast<ExistsWork*>(req->data);

        if (work->error) {
            Local<Value> argv[] = {Integer::New(isolate, work->error)};
            Local<Function>::New(isolate, work->error_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        } else {
            Local<Value> argv[] = {Boolean::New(isolate, work->result)};
            Local<Function>::New(isolate, work->success_callback)->Call(isolate->GetCurrentContext()->Global(), sizeof(argv)/sizeof(*argv), argv);
        }
        work->success_callback.Reset();
        work->error_callback.Reset();
        delete work;
    }

    void DoExistsAsync(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        ExistsWork * work = new ExistsWork();
        work->request.data = work;

        work->file_root = std::string(*v8::String::Utf8Value(args[0]->ToString()));
        work->file_path = std::string(*v8::String::Utf8Value(args[1]->ToString()));
        work->success_callback.Reset(isolate, Local<Function>::Cast(args[2]));
        work->error_callback.Reset(isolate, Local<Function>::Cast(args[3]));

        uv_queue_work(uv_default_loop(), &work->request, ExistsAsync, ExistsAsyncComplete);

        args.GetReturnValue().Set(Undefined(isolate));
    }

    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "read_file", DoReadFileAsync);
        NODE_SET_METHOD(exports, "write_file", DoWriteFileAsync);
        NODE_SET_METHOD(exports, "copy_file_to_storage", DoCopyFileToAsync);
        NODE_SET_METHOD(exports, "copy_file_from_storage", DoCopyFileFromAsync);
        NODE_SET_METHOD(exports, "link_file", DoLinkAsync);
        NODE_SET_METHOD(exports, "delete_file", DoDeleteFileAsync);
        NODE_SET_METHOD(exports, "file_exists", DoExistsAsync);
    }
    NODE_MODULE(file_store_internal, init)
}
