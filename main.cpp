/* 
main.cpp
Andrew Baluyot
Started: 2024-11-15
Updated: 2024-12-12
  
For CS 301 Fall 2024 Class Project (Generating 8-bit Music)
A C++ file that generates and plays audio based on note data taken from a .txt file.
It uses SDL2 for audio playback and includes functionality to generate various waveform types
that 8-bit music is comprised of.
*/

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Audio format constants
const int SAMPLE_RATE = 44100; // Standard audio sample rate
const int MAX_AMPLITUDE = 127; // 8-bit range (0-255); 2's complement (-128-127)

// Waveform type
enum WaveformType { SQUARE, TRIANGLE, SAWTOOTH, SINE };

// Audio state
struct AudioState {
    WaveformType type;
    double frequency;
    double phase;
    double volume; // Volume as a percentage (0.0 - 1.0)
};

// Note structure
struct Note {
    WaveformType type;
    double frequency; // Frequency in Hz
    int duration;     // Duration in milliseconds
    double volume;    // Volume as a percentage (0.0 - 1.0)
};

// Function to generate a single sample for specified 8-bit music waveforms
Uint8 generateSample(WaveformType type, double frequency, double& phase, double volume) {
    double increment = frequency / SAMPLE_RATE; // Phase increment per sample
    double value = 0;

    // Calculates sample value of a specified waveform based on its position during the phase
    switch (type) {
        case SQUARE:
            value = (phase < 0.5) ? MAX_AMPLITUDE : -MAX_AMPLITUDE;
            break;
        case TRIANGLE:
            value = (phase < 0.5) ? (4 * MAX_AMPLITUDE * phase - MAX_AMPLITUDE) : (-4 * MAX_AMPLITUDE * (phase - 0.5) + MAX_AMPLITUDE);
            break;
        case SAWTOOTH:
            value = 2 * MAX_AMPLITUDE * (phase - 0.5);
            break;
        case SINE:
            value = MAX_AMPLITUDE * std::sin(2 * M_PI * phase);
            break;
    }

    phase += increment;
    if (phase >= 1.0) phase -= 1.0; // Wrap phase
    value *= volume; // Scale volume (0.0 - 1.0)
    return static_cast<Uint8>(value + 128); // Shift to 8-bit range
}

// SDL audio callback function to fill audio buffer with generated samples
void audioCallback(void* userdata, Uint8* stream, int len) {
    AudioState* state = static_cast<AudioState*>(userdata);
    for (int i = 0; i < len; ++i) {
        stream[i] = generateSample(state->type, state->frequency, state->phase, state->volume);
    }
}

// Function to read notes from a file into a vector format
std::vector<Note> readNotesFromFile(const std::string& filename) {
    std::vector<Note> notes;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return notes;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        double frequency, volume;
        int duration, waveform;
        if (ss >> waveform >> frequency >> duration >> volume) {
            notes.push_back({static_cast<WaveformType>(waveform), frequency, duration, volume / 100.0}); // Convert percentage to 0.0-1.0
        }
    }

    file.close();
    return notes;
}

int main() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    AudioState audioState = {SQUARE, 440.0, 0.0, 1.0}; // Default State

    // Configure SDL_AudioSpec structure
    SDL_AudioSpec desiredSpec{};
    desiredSpec.freq = SAMPLE_RATE;
    desiredSpec.format = AUDIO_U8; // 8-bit unsigned audio
    desiredSpec.channels = 1;      // Mono audio
    desiredSpec.samples = 4096;    // Buffer size
    desiredSpec.callback = audioCallback;
    desiredSpec.userdata = &audioState;

    SDL_AudioSpec obtainedSpec{};
    if (SDL_OpenAudio(&desiredSpec, &obtainedSpec) < 0) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Load notes from file
    std::vector<Note> notes = readNotesFromFile("mario_theme.txt");
    if (notes.empty()) {
        std::cerr << "No notes to play!" << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_PauseAudio(0); // Start audio playback

    // Play each note
    for (const auto& note : notes) {
        audioState.frequency = note.frequency;
        audioState.type = note.type;
        audioState.volume = note.volume;
        audioState.phase = 0.0;

        SDL_Delay(note.duration); // Play note for its duration
    }

    SDL_PauseAudio(1); // Stop audio playback
    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}
