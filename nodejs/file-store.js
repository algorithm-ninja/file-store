'use strict';

var fs_internal = require('../build/Release/file_store_internal');

function getReturnCallback(callback) {
  return function(err, ret) {
    callback(err ? new Error(err) : null, ret);
  };
}

function getVoidCallback(callback) {
  return function(err) {
    callback(err ? new Error(err) : null);
  };
}

function FileHandle(fileRoot, filePath) {
  var self = this;

  self.fileRoot = fileRoot;
  self.filePath = filePath;

  // callback(error, exists)
  this.exists = function(callback) {
    /* jshint camelcase: false */
    fs_internal.file_exists(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  // callback(error, data)
  this.read = function(callback) {
    /* jshint camelcase: false */
    fs_internal.read_file(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  // callback(error)
  this.write = function(data, callback) {
    /* jshint camelcase: false */
    fs_internal.write_file(
        self.fileRoot,
        self.filePath,
        data,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.createLink = function(destination, callback) {
    /* jshint camelcase: false */
    fs_internal.link_file(
        self.fileRoot,
        self.filePath,
        destination,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.remove = function(callback) {
    /* jshint camelcase: false */
    fs_internal.delete_file(
        self.fileRoot,
        self.filePath,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.copyTo = function(destPath, callback) {
    /* jshint camelcase: false */
    fs_internal.copy_file_from_storage(
        self.fileRoot,
        self.filePath,
        destPath,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.copyFrom = function(origPath, callback) {
    /* jshint camelcase: false */
    fs_internal.copy_file_to_storage(
        self.fileRoot,
        self.filePath,
        origPath,
        getVoidCallback(callback));
    return self;
  };
}

module.exports = FileHandle;
