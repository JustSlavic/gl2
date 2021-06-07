# Config Builder utility

The main idea behind config builder is that I didn't want to access config via regular JSON interface, e.g.

```c++
auto& cfg = config::instance;

son window_ = cfg["window"];
if (!window_.is_object()) return;

son width_ = window_["width"];
if (!width_.is_integer()) return;

int width = width_.get_integer();
```

What I wanted instead is accessing config directly, like that:

```c++
auto& cfg = config::instance;

int width = cfg.window.width;
```

To do so, before compiling my project, one should compile and run this little program. It will generate two files:
`config.hpp` and `config.cpp`, containing `config` class and methods for filling it up with values from *son* object.

### Scheme

Config builder uses scheme of config file to generate c++ files, but also it saves `config.scheme.son` file in `.generated` directory to compare schemes with old scheme later. In this manner config builder checks, if you changed config only in its values or in types and structure too. It saves compilation time considerably!

### Usage

Assuming you run config builder before every compilation, you should include two files into your project:
`.generated/config.hpp` and `.generated/config.cpp`.
Then you can include header and don't forget to initialize config providing son file to it:


### Example

Assuming config file be like that:

```
name = "Doge"
window = {
    width = 800
    height = 600
}
```

Here's how you access the data:

```c++
#include <stdio.h>
#include <config.hpp>

int main() {
    bool initialized = config::initialize("config.son");
    if (not initialized) {
        printf("Could not initialize config file from \"config.son\"\n");
        return 1;
    }

    // use config by getting instance:
    auto& cfg = config::get_instance();

    int width = cfg.window.width;
    int height = cfg.window.height;

    printf("%s window size: (%dx%d)\n", cfg.name.c_str(), width, height);

    return 0;
}
```
