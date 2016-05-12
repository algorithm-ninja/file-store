'use strict';

const file_store_internal = require("./build/Release/file-store-internal");

class FileHandle {
    constructor(file_root, file_path) {
        this.file_root = file_root
        this.file_path = file_path
    }
    // callback(error, exists)
    exists(callback) {
        file_store_internal.file_exists(this.file_root, this.file_path, found => callback(0, found), error => callback(error));
        return this;
    }
    // callback(error, data)
    read(callback) {
        file_store_internal.read_file(this.file_root, this.file_path, data => callback(0, data, error => callback(error)))
        return this;
    }
    // callback(error)
    write(data, callback) {
        file_store_internal.write_file(this.file_root, this.file_path, data, () => callback(0), error => callback(error))
        return this;
    }
    // callback(error)
    createLink(destination, callback) {
        file_store_internal.link_file(this.file_root, this.file_path, destination, () => callback(0), error => callback(error))
        return this;
    }
    // callback(error)
    remove(callback) {
        file_store_internal.delete_file(this.file_root, this.file_path, () => callback(0), error => callback(error))
        return this;
    }
    // callback(error)
    copyTo(dest_path, callback) {
        file_store_internal.copy_file_from_storage(this.file_root, this.file_path, dest_path, () => callback(0), error => callback(error))
        return this;
    }
    // callback(error)
    copyFrom(orig_path, callback) {
        file_store_internal.copy_file_to_storage(this.file_root, this.file_path, orig_path, () => callback(0), error => callback(error))
        return this;
    }
}


module.exports = {
    newFileHandle: function(file_root, file_path) {
        return new FileHandle(file_root, file_path);
    }
};
