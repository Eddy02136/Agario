
# Comparison of Graphics Libraries for R-Type

This document compares three major graphics libraries: **Raylib**, **SFML**, and **SDL**, based on their usability, performance, features, and documentation. It also explains why **SFML** was chosen for the R-Type project.

---

## Library Comparison

| Criteria          | Raylib                                   | SFML                              | SDL                                   |
|-------------------|-----------------------------------------|-----------------------------------|---------------------------------------|
| **Usability**     | Intuitive                               | Simple and well-known             | Complex but flexible                  |
| **Performance**   | Good for medium-sized projects          | Optimized for 2D                  | Excellent performance                 |
| **Features**      | 2D/3D games, audio, input handling, basic UI | 2D games and applications, audio, networking, text rendering | 2D games, advanced platform handling |
| **Documentation** | Very well-documented with many examples | Good documentation and tutorials  | Good but more technical               |
| **Platforms**     | Windows, macOS, Linux, Web, Android, iOS | Windows, macOS, Linux             | Windows, macOS, Linux, Android        |

---

## Why We Chose SFML

We chose to use **SFML** for the following reasons:
1. **Adequate performance**: SFML is optimized for 2D games like R-Type and provides performance suitable for the project’s needs.
2. **Ease of use**: Its simplicity allows faster development and debugging, which is crucial for meeting project deadlines.
3. **Past experience**: We have previously worked with SFML during our studies, reducing the learning curve.
4. **Quality documentation**: SFML offers clear documentation along with practical tutorials, speeding up development.

---

## References

- [SFML Official Website](https://www.sfml-dev.org/)
- [Comparison of Raylib, SFML, and SDL](https://github.com/raysan5/raylib)
- [SFML Tutorials](https://www.sfml-dev.org/tutorials/2.5/)
