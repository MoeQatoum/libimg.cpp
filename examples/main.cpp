#include <filesystem>
#include <iostream>
#include <libimg>

#include "timer.hpp"

using namespace img;
Timer global_timer;

int i = 0;

template<typename T>
void img_info(const Image<T>& img, const fs::path path) {
    std::cout << "name     : " << path.filename() << "\n";
    std::cout << "width    : " << img.width() << "\n";
    std::cout << "height   : " << img.height() << "\n";
    std::cout << "size     : " << img.pixelCount() << "\n";
    std::cout << "channels : " << img.chanelCount() << "\n";
    std::cout << "data size: " << sizeof(img) << std::endl << "\n";
}

template<typename T>
void add_noise_grey_gs(fs::path path, float mean, float dev) {
    Image<T> img{path};

    global_timer.restart();
    auto gs = img.greyScaleLum();
    gs.addGaussianNoise(mean, dev);
    global_timer.log_elapsed("add_noise_grey_gs");

    gs.save(fs::current_path() / (path.stem().string() + "-noise-add-grey-gs" + path.extension().string()));
}

template<typename T>
void add_noise_grey_sap(fs::path path, float prob, float top, float bot) {
    Image<T> img{path};

    global_timer.restart();
    auto gs = img.addSaltAndPepperNoise(prob, top, bot);
    global_timer.log_elapsed("add_noise_grey_s&p");

    gs.save(fs::current_path() / (path.stem().string() + "-noise-add-greys&p" + path.extension().string()));
}

template<typename T>
void add_noise_gs(fs::path path, float mean, float dev) {
    Image<T> img{path};

    global_timer.restart();
    img.addGaussianNoise(mean, dev);
    global_timer.log_elapsed("add_noise_gs");

    img.save(fs::current_path() / (path.stem().string() + "-noise-add-gs" + path.extension().string()));
}

template<typename T>
void mask(fs::path path, float r_mask, float g_mask, float b_mask) {
    Image<T> img{path};

    global_timer.restart();
    img.colorMask(r_mask, g_mask, b_mask);
    global_timer.log_elapsed("mask");

    img.save(fs::current_path() / (path.stem().string() + "-masked" + path.extension().string()));
}

template<typename T>
void genGaussianRandomNoise(img::i32 w, img::i32 h, float mean = (255.f / 2), float dev = 255) {
    global_timer.restart();
    auto img = Image<T>::gaussianRandomNoise(w, h, mean, dev);
    global_timer.log_elapsed("gaussian_noise");

    img.save(fs::current_path() / "img-gaussian-noise.jpeg");
}

template<typename T>
void add(fs::path path, fs::path other) {
    if (path.extension() != other.extension()) {
        return;
    }
    Image<T> original_img{path};
    Image<T> other_img{other};

    global_timer.restart();
    auto add_image = (original_img + other_img);
    global_timer.log_elapsed("add_image");

    add_image.save(fs::current_path()
                   / (path.stem().string() + "-" + other.stem().string() + "plus" + path.extension().string()));
}

template<typename T>
void sub(fs::path path, fs::path other) {
    if (path.extension() != other.extension()) {
        return;
    }
    Image<T> original_img{path};
    Image<T> other_img{other};

    global_timer.restart();
    auto sub_image = (original_img - other_img);
    global_timer.log_elapsed("sub_image");

    sub_image.save(fs::current_path()
                   / (path.stem().string() + "-" + other.stem().string() + "-sub" + path.extension().string()));
}

template<typename T>
void greyScaleLum(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    auto gs = img.greyScaleLum();
    global_timer.log_elapsed("grey_scale_lum");

    gs.save(fs::current_path() / (path.stem().string() + "-grey-lum" + path.extension().string()));
}

template<typename T>
void greyScaleAvg(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    auto gs = img.greyScaleAvg();
    global_timer.log_elapsed("grey_scale_avg");

    gs.save(fs::current_path() / (path.stem().string() + "-gre-avg" + path.extension().string()));
}

template<typename T>
void crop(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    // img.crop(1, 1, img.width() / 4, img.height() / 4);
    Image cropped = img.crop(1, 1, (img.width() / 4) * 3, (img.height() / 4) * 3);
    // Image cropped = img.crop(img.width() / 4, img.height() / 4, (img.width() / 4) * 2, (img.height() / 4) * 2);
    global_timer.log_elapsed("cropping");

    img.save(fs::current_path() / (path.stem().string() + "-cropped" + path.extension().string()));
}

template<typename T>
void flipX(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.flipX();
    global_timer.log_elapsed("flipping_X");

    img.save(fs::current_path() / (path.stem().string() + "-flipX" + path.extension().string()));
}

template<typename T>
void flipY(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.flipY();
    global_timer.log_elapsed("flipping_Y");

    img.save(fs::current_path() / (path.stem().string() + "-flipY" + path.extension().string()));
}

template<typename T>
void RotateRight(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.rotateRight();
    global_timer.log_elapsed("rotate_right");

    img.save(fs::current_path() / (path.stem().string() + "-1RR" + path.extension().string()));
    global_timer.restart();
    img.rotateRight();
    global_timer.log_elapsed("rotate_right");

    img.save(fs::current_path() / (path.stem().string() + "-2RR" + path.extension().string()));
    global_timer.restart();
    img.rotateRight();
    global_timer.log_elapsed("rotate_right");

    img.save(fs::current_path() / (path.stem().string() + "-3RR" + path.extension().string()));
    global_timer.restart();
    img.rotateRight();
    global_timer.log_elapsed("rotate_right");

    img.save(fs::current_path() / (path.stem().string() + "-4RR" + path.extension().string()));
}

template<typename T>
void RotateLeft(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.rotateLeft();
    global_timer.log_elapsed("rotate_left");

    img.save(fs::current_path() / (path.stem().string() + "-1RL" + path.extension().string()));

    global_timer.restart();
    img.rotateLeft();
    global_timer.log_elapsed("rotate_left");

    img.save(fs::current_path() / (path.stem().string() + "-2RL" + path.extension().string()));

    global_timer.restart();
    img.rotateLeft();
    global_timer.log_elapsed("rotate_left");

    img.save(fs::current_path() / (path.stem().string() + "-3RL" + path.extension().string()));

    global_timer.restart();
    img.rotateLeft();
    global_timer.log_elapsed("rotate_left");

    img.save(fs::current_path() / (path.stem().string() + "-4RL" + path.extension().string()));
}

template<typename T>
void neg(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    ~img;
    global_timer.log_elapsed("neg");

    img.save(fs::current_path() / (path.stem().string() + "-neg" + path.extension().string()));
}

template<typename T>
void padAll(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.pad(50, 70, 10, 50, {.r = 255, .g = 0, .b = 0});
    // img.pad(1, 1, 1, 1, {255, 0, 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-1" + path.extension().string()));
}

template<typename T>
void padAll1(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.padBorderEqual(100, {.r = 255, .g = 0, .b = 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-2" + path.extension().string()));
}

template<typename T>
void padTop(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.padTop(100, {.r = 255, .g = 0, .b = 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-3" + path.extension().string()));
}

template<typename T>
void padBottom(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.padBottom(100, {.r = 255, .g = 0, .b = 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-4" + path.extension().string()));
}

template<typename T>
void padRight(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.padRight(100, {.r = 255, .g = 0, .b = 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-5" + path.extension().string()));
}

template<typename T>
void padLeft(fs::path path) {
    Image<T> img{path};

    global_timer.restart();
    img.padLeft(100, {.r = 255, .g = 0, .b = 0});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-6" + path.extension().string()));
}

template<typename T>
void do_all(fs::path path) {
    img_info(Image<T>{path}, path);
    // mask<T>(path, 0, 1.f, 0);

    float dev = 100.f;
    float m   = 0.f;
    add_noise_grey_gs<T>(path, m, dev);
    add_noise_gs<T>(path, m, dev);
    add_noise_grey_sap<T>(path, .01f, 0.f, 1.f);

    add<T>(path, (path.parent_path() / "img-3.jpeg"));
    sub<T>(path, (path.parent_path() / "img-3.jpeg"));

    greyScaleAvg<T>(path);
    greyScaleLum<T>(path);
    crop<T>(path);
    flipX<T>(path);
    flipY<T>(path);
    RotateRight<T>(path);
    RotateLeft<T>(path);
    neg<T>(path);
    padAll<T>(path);
    padAll1<T>(path);
    padRight<T>(path);
    padLeft<T>(path);
    padTop<T>(path);
    padBottom<T>(path);
    std::cout << "------------------------------\n";
}

int main(int argc, char* argv[]) {
    fs::path img_path = fs::path(argv[0]).parent_path() / "test_images/";

    do_all<RGB8>(img_path / "img-0.jpeg");
    do_all<RGB8>(img_path / "img-1.jpeg");
    do_all<RGB8>(img_path / "img-2.png");
    do_all<RGB8>(img_path / "img-3.jpeg");

    do_all<RGBa8>(img_path / "img-4.png");
    do_all<RGB8>(img_path / "img-5.jpeg");
    do_all<RGB8>(img_path / "img-6.jpeg");
    do_all<RGB8>(img_path / "img-7.png");

    // genGaussianRandomNoise(1270, 800);
    // genPoissonRandomNoise(1270, 800);

    return 0;
}
