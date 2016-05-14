'use strict';

var FileDB = require('../file-store.js');

var test = require('ava');
var fs = require('fs');
var temp = require('temp');
var path = require('path');

temp.track();

test.cb('FileHandle.read, FileHandle.write', function(t) {
  var dirPath = temp.mkdirSync('shuriken-fs-test-');
  var filePath = path.join('my-test-folder', 'my-file.txt');

  var actualData = fs.readFileSync(path.join(__dirname, 'itworks.txt'))
      .toString('utf-8');

  var fileDB = new FileDB(dirPath);
  var fileHandle = fileDB.get('shuriken://' + filePath);

  fileHandle.write(actualData, function(error) {
    t.falsy(error);
    fileHandle.read(function(error, data) {
      t.falsy(error);
      t.is(data, actualData);
      t.end();
    });
  });
});

test('FileHandle.readSync, FileHandle.writeSync', function(t) {
  var dirPath = temp.mkdirSync('shuriken-fs-test-');
  var filePath = path.join('my-test-folder', 'my-file.txt');

  var actualData = fs.readFileSync(path.join(__dirname, 'itworks.txt'))
      .toString('utf-8');

  var fileDB = new FileDB(dirPath);
  var fileHandle = fileDB.get('shuriken://' + filePath);

  fileHandle.writeSync(actualData);

  var data = fileHandle.readSync();
  t.is(data, actualData);
});

test.cb('Check if file does not exist', function(t) {
  var dirPath = temp.mkdirSync('shuriken-fs-test-');
  var filePath = path.join('my-test-folder', 'non-existing-file.txt');

  var fileDB = new FileDB(dirPath);
  fileDB.get('shuriken://' + filePath).exists(function(error, outcome) {
    t.falsy(error);
    t.is(outcome, false);
    t.end();
  });
});

test('Check if file does not exist (sync version)', function(t) {
  var dirPath = temp.mkdirSync('shuriken-fs-test-');
  var filePath = path.join('my-test-folder', 'non-existing-file.txt');

  var fileDB = new FileDB(dirPath);
  t.is(fileDB.get('shuriken://' + filePath).existsSync(), false);
});
