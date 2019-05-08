#include <sdl++/sdl.h>

#include <SDL2/SDL.h>

namespace sdl {

void initialize(Uint32 flags) { SDL_Init(flags); }

void quit() {SDL_Quit(); }

void delay(unsigned ms) { SDL_Delay(ms); }

std::string error() { return SDL_GetError(); }

namespace audio {

    device::device(): _name() {}
    device::device(const char *name, bool capture)
        : _name(name), _capture(capture) {}
    device::device(const std::string& name, bool capture)
        : _name(name), _capture(capture) {}

    int device::count(bool capture) {
        return SDL_GetNumAudioDevices(capture);
    }



    std::string device::name(int index, bool capture) {
        if(auto n = SDL_GetAudioDeviceName(index, capture)) {
         return n;
        }
        return {};
    }

    std::string device::name() const {
        return _name;
    }

    spec& device::desired_spec() {
        return _desired;
    }

    const spec& device::desired_spec() const {
        return _desired;
    }

    const spec& device::obtained_spec() const  {
     return _obtained;
    }

    bool device::good() const {
        return _devid >= 2;
    }

    void device::pause(bool paused) {
        SDL_PauseAudioDevice(_devid, paused);
    }

    template <typename T, void (T::*func)(Uint8 *, int)>
    void sdl_audio_spec_callback(void *obj, Uint8 * stream, int size)
    {
        (static_cast<T *>(obj)->*func)(stream, size);
    }

    void device::open(const callback_t &callback, int allow_changes) {
        SDL_AudioSpec want = _desired;
        SDL_AudioSpec obtain{};
        want.callback = sdl_audio_spec_callback<device, &device::process>;
        want.userdata = this;
        if((_devid = SDL_OpenAudioDevice(_name.empty() ? nullptr: _name.c_str(), _capture, &want, &obtain, allow_changes))) {
         _obtained = obtain;
        }

        _audio_callback = callback;
    }

    void device::close() {
        SDL_CloseAudioDevice(_devid);
    }

    void device::set_audio_callback(const device::callback_t& callback) {
        _audio_callback = callback;
    }

    void device::process(Uint8 *stream, int size)
    {
        if(_audio_callback) _audio_callback(stream, size);
    }

    int init() { return SDL_InitSubSystem(SDL_INIT_AUDIO); }

}

}
