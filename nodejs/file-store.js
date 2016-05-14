'use strict';

var fsInternal = require('../build/Release/file_store_internal');
var should = require('should/as-function');
var deasync = require('deasync');

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
    var self = this;

    /* jshint camelcase: false */
    fsInternal.file_exists(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  this.existsSync = function() {
    var outcome = deasync(this.exists.bind(this))();
    return outcome;
  };

  // callback(error, data)
  this.read = function(callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.read_file(
        self.fileRoot,
        self.filePath,
        getReturnCallback(callback));
    return self;
  };

  this.readSync = function() {
    var data = deasync(this.read.bind(this))();
    return data;
  };

  // callback(error)
  this.write = function(data, callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.write_file(
        self.fileRoot,
        self.filePath,
        data,
        getVoidCallback(callback));
    return self;
  };

  this.writeSync = function(data) {
    deasync(this.write.bind(this))(data);
  };

  // callback(error)
  this.createLink = function(destination, callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.link_file(
        self.fileRoot,
        self.filePath,
        destination,
        getVoidCallback(callback));
    return self;
  };

  this.createLinkSync = function(destination) {
    deasync(this.createLink.bind(this))(destination);
  };

  // callback(error)
  this.remove = function(callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.delete_file(
        self.fileRoot,
        self.filePath,
        getVoidCallback(callback));
    return self;
  };

  this.removeSync = function() {
    deasync(this.remove.bind(this))();
  };

  // callback(error)
  this.copyTo = function(destPath, callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.copy_file_from_storage(
        self.fileRoot,
        self.filePath,
        destPath,
        getVoidCallback(callback));
    return self;
  };

  this.copyToSync = function(destPath) {
    deasync(this.copyTo.bind(this))(destPath);
  };

  // callback(error)
  this.copyFrom = function(origPath, callback) {
    var self = this;

    /* jshint camelcase: false */
    fsInternal.copy_file_to_storage(
        self.fileRoot,
        self.filePath,
        origPath,
        getVoidCallback(callback));
    return self;
  };

  this.copyFromSync = function(origPath) {
    deasync(this.copyFrom.bind(this))(origPath);
  };
}

function FileDB(fileRoot) {
    this.fileRoot = fileRoot;

    this.get = function(relativePath) {
        should(relativePath).startWith('shuriken://');
        relativePath = relativePath.replace('shuriken://', '');
        return new FileHandle(this.fileRoot, relativePath);
    };
}

module.exports = FileDB;
