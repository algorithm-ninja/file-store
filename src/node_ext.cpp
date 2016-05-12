#include <nan.h>
#include <iostream>
#include <unistd.h>
#include "file_manager.hpp"
// TODO: we could probably templatize this.

// std::vector<unsigned char> read_file(const std::string& file_root, const std::string& path);
class ReadFileWorker: public Nan::AsyncWorker {
    std::vector<unsigned char> data;
    std::string file_root;
    std::string path;
    int error;
public:
    ReadFileWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path)
        : AsyncWorker(callback), file_root(file_root), path(path), error(0) {}
    void Execute() {
        try {
            data = read_file(file_root, path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        if (error) {
            v8::Local<v8::Value> argv[] = {error_code};
            callback->Call(1, argv);
        } else {
            v8::Local<v8::Value> return_value = Nan::New<v8::String>(
                (char*)data.data(),
                data.size()
            ).ToLocalChecked();
            v8::Local<v8::Value> argv[] = {error_code, return_value};
            callback->Call(2, argv);
       }
    }
};

NAN_METHOD(ReadFile) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new ReadFileWorker(callback, file_root, path));
}

// void write_file(const std::string& file_root, const std::string& path, const std::vector<unsigned char>& data);
class WriteFileWorker: public Nan::AsyncWorker {
    std::string file_root;
    std::string path;
    std::vector<unsigned char> data;
    int error;
public:
    WriteFileWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path, const std::string& data)
        : AsyncWorker(callback), file_root(file_root), path(path), data(data.begin(), data.end()), error(0) {}
    void Execute() {
        try {
            write_file(file_root, path, data);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        v8::Local<v8::Value> argv[] = {error_code};
        callback->Call(1, argv);
    }
};

NAN_METHOD(WriteFile) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Utf8String js_data(info[2]);
    std::string data(*js_data, js_data.length());
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new WriteFileWorker(callback, file_root, path, data));
}

// void copy_file_to_storage(const std::string& file_root, const std::string& path, const std::string& orig_file);
class CopyFileToWorker: public Nan::AsyncWorker {
    std::string file_root;
    std::string path;
    std::string orig_path;
    int error;
public:
    CopyFileToWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path, const std::string& orig_path)
        : AsyncWorker(callback), file_root(file_root), path(path), orig_path(orig_path), error(0) {}
    void Execute() {
        try {
            copy_file_to_storage(file_root, path, orig_path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        v8::Local<v8::Value> argv[] = {error_code};
        callback->Call(1, argv);
    }
};

NAN_METHOD(CopyFileTo) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Utf8String js_orig_path(info[2]);
    std::string orig_path(*js_orig_path, js_orig_path.length());
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new CopyFileToWorker(callback, file_root, path, orig_path));
}

// void copy_file_from_storage(const std::string& file_root, const std::string& path, const std::string& dest_file);
class CopyFileFromWorker: public Nan::AsyncWorker {
    std::string file_root;
    std::string path;
    std::string dest_path;
    int error;
public:
    CopyFileFromWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path, const std::string& dest_path)
        : AsyncWorker(callback), file_root(file_root), path(path), dest_path(dest_path), error(0) {}
    void Execute() {
        try {
            copy_file_from_storage(file_root, path, dest_path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        v8::Local<v8::Value> argv[] = {error_code};
        callback->Call(1, argv);
    }
};

NAN_METHOD(CopyFileFrom) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Utf8String js_dest_path(info[2]);
    std::string dest_path(*js_dest_path, js_dest_path.length());
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new CopyFileFromWorker(callback, file_root, path, dest_path));
}

// void delete_file(const std::string& file_root, const std::string& path);
class DeleteFileWorker: public Nan::AsyncWorker {
    std::string file_root;
    std::string path;
    int error;
public:
    DeleteFileWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path)
        : AsyncWorker(callback), file_root(file_root), path(path), error(0) {}
    void Execute() {
        try {
            delete_file(file_root, path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        v8::Local<v8::Value> argv[] = {error_code};
        callback->Call(1, argv);
    }
};

NAN_METHOD(DeleteFile) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new DeleteFileWorker(callback, file_root, path));
}

// bool exists(const std::string& file_root, const std::string& path);
class FileExistsWorker: public Nan::AsyncWorker {
    bool result;
    std::string file_root;
    std::string path;
    int error;
public:
    FileExistsWorker(Nan::Callback* callback, const std::string& file_root, const std::string& path)
        : AsyncWorker(callback), file_root(file_root), path(path), error(0) {}
    void Execute() {
        try {
            result = exists(file_root, path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        if (error) {
            v8::Local<v8::Value> argv[] = {error_code};
            callback->Call(1, argv);
        } else {
            v8::Local<v8::Value> return_value = Nan::New<v8::Boolean>(result);
            v8::Local<v8::Value> argv[] = {error_code, return_value};
            callback->Call(2, argv);
       }
    }
};

NAN_METHOD(FileExists) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_path(info[1]);
    std::string path(*js_path, js_path.length());
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new FileExistsWorker(callback, file_root, path));
}

// void link(const std::string& file_root, const std::string& orig_path, const std::string& dest_path);
class LinkFileWorker: public Nan::AsyncWorker {
    std::string file_root;
    std::string orig_path;
    std::string dest_path;
    int error;
public:
    LinkFileWorker(Nan::Callback* callback, const std::string& file_root, const std::string& orig_path, const std::string& dest_path)
        : AsyncWorker(callback), file_root(file_root), orig_path(orig_path), dest_path(dest_path), error(0) {}
    void Execute() {
        try {
            link(file_root, orig_path, dest_path);
        } catch (OSError& e) {
            error = e.get_err();
        }
    }
    void HandleOKCallback () {
        Nan::HandleScope scope;
        v8::Local<v8::Value> error_code = Nan::New<v8::Integer>(error);
        v8::Local<v8::Value> argv[] = {error_code};
        callback->Call(1, argv);
    }
};

NAN_METHOD(LinkFile) {
    Nan::Utf8String js_file_root(info[0]);
    std::string file_root(*js_file_root, js_file_root.length());
    Nan::Utf8String js_orig_path(info[1]);
    std::string orig_path(*js_orig_path, js_orig_path.length());
    Nan::Utf8String js_dest_path(info[2]);
    std::string dest_path(*js_dest_path, js_dest_path.length());
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new LinkFileWorker(callback, file_root, orig_path, dest_path));
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New<v8::String>("read_file").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(ReadFile)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("write_file").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(WriteFile)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("copy_file_to_storage").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(CopyFileTo)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("copy_file_from_storage").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(CopyFileFrom)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("delete_file").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(DeleteFile)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("file_exists").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(FileExists)).ToLocalChecked());
    Nan::Set(target, Nan::New<v8::String>("link_file").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(LinkFile)).ToLocalChecked());
}

NODE_MODULE(file_store_internal, Init)
