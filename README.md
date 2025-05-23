🎨 BMP-Sketcher: The Ultimate Image Processing Playground 🖌️

![Banner](https://placehold.co/1200x400/2d2d2d/FFF?text=BMP-Sketcher\nPaint%20Your%20Digital%20Dreams&font=montserrat)

**Transform ordinary images into extraordinary art** with BMP-Sketcher - where C++ meets creativity! A multi-threaded, strategy-powered image processing toolkit that turns your BMP files into canvases for digital expression.

---

### 🧩 Overview

**BMP-Sketcher** is a C++-based image processing toolkit focused on BMP files (24/32-bit).  
It combines console-based visual output, multithreaded processing strategies, and a modular, extensible architecture.

---

### 🌟 Features

- ✅ BMP file loading/saving (24-bit and 32-bit)
- ✅ Grayscale and black-and-white conversion
- ✅ Customizable drawing color and thickness
- ✅ Console image preview
- ✅ Multithreading via OpenMP or std::thread
- ✅ Strategy Pattern for pluggable drawing logic
- ✅ Ready-to-use automation scripts
- ✅ Alpha channel support (transparency)

---

## ⚡ Quick Start - 30 Seconds to Art

1. **Clone and conquer**:

   ```bash
   git clone https://github.com/NotACat1/BMP-Sketcher.git
   cd BMP-Sketcher
   ```

2. **Build your creative engine**:

   ```bash
   ./scripts/build.sh Release
   ```

3. **Create your first masterpiece**:

   ```bash
   ./bin/BMP-Sketcher -i input.bmp -o art.bmp -s openmp -c 255,100,0 -t 3
   ```

---

### 🧠 BMPProcessor API

#### 🔨 Constructor

```cpp
BMPProcessor(const Config& config, std::unique_ptr<IDrawStrategy> strategy)
```

Initializes the processor with configuration and a drawing strategy.

---

#### 🔁 `bool process()`

Processes the image:

1. Loads BMP file
2. Applies the drawing strategy
3. Converts to grayscale
4. Saves the result

Returns `true` on success.

---

#### 🖥️ `void display() const`

Displays the image in the console using `on_char` and `off_char`, based on brightness:

```
brightness = 0.299 * R + 0.587 * G + 0.114 * B
```

---

#### ⚙️ `struct Config`

```cpp
static Config parse(int argc, char* argv[])
```

Parses command-line arguments. Supported flags:

| Option                  | Description                             |
| ----------------------- | --------------------------------------- |
| `-i, --input <file>`    | Input BMP file (**required**)           |
| `-o, --output <file>`   | Output BMP file (default: `output.bmp`) |
| `-t, --thickness <n>`   | Line thickness (default: 1)             |
| `-c, --color R,G,B[,A]` | RGBA color (default: `0,0,0,255`)       |
| `-d, --display XY`      | Display symbols (default: `"# "`)       |
| `-s, --strategy <name>` | Strategy: `none`, `openmp`, `thread`    |
| `-h, --help`            | Show usage help                         |

Throws on unknown strategies.

---

#### ❓ `void printHelp(const std::string& program_name)`

Prints help message with usage examples.

---

### 🧵 Draw Strategies

Modular drawing implementations using the Strategy Pattern:

| Strategy | Description                   |
| -------- | ----------------------------- |
| `none`   | Single-threaded baseline      |
| `openmp` | OpenMP parallelized drawing   |
| `thread` | std::thread-based parallelism |

#### Interface: `IDrawStrategy`

```cpp
void draw(BMPFile&);
std::string getName() const;
void setColor(Pixel);
Pixel getColor() const;
void setThickness(uint32_t);
uint32_t getThickness() const;
```

Create new strategies easily by implementing this interface.

---

### 🖼️ BMPFile API

C++ class for loading, manipulating, and saving BMP files.

#### Core Methods

| Method                     | Description                           |
| -------------------------- | ------------------------------------- |
| `load(const std::string&)` | Load a 24/32-bit BMP                  |
| `save(const std::string&)` | Save image as BMP                     |
| `getPixel(x, y)`           | Access individual pixel               |
| `setPixel(x, y, pixel)`    | Modify pixel color                    |
| `flipVertically()`         | Flip image upside-down                |
| `convertToBlackAndWhite()` | Grayscale + threshold to binary image |
| `create(width, height)`    | Create blank image                    |

#### Supported Formats

- ✅ 24-bit (BGR)
- ✅ 32-bit (BGRA)
- ❌ RLE compression / palettes unsupported

---

### ⚙️ Project Scripts

All scripts live in `scripts/` and are `chmod +x`-ready.

| Script                    | Description                                 |
| ------------------------- | ------------------------------------------- |
| `install_dependencies.sh` | Installs build dependencies (Ubuntu/Debian) |
| `build.sh`                | Builds the project using CMake              |
| `clean.sh`                | Removes `build/` directory                  |

#### 🔧 `install_dependencies.sh`

```bash
./install_dependencies.sh
```

Installs: CMake, OpenMP.

---

#### 🧪 `build.sh`

```bash
./scripts/build.sh [Debug|Release]
```

Configures and builds the project.

---

#### 🧼 `clean.sh`

```bash
./scripts/clean.sh
```

Deletes build artifacts.

---

### 🚀 Usage Example

```cpp
int main(int argc, char* argv[]) {
    try {
        auto config = BMPProcessor::Config::parse(argc, argv);
        auto strategy = DrawStrategyFactory::create(config.strategy_type);
        BMPProcessor processor(config, std::move(strategy));

        if (processor.process()) {
            processor.display();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```

---

### ✅ Best Practices

- Run `install_dependencies.sh` once after cloning
- Use `build.sh` during development
- Run `clean.sh` before switching toolchains
- Combine strategies and config for rapid prototyping

---

### 📜 License

MIT License — see [LICENSE](LICENSE) for details.

---

### 🎯 Conclusion

BMP-Sketcher showcases clean architecture, extensibility, and practical use of C++ design patterns.  
It’s perfect for learning graphics processing, multithreading, and modern C++ patterns.

**Pull requests welcome!** 🎨👾
