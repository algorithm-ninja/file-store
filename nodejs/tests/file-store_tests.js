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

  var data = fs.readFileSync(path.join(__dirname, 'itworks.txt'));

  var fileDB = new FileDB(dirPath);
  var fileHandle = fileDB.get('shuriken://' + filePath);

  fileHandle.write(data, function(error) {
    t.falsy(error);
    fileHandle.read(function(error, data) {
      t.falsy(error);
      t.is(data, 'It works!');
      t.end();
    });
  });
});

test('FileHandle.readSync, FileHandle.writeSync', function(t) {
  var dirPath = temp.mkdirSync('shuriken-fs-test-');
  var filePath = path.join('my-test-folder', 'my-file.txt');

  var data = fs.readFileSync(path.join(__dirname, 'itworks.txt'));

  var fileDB = new FileDB(dirPath);
  var fileHandle = fileDB.get('shuriken://' + filePath);

  var status = fileHandle.writeSync(data);
  t.falsy(status.error);

  status = fileHandle.readSync();
  t.falsy(status.error);
  t.is(status.data, 'It works!');
});
