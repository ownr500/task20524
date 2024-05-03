{
  "targets": [
    {
      "target_name": "checkuser",
      "sources": [ "checkuser.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ]
    }
  ]
}
