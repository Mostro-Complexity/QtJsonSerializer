# QtJsonSerializer
A library to perform generic seralization and deserialization of QObjects from and to JSON and CBOR.

With this small library, you are able to serialize any C++ datatype to JSON or CBOR and back. This is done with help of Qt's meta system.

> The library was refered from [here](https://github.com/Skycoder42/QtJsonSerializer), and add cmake support features. Have a look at the usage of original repo to learn how to use this serialization library. Don't be afraid, as for most existing projects, only class names will have changed.

## Download/Configuration
There is a simple way to configurate this Qt module, just create cmake script add include it in CMakeLists.txt of your project:

```cmake
include(FetchContent)

FetchContent_Declare(QtJsonSerializer
    GIT_REPOSITORY https://github.com/Mostro-Complexity/QtJsonSerializer
    GIT_TAG master
)

FetchContent_MakeAvailable(QtJsonSerializer)

```

And link it if your project module need to use:

```cmake
target_link_libraries(${PROJECT_NAME} 
    PRIVATE QtJsonSerializer::QtJsonSerializer
)
```

