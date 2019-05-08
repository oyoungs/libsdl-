#ifndef SDLPP_MIXER_H
#define SDLPP_MIXER_H


#include <string>
#include <SDL2/SDL_stdinc.h>

typedef struct Mix_Chunk Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

namespace sdl {

namespace mixer {

    int initialize(int flags);

    void quit();

    int query(int & frequency, Uint16& format, int &channels);

    std::string error();

    struct audio {
        audio(int frequency, Uint16 format, int channels, int chunksize);
        void close();
        ~audio();
    private:
        bool _opened {false};
    };

    struct initializer {

        initializer();

        ~initializer();

        int flags() const;

    private:
        int _flags{0};

    };

    struct chunk {
        explicit chunk(Mix_Chunk *handle);
        explicit chunk(const char *file);

        void set_volume(double volume);
        double volume() const;

        operator Mix_Chunk *() {
            return _handle;
        }

        bool empty() const;

        operator bool() const;

        chunk(const chunk& other);
        chunk(chunk&& other) noexcept;

        ~chunk();

    private:
        bool _autorelease {false};
        Mix_Chunk *_handle{nullptr};
    };

    namespace wave {
        chunk load(const char *file);
    }

    struct channel {
        static channel all;

        channel();
        explicit channel(int ch);

        void set_volume(double volumen);
        double volume() const;

        int play(chunk& chk, int loops = 0);
        int play_timed(chunk& chk, int loops = 0, int ticks = 1000);
        bool playing() const;

        void pause();
        void resume();
        bool paused() const;

        int halt();
        void stop() { halt(); }

        int expire(int ticks);

        chunk get_chunk();

        static int allocate(int number = 2);


    private:
        int _channel{0};
    };



    struct music {

        music();
        explicit music(const char *file);

        static music load(const char *file);

        bool empty() const;

        int play(int loops = 0);
        int fadein(int loops = 0, int time = 1000 /* 1000ms */);

        void rewind();

        void pause();
        bool paused() const;
        void resume();

        int stop();
        int fadeout(int time = 3000 /* 3s*/);


        bool playing() const ;

        music(const music& other): _handle(other._handle) {}
        music(music&& other): _handle(other._handle) { other._handle = nullptr;}

        ~music();
    private:
        Mix_Music *_handle{nullptr};
    };
}

}

#endif
