A tool for creating highspeed RapidJSON SAX reader and writer using a some plain old data (POD) struct.

## How to build

From this directory:
```
mkdir build
cd build
cmake ..
make
```

## Using the generator

```c++
// server_list.h

#pragma once

namespace volt
{
    namespace core
    {
        struct ServerInfo
        {
            std::string name_;
            std::string publicKey_;
            std::string address_;
            uint16_t port_;
        };

        struct ServerList
        {
            std::vector<ServerInfo> servers;
        };
    }
}
```

```
rapidjson-generator --from /work/structs/server_list.h --to /work/otput --struct ServerList
```
