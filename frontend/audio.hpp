#pragma once

#include "SDL_audio.h"
#include "SDL.h"

#include "../lynes/devices/apu/apu.hpp"

#define AUDIO_DEVICE_SAMPLERATE 48000
#define AUDIO_BUFFER_SIZE 32
#define AUDIO_RESAMPLER_FILL_SIZE \
    ((size_t)(AUDIO_BUFFER_SIZE * ((double)NES_NATIVE_SAMPLERATE / (double)AUDIO_DEVICE_SAMPLERATE)))

#include <cstdint>
#include <vector>
#include <array>

namespace frontend {
    namespace audio {
        SDL_AudioDeviceID dev;
        SDL_AudioSpec obtained, desired;

        SDL_AudioStream* stream = nullptr;

        std::array <int16_t, AUDIO_RESAMPLER_FILL_SIZE> mixed_sample_buf = { 0 };

        void update(void* ud, uint8_t* buf, int size) {
            for (size_t i = 0; i < AUDIO_RESAMPLER_FILL_SIZE; i++) {
                mixed_sample_buf[i] = apu::get_sample();
            }

            SDL_AudioStreamPut(stream, mixed_sample_buf.data(), mixed_sample_buf.size() * 2);

            SDL_AudioStreamFlush(stream);

            SDL_AudioStreamGet(stream, buf, size);
        }

        void init() {
            SDL_Init(SDL_INIT_AUDIO);

            stream = SDL_NewAudioStream(
                AUDIO_S16SYS,
                1,
                NES_NATIVE_SAMPLERATE,
                AUDIO_S16SYS,
                1,
                AUDIO_DEVICE_SAMPLERATE
            );

            desired.freq     = AUDIO_DEVICE_SAMPLERATE;
            desired.format   = AUDIO_S16SYS;
            desired.channels = 1;
            desired.samples  = AUDIO_BUFFER_SIZE;
            desired.callback = &update;
            desired.userdata = nullptr;

            dev = SDL_OpenAudioDevice(nullptr, 0, &desired, nullptr, 0);

            if (dev)
                SDL_PauseAudioDevice(dev, 0);
        }
    }
}