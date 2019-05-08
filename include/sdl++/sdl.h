#ifndef MAIN_SDL_H
#define MAIN_SDL_H

#include <string>
#include <functional>

#include <SDL2/SDL.h>

namespace sdl {

    void initialize(Uint32 flags);
    void quit();
    void delay(unsigned ms);

    std::string error();

    template <typename T, typename U, typename R>
    R clamp(R in, T min, U max) {
        return std::min( std::max(in, R(min)), R(max));
    }

    namespace audio {

        struct spec {
            int freq;                   /**< DSP frequency -- samples per second */
            Uint32 format;     /**< Audio data format */
            Uint32 channels;             /**< Number of channels: 1 mono, 2 stereo */
            Uint32 silence;              /**< Audio buffer silence value (calculated) */
            Uint32 samples;             /**< Audio buffer size in samples (power of 2) */
            Uint32 padding;             /**< Necessary for some compile environments */
            Uint32 size;

            template<typename T>
            operator T() const {
                T type{};
                to_type(type, *this);
                return type;
            }
            template<typename T>
            spec& operator=(const T& type) {
                from_type(type, *this);
                return *this;
            }
        };

        template <typename T>
        void to_type(T& type, const spec& sp)
        {
            type.freq = sp.freq;
            type.format = sp.format;
            type.channels = sp.channels;
            type.silence = sp.silence;
            type.samples = sp.samples;
            type.padding = sp.padding;
            type.size = sp.size;
        }

        template <typename T>
        void from_type(const T& type, spec& sp)
        {
            sp.freq = type.freq;
            sp.format = type.format;
            sp.channels = type.channels;
            sp.silence = type.silence;
            sp.samples = type.samples;
            sp.padding = type.padding;
            sp.size = sp.size;
        }

        struct device {
            using callback_t = std::function<void(Uint8*, int)>;

            static int count(bool capture = false);
            static std::string name(int index, bool capture = false);

            device();
            device(const char *name, bool capture = false);
            device(const std::string& name, bool capture = false);

            std::string name() const;

            spec& desired_spec();

            const spec& desired_spec() const;

            const spec& obtained_spec() const;

            void open(const callback_t& callback = nullptr, int allow_changes = 7);

            void close();

            void set_audio_callback(const callback_t& callback);

            bool good() const;

            void pause(bool paused);

            ~device();

        private:
            void process(Uint8 *stream, int size);

        private:
            std::string _name;
            bool _capture{false};
            spec _desired, _obtained;
            Uint32  _devid{0};

            callback_t _audio_callback;
        };

        int init();
        void quit();
    }

}


#endif
