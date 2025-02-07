# SmartBuffer: Fast and Flexible Data Buffer

**SmartBuffer** is a custom library I built for managing binary data efficiently. It’s simple, fast, and works in any C++ project. The main idea is to make it easy to read, write, and manipulate raw data without worrying about memory leaks or performance hits.

---

## Why SmartBuffer?

1. **Performance**: Optimized for fast reads/writes with minimal overhead.
2. **No Memory Leaks**: Fully manages its memory using `std::vector`.
3. **Universal**: Works with any trivially copyable type (like `int`, `float`, etc.) and strings.
4. **Reusable**: You can use it in any C++ project. It’s modular and lightweight.

---

## How It Works

### Core Features:
- **Dynamic Resizing**: The buffer grows automatically when needed.
- **Serialization**: Supports trivially copyable types and strings.
- **Operator Overloads**: Use `<<` and `>>` for easy read/write.
- **Buffer Reset**: Clear all data or just reset the read pointer.

### Structure
The buffer is built around:
- `std::vector<uint8_t>`: Handles the raw data.
- `readOffset` and `writeOffset`: Keep track of where to read and write.
- Templates for `read` and `write`: Allow type-safe operations.

---

## Key Methods

### Initialization
```cpp
SmartBuffer(size_t initialCapacity = 8);
```
- Reserves initial capacity (default: 8 bytes).
- The buffer expands as needed.

### Writing Data
```cpp
template <typename T>
void write(const T& value);
```
- Supports any trivially copyable type.
- For strings, writes the length first, then the content.

### Reading Data
```cpp
template <typename T>
T read();
```
- Reads data back in the same order it was written.
- Throws an exception if you try to read beyond the buffer.

### Injecting Raw Data
```cpp
void inject(const uint8_t* rawData, size_t size);
```
- Adds raw binary data directly into the buffer.

### Resetting
```cpp
void reset(); // Clears all data
void resetRead(); // Resets the read pointer
```
- `reset()`: Clears the whole buffer.
- `resetRead()`: Resets the read pointer to the start.

### Getters
```cpp
size_t getSize() const;      // Total size of written data
const uint8_t* getBuffer() const; // Pointer to raw buffer data
```
- Use these to access buffer stats or get the raw data.

---

## Why `.inl`?

SmartBuffer’s implementation uses `.inl` for templates. Templates must be defined in the header (or included inline) so the compiler can generate code for specific types during compilation. This keeps the header clean and avoids duplication.

---

## Example Usage

### Writing and Reading
```cpp
SmartBuffer buffer;

// Writing data
buffer << 42 << 3.14f << std::string("Hello");

// Reading data
int intValue;
float floatValue;
std::string stringValue;
buffer >> intValue >> floatValue >> stringValue;

// Output: 42, 3.14, "Hello"
```

### Injecting Raw Data
```cpp
uint8_t rawData[] = {0x01, 0x02, 0x03};
buffer.inject(rawData, sizeof(rawData));
```

### Resetting
```cpp
buffer.reset();      // Clear everything
buffer.resetRead();  // Start reading from the beginning
```

---

## Technical Highlights

1. **Memory Management**:
   - Uses `std::vector` for automatic memory handling.
   - Ensures no leaks even with dynamic resizing.

2. **Trivial Type Optimization**:
   - Writes and reads trivially copyable types directly with `std::memcpy`.

3. **String Support**:
   - Writes the length first as a `uint32_t`, then the content.
   - Reads back the length to reconstruct the string.

4. **Error Handling**:
   - Throws `std::runtime_error` for out-of-bounds reads.

---

## Performance

- **Speed**: Minimal overhead, only resizes when necessary.
- **Safety**: No manual memory management.
- **Efficiency**: Avoids unnecessary copies with `std::vector`.

---

## Where to Use It?

- Networking (serialize/deserialize packets).
- Game development (handling binary assets or messages).
- General-purpose C++ projects needing binary buffers.
