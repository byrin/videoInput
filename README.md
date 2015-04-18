This fork enriches the [videoInput](https://github.com/ofTheo/videoInput) library by Theodore Watson et al. with a C API. This allows videoInput to be used in C projects. A build script to compile a DLL with MinGW is also provided.

The source of the C API is located in `videoInputSrcAndDemos\libs\videoInput-c-api`. To build the DLL navigate to `videoInputSrcAndDemos\mingw-compileAsDll` and run build.bat. The library will be created in `compiledLib\mingw`.

Just like videoInput, this code is not tied to any particular license so it's used in all scenarios without any restriction.
