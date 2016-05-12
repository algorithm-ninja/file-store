'use strict';

var fs_internal = require('../build/Release/file_store_internal');

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
        function(found) { callback(null, found); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error, data)
  this.read = function(callback) {
    /* jshint camelcase: false */
    fs_internal.read_file(
        self.fileRoot,
        self.filePath,
        function(data) { callback(null, data); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error)
  this.write = function(data, callback) {
    /* jshint camelcase: false */
    fs_internal.write_file(
        self.fileRoot,
        self.filePath,
        data,
        function() { callback(null); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error)
  this.createLink = function(destination, callback) {
    /* jshint camelcase: false */
    fs_internal.link_file(
        self.fileRoot,
        self.filePath,
        destination,
        function() { callback(null); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error)
  this.remove = function(callback) {
    /* jshint camelcase: false */
    fs_internal.delete_file(
        self.fileRoot,
        self.filePath,
        function() { callback(null); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error)
  this.copyTo = function(destPath, callback) {
    /* jshint camelcase: false */
    fs_internal.copy_file_from_storage(
        self.fileRoot,
        self.filePath,
        destPath,
        function() { callback(null); },
        function(error) { callback(error); });
    return self;
  };

  // callback(error)
  this.copyFrom = function(origPath, callback) {
    /* jshint camelcase: false */
    fs_internal.copy_file_to_storage(
        self.fileRoot,
        self.filePath,
        origPath,
        function() { callback(null); },
        function(error) { callback(error); });
    return self;
  };
}

module.exports = FileHandle;
