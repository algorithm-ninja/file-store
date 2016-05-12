{
  "targets": [
    {
      "target_name": "file_store_internal",
      "sources": [ "src/file_manager.cpp", "src/node_ext.cpp" ],
      "libraries": [ "-lssl" ],
      "cflags": [ "-O2", "-Wall", "-std=c++11", "-I<!(pwd)/headers", "-fexceptions" ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "xcode_settings": {
        "OTHER_CFLAGS": [ "-O2", "-Wall", "-std=c++11", "-I<!(pwd)/headers", "-fexceptions", "-stdlib=libc++", "-mmacosx-version-min=10.8" ],
        "OTHER_LDFLAGS": [ "-mmacosx-version-min=10.8" ]
      }
    }
  ]
}
