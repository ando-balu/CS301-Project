# CS 301: Low-Level Audio Synthesis in C++ Using SDL2

This project demonstrates real-time audio synthesis using SDL2 in C++. It generates various waveforms and plays musical notes loaded from a text file.

## Features
- Generate waveforms: square, triangle, sawtooth, and sine.
- Read musical notes from a file.
- Play notes in real-time using SDL2's audio callback system.

## Dependencies

1. **SDL2 Library**:
   - Install SDL2 via your package manager or download it from the [official SDL2 website](https://libsdl.org).
2. **C++ Compiler**:
   - A compiler supporting C++11 or later (e.g., `g++`, `clang++`).
3. **Make** (optional):
   - For easier builds using a `Makefile` (recommended).

## Build Instructions

### 1. Clone the Repository
```bash
git clone https://github.com/ando-balu/CS301-Project.git
cd CS301-Project
```

### 2. Build the Project

#### Using `g++`
```bash
g++ -std=c++11 -o play main.cpp -lSDL2
```

#### Using a `Makefile` (if available)
If the repository includes a `Makefile`, build the project by running:
```bash
make
```

### 3. Run the Program
```bash
./play
```
Ensure the input file (`mario_theme.txt`) is in the same directory as the executable.

## Input File Format
The program reads notes from a text file, where each line represents a note with the following format:
```plaintext
<Waveform_Type> <Frequency_Hz> <Duration_ms> <Volume_Percentage>
```
- **Waveform_Type**: `0` (Square), `1` (Triangle), `2` (Sawtooth), `3` (Sine).
- **Frequency_Hz**: Frequency of the note in Hertz (e.g., 440 for A4).
- **Duration_ms**: Duration of the note in milliseconds.
- **Volume_Percentage**: Volume level (0-100).

### Example (`song.txt`):
```plaintext
0 440 500 80
1 554.37 300 50
2 659.25 200 100
```

## Troubleshooting

1. **SDL2 Not Found**:
   - Ensure SDL2 is installed and the library paths are correctly set.
   - On Linux, install via package manager (e.g., `sudo apt install libsdl2-dev`).

2. **Audio Latency or Distortion**:
   - Try adjusting the buffer size (`samples` field in `SDL_AudioSpec`).

3. **File Reading Errors**:
   - Verify the input file exists and follows the specified format.

## Extending the Program

- Add support for polyphony (playing multiple notes simultaneously).
- Implement real-time controls for dynamic audio playback.
- Create a graphical interface to load and play music files.

## License
This project is open-source and available under the [MIT License](LICENSE).

## Acknowledgments
- SDL2 for providing a robust multimedia framework.
- Inspiration from classic synthesizers and audio programming tutorials.

