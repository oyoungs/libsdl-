#include <iostream>
#include <nlohmann/json.hpp>
#include <sspdlog/sspdlog.h>


#include <sdl++/sdl.h>

int main(int , char **)
{

    sdl::audio::init();

    auto count = sdl::audio::device::count();

    std::cout << "audio device count: " << count << std::endl;

    std::cout << "audio device name: " << sdl::audio::device::name(0) << std::endl;

    auto device = sdl::audio::device{};

    device.desired_spec().freq = 44100;
    device.desired_spec().format = AUDIO_S16SYS;
    device.desired_spec().channels = 2;
    device.desired_spec().samples = 1024;

    device.open([=](Uint8 *stream, int size) {
        std::cout << "size: " << size  << std::endl;
    });

    if(!device.good()) {
        std::cerr << "open failed: " << sdl::error() << std::endl;
        return -1;
    }

    std::cout << "real format: " << device.obtained_spec().format << std::endl;
    std::cout << "real frequency: " << device.obtained_spec().freq << std::endl;
    std::cout << "real channels: " << device.obtained_spec().channels << std::endl;

    sdl::audio::quit();

    return 0;
}

