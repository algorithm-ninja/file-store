{
  "targets": [
    {
      "target_name": "file-store-internal",
      "sources": [ "src/file_manager.cpp", "src/node_ext.cpp" ],
      "cflags": [ "-O2", "-Wall", "-std=c++11", "-I<!(pwd)/headers", "-fexceptions" ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "libraries": [ "-lssl" ]
    }
  ]
}
