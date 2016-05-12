'use strict';

const fileStoreInternal = require('../build/Release/file-store-internal');

class FileHandle {
  constructor(fileRoot, filePath) {
    this.fileRoot = fileRoot;
    this.filePath = filePath;
  }

  // callback(error, exists)
  exists(callback) {
    /* jshint camelcase: false */
    fileStoreInternal.file_exists(
        this.fileRoot,
        this.filePath,
        found => callback(0, found),
        error => callback(error));
    return this;
  }

  // callback(error, data)
  read(callback) {
    /* jshint camelcase: false */
    fileStoreInternal.read_file(
        this.fileRoot,
        this.filePath,
        data => callback(0, data),
        error => callback(error));
    return this;
  }

  // callback(error)
  write(data, callback) {
    /* jshint camelcase: false */
    fileStoreInternal.write_file(
        this.fileRoot,
        this.filePath,
        data,
        () => callback(0),
        error => callback(error));
    return this;
  }

  // callback(error)
  createLink(destination, callback) {
    /* jshint camelcase: false */
    fileStoreInternal.link_file(
        this.fileRoot,
        this.filePath,
        destination,
        () => callback(0),
        error => callback(error));
    return this;
  }

  // callback(error)
  remove(callback) {
    /* jshint camelcase: false */
    fileStoreInternal.delete_file(
        this.fileRoot,
        this.filePath,
        () => callback(0),
        error => callback(error));
    return this;
  }

  // callback(error)
  copyTo(destPath, callback) {
    /* jshint camelcase: false */
    fileStoreInternal.copy_file_from_storage(
        this.fileRoot,
        this.filePath,
        destPath,
        () => callback(0),
        error => callback(error));
    return this;
  }

  // callback(error)
  copyFrom(origPath, callback) {
    /* jshint camelcase: false */
    fileStoreInternal.copy_file_to_storage(
        this.fileRoot,
        this.filePath,
        origPath,
        () => callback(0),
        error => callback(error));
    return this;
  }
}

module.exports = FileHandle;
