#include <sdl++/sdl.h>
#include <sdl++/mixer.h>
#include <SDL2/SDL_mixer.h>


namespace sdl {

namespace mixer {
    int initialize(int flags) {
     return Mix_Init(flags);
    }

    void quit() {
     Mix_Quit();
    }

    int query(int & frequency, Uint16& format, int &channels) {
     return Mix_QuerySpec(&frequency, &format, &channels);
    }

    std::string error() { return Mix_GetError(); }


    audio::audio(int frequency, Uint16 format, int channels, int chunksize) {
     _opened = Mix_OpenAudio(frequency, format, channels, chunksize) == 0;
    }

    void audio::close()  { Mix_CloseAudio(); }

    audio::~audio()  { if(_opened) close(); }

    initializer::initializer() {  _flags = initialize(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC); }

    int initializer::flags() const { return _flags; }

    initializer::~initializer()  { quit(); }

    chunk::chunk(Mix_Chunk *handle): _autorelease(false),  _handle(handle) {}
    chunk::chunk(const char *file): _autorelease(true) { _handle = Mix_LoadWAV(file); }

    chunk::chunk(const chunk& other)
     : _autorelease(other._autorelease), _handle(other._handle) {}

    chunk::chunk(chunk&& other) noexcept
    : _autorelease(other._autorelease), _handle(other._handle)
    {
         other._autorelease = false;
         other._handle = nullptr;
    }

    chunk::~chunk() {
     if(_autorelease) Mix_FreeChunk(_handle);
    }

    void chunk::set_volume(double volume)
    {
     volume = clamp(volume, 0.0, 1.0);
     Mix_VolumeChunk(_handle, static_cast<int>(volume * MIX_MAX_VOLUME));
    }

    double chunk::volume() const {
     return static_cast<double>(Mix_VolumeChunk(_handle, -1)) / MIX_MAX_VOLUME;
    }

    bool chunk::empty() const {
     return _handle == nullptr;
    }

    chunk::operator bool() const {
     return _handle;
    }

    namespace wave {
        chunk load(const char* file) { return chunk(file); }
    }

    channel::channel() : _channel(0) {}
    channel::channel(int ch): _channel(ch) {}

    void channel::set_volume(double volume)
    {
        volume = clamp(volume, 0.0, 1.0);
        Mix_Volume(_channel, static_cast<int>(volume * MIX_MAX_VOLUME));
    }

    double channel::volume() const {
        return static_cast<double>(Mix_Volume(_channel, -1)) / MIX_MAX_VOLUME;
    }

    int channel::play(chunk& chk, int loops) { return Mix_PlayChannel(_channel, chk, loops); }

    int channel::play_timed(chunk& chk, int loops, int ticks) {
        return Mix_PlayChannelTimed(_channel, chk, loops, ticks);
    }

    bool channel::playing() const { return Mix_Playing(_channel); }

    void channel::pause() { Mix_Pause(_channel); }

    void channel::resume() { Mix_Resume(_channel); }

    bool channel::paused() const { return Mix_Paused(_channel); }


    int channel::halt() { return Mix_HaltChannel(_channel); }

    int channel::expire(int ticks) { return Mix_ExpireChannel(_channel, ticks); }

    chunk channel::get_chunk() {
     return chunk(Mix_GetChunk(_channel));
    }

    channel channel::all(-1);

    int channel::allocate(int number) { return Mix_AllocateChannels(number); }


    music::music() {}
    music::music(const char *file) { _handle = Mix_LoadMUS(file); }

    bool music::empty() const  { return _handle == nullptr; }

    int music::play(int loops) { return Mix_PlayMusic(_handle, loops); }

    int music::fadein(int loops, int time ) { return Mix_FadeInMusic(_handle, loops, time); }

    void music::rewind() { Mix_RewindMusic(); }

    void music::pause() { Mix_PauseMusic(); }

    bool music::paused() const { return Mix_PausedMusic(); }

    void music::resume() { Mix_ResumeMusic(); }

    int music::stop() { return Mix_HaltMusic(); }

    int music::fadeout(int time) { return  Mix_FadeOutMusic(time); }

    bool music::playing()  const { return Mix_PlayingMusic(); }

    music music::load(const char *file) { return music(file); }

    music::~music()  { Mix_FreeMusic(_handle); }
}
}
