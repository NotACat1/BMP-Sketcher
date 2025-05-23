# BMP-Sketcher

`BMP-Sketcher` is a utility for processing BMP images 🖼️ with capabilities for drawing using various strategies (single-threaded and multi-threaded), converting images to grayscale, and displaying results in the console.

---

## 📦 Class: `BMPProcessor`

### 🔧 Constructor

```cpp
BMPProcessor(const Config& config, std::unique_ptr<IDrawStrategy> strategy)
```

**Parameters:**

- `config` — Configuration obtained via command line.
- `strategy` — Pointer to the selected drawing strategy (`IDrawStrategy`).

Initializes the processor and sets the color and thickness for the drawing strategy.

---

### 🔁 Method: `bool process()`

Processes the image:

1. Loads the BMP file.
2. Performs drawing using the selected strategy (if specified).
3. Converts the image to grayscale.
4. Saves the result to the output file.

**Returns:** `true` if successful; `false` on error.

---

### 🖥️ Method: `void display() const`

Displays the image in the console using two symbols:

- First symbol (`on_char`) — for bright pixels.
- Second symbol (`off_char`) — for dark pixels.

Brightness is calculated using the formula:

```
brightness = 0.299 * R + 0.587 * G + 0.114 * B
```

---

### ⚙️ Nested Class: `Config`

#### 🧩 Method

```cpp
static Config parse(int argc, char* argv[])
```

Parses command-line arguments and returns the configuration.

**Supported arguments:**

| Key                     | Description                                 |
| ----------------------- | ------------------------------------------- |
| `-i, --input <file>`    | Path to input BMP file (**required**)       |
| `-o, --output <file>`   | Path to output file (default: `output.bmp`) |
| `-t, --thickness <n>`   | Line thickness (≥ 1, default: `1`)          |
| `-c, --color R,G,B[,A]` | Color in RGBA format (default: `0,0,0,255`) |
| `-d, --display XY`      | Symbols for display (default: `"# "`)       |
| `-s, --strategy <name>` | Strategy: `none`, `openmp`, `thread`        |
| `-h, --help`            | Show help and exit                          |

Throws an exception for unknown strategies.

---

#### 🆘 Method

```cpp
static void printHelp(const std::string& program_name)
```

Displays program usage help in the console.

**Example usage:**

```bash
./bmp_processor -i input.bmp -o out.bmp -t 2 -c 255,0,0 -s openmp
```

---

## 📌 Usage Example

```cpp
int main(int argc, char* argv[]) {
    try {
        auto config = BMPProcessor::Config::parse(argc, argv);
        auto strategy = DrawStrategyFactory::create(config.strategy_type);
        BMPProcessor processor(config, std::move(strategy));

        if (processor.process()) {
            processor.display(); // Console output
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```

---

## 💡 Features

✅ Custom color and thickness support  
✅ Console preview  
✅ Flexible drawing strategy selection  
✅ Error handling  
✅ Transparency (Alpha) support

---

## 🛠 Scripts & Project Automation

To streamline the development workflow and ensure consistency across environments, `BMP_Sketcher` includes a comprehensive set of utility scripts located in the [`scripts/`](scripts/) directory. These scripts handle building, testing, running, cleaning, and code coverage — as well as a top-level dependency installation script to get you started instantly.

Whether you're a new contributor or maintaining the project, these tools will save you time and reduce friction.

---

### 📦 1. `install_dependencies.sh` — Install All Requirements (Ubuntu/Debian)

```bash
./install_dependencies.sh
```

> **Purpose:** Installs all required libraries and tools, including CMake, OpenMP, OpenCL, GoogleTest, and Google Benchmark.

This is your one-stop setup script. It updates your package index and installs everything the project needs. It even builds and installs GoogleTest from source, since it's shipped as headers only. Run this script **once** before building the project.

💡 _Supports Ubuntu/Debian-based systems._

---

## ⚙️ Utility Scripts (Inside `scripts/`)

All scripts in the `scripts/` folder are meant to be run from the root of the repository and are executable (`chmod +x` by default).

---

### 🔧 2. `scripts/build.sh` — Build the Project

```bash
./scripts/build.sh [Debug|Release]
```

> **Default:** `Debug`

This script configures and compiles the project using CMake. It automatically creates a `build/` directory and compiles with the specified build type using all available CPU cores.

✅ Supports switching between build types for debugging or production use.

---

### 🧪 3. `scripts/test.sh` — Run Unit Tests

```bash
./scripts/test.sh
```

> **Purpose:** Executes all unit tests using CTest and then runs benchmarks if they are available.

- Runs unit tests with verbose output on failure.
- Automatically detects and runs BMP_Sketcher_benchmarks if it was built (BUILD_BENCHMARKS=ON).
- Benchmarks are useful for measuring and comparing performance of algorithms or implementations.

📌 If the benchmark executable is missing, it shows a helpful message about enabling it during build.

---

### 🧹 4. `scripts/clean.sh` — Clean the Build Environment

```bash
./scripts/clean.sh
```

> **Purpose:** Completely removes the `build/` directory.

Run this when you want to start fresh or switch between toolchains/build systems. Clears all compiled artifacts.

---

## 📚 Summary Table

| Script                    | Description                                 | Usage Example                |
| ------------------------- | ------------------------------------------- | ---------------------------- |
| `install_dependencies.sh` | Installs all required dependencies (Ubuntu) | `./install_dependencies.sh`  |
| `scripts/build.sh`        | Builds the project (Debug/Release)          | `./scripts/build.sh Release` |
| `scripts/test.sh`         | Runs unit tests and benchmarks (if enabled) | `./scripts/test.sh`          |
| `scripts/clean.sh`        | Deletes the build directory                 | `./scripts/clean.sh`         |

---

## 🧠 Best Practices

- Run `install_dependencies.sh` once after cloning the project.
- Use `build.sh` during development.
- Use `run.sh` to test input/output behavior.
- Use `clean.sh` if you encounter build issues or switch toolchains.

---

Here's the English version of your BMP file documentation:

## 🖼️ Working with BMP Files

`BMPFile` is a C++ class designed for loading, saving, and basic processing of BMP (Bitmap) files. It supports both 24-bit (BGR) and 32-bit (BGRA) formats and provides a low-level API for image manipulation.

---

### 🚀 Features

| Functionality         | Description                                                  | Method                      |
| --------------------- | ------------------------------------------------------------ | --------------------------- |
| 🔄 Loading & Saving   | Supports reading/writing uncompressed BMP files (24/32 bit)  | `load()` / `save()`         |
| 🧱 Pixel Manipulation | Get and set individual pixels                                | `getPixel()` / `setPixel()` |
| 🔃 Vertical Flip      | Fast vertical image flipping                                 | `flipVertically()`          |
| ⚫⚪ B/W Conversion   | Converts color images to black and white (with thresholding) | `convertToBlackAndWhite()`  |
| 🆕 Image Creation     | Creates a new blank image with specified size and format     | `create()`                  |

---

### 📦 Supported Formats

- ✅ 24-bit BMP (BGR)
- ✅ 32-bit BMP (BGRA)
- ❌ Not supported: RLE compression, palettes, other non-standard BMP variants

---

### 📚 Quick Usage Example

```cpp
#include "BMPFile.hpp"
#include <iostream>

int main() {
    BMPFile image;

    if (!image.load("input.bmp")) {
        std::cerr << "Error loading BMP file!\n";
        return 1;
    }

    image.convertToBlackAndWhite();   // Convert to B/W
    image.flipVertically();           // Flip vertically

    if (!image.save("output.bmp")) {
        std::cerr << "Error saving BMP file!\n";
        return 1;
    }

    std::cout << "Processing completed successfully!\n";
    return 0;
}
```

---

### 🛠 Class Structure

| Component   | Purpose                              |
| ----------- | ------------------------------------ |
| `BMPHeader` | BMP file header (type, size, offset) |
| `DIBHeader` | Size, depth, compression information |
| `Pixel`     | Single pixel representation (RGB[A]) |

---

## 🖌️ Draw Strategy Factory

Welcome to **Draw Strategy Factory** — a module for flexible and multithreaded drawing on BMP images! Choose your strategy 🧠, set the color 🎨 and thickness ✍️ — and go ahead, create masterpieces!

---

### 🔍 What is this?

This project implements the **Strategy Pattern** via a factory, enabling drawing on BMP files using different approaches:

- 🧵 **NONE** — Standard single-threaded implementation
- 💥 **OPENMP** — Multithreading via OpenMP
- ⚙️ **THREAD** — Classic threads

---

### 🧱 Architecture

#### 🎭 `IDrawStrategy` — Your Artist's Contract

An interface that each strategy must implement:

| Method               | Description                 |
| -------------------- | --------------------------- |
| `draw(BMPFile&)`     | Draw something on the image |
| `getName()`          | Get the strategy name       |
| `setColor(Pixel)`    | Set the color 🎨            |
| `getColor()`         | Get the current color       |
| `setThickness(uint)` | Set line thickness          |
| `getThickness()`     | Check current thickness     |

---
