'use strict';

var fsInternal = require('../build/Release/file_store_internal');
var should = require('should/as-function');
var async = require('async');

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
    fsInternal.file_exists(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  // callback(error, data)
  this.read = function(callback) {
    /* jshint camelcase: false */
    fsInternal.read_file(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  this.readSync = function() {
    var self = this;
    var status = {};

    async.series([
      function() {
        self.read(function(error, data) {
          status.error = error;
          status.data = data;
        });
      }
    ]);

    return status;
  };

  // callback(error)
  this.write = function(data, callback) {
    /* jshint camelcase: false */
    fsInternal.write_file(
        self.fileRoot,
        self.filePath,
        data,
        getVoidCallback(callback));
    return self;
  };

  this.writeSync = function() {
    var self = this;
    var status = {};

    async.series([
      function() {
        self.write(function(error) {
          status.error = error;
        });
      }
    ]);

    return status;
  };

  // callback(error)
  this.createLink = function(destination, callback) {
    /* jshint camelcase: false */
    fsInternal.link_file(
        self.fileRoot,
        self.filePath,
        destination,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.remove = function(callback) {
    /* jshint camelcase: false */
    fsInternal.delete_file(
        self.fileRoot,
        self.filePath,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.copyTo = function(destPath, callback) {
    /* jshint camelcase: false */
    fsInternal.copy_file_from_storage(
        self.fileRoot,
        self.filePath,
        destPath,
        getVoidCallback(callback));
    return self;
  };

  // callback(error)
  this.copyFrom = function(origPath, callback) {
    /* jshint camelcase: false */
    fsInternal.copy_file_to_storage(
        self.fileRoot,
        self.filePath,
        origPath,
        getVoidCallback(callback));
    return self;
  };
}

function FileDB(fileRoot) {
    var self = this;

    self.fileRoot = fileRoot;

    self.get = function(relativePath) {
        should(relativePath).startWith('shuriken://');
        relativePath = relativePath.replace('shuriken://', '');
        return new FileHandle(self.fileRoot, relativePath);
    };
}

module.exports = FileDB;
