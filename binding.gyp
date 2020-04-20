{
  "targets": [
    {
      "target_name": "xmlvalidator",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "./src/validate.cpp",
        "./src/index.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "/usr/include/libxml2"
      ],
       "libraries": [
            "-lxml2"
          ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}