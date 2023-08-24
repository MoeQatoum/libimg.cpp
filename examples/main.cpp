#include <filesystem>
#include <iostream>
#include <libimg>

#include "timer.hpp"

using namespace img;
Timer global_timer;

void img_info(const Image& img, const fs::path path) {
    std::cout << "name     : " << path.filename() << "\n";
    std::cout << "width    : " << img.width() << "\n";
    std::cout << "height   : " << img.height() << "\n";
    std::cout << "size     : " << img.pixelCount() << "\n";
    std::cout << "channels : " << img.chanelCount() << "\n";
    std::cout << "data size: " << sizeof(img) << "\n";
}

void add_noise_grey_gs(fs::path path, float mean, float dev) {
    Image img{path};

    global_timer.restart();
    img.greyScaleLum().addGaussianNoise(mean, dev);
    global_timer.log_elapsed("add_noise_grey_gs");

    img.save(fs::current_path() / (path.stem().string() + "-noise-add-grey-gs" + path.extension().string()));
}

void add_noise_grey_sap(fs::path path, float prob) {
    Image img{path};

    global_timer.restart();
    img.addSaltAndPepperNoise(prob);
    global_timer.log_elapsed("add_noise_grey_s&p");

    img.save(fs::current_path() / (path.stem().string() + "-noise-add-greys&p" + path.extension().string()));
}

void add_noise_gs(fs::path path, float mean, float dev) {
    Image img{path};
    img.save(fs::current_path() / path.filename());

    global_timer.restart();
    img.addGaussianNoise(mean, dev);
    global_timer.log_elapsed("add_noise_gs");

    img.save(fs::current_path() / (path.stem().string() + "-noise-add-gs" + path.extension().string()));
}

void mask(fs::path path, float r_mask, float g_mask, float b_mask) {
    Image img{path};

    global_timer.restart();
    img.colorMask(r_mask, g_mask, b_mask);
    global_timer.log_elapsed("mask");

    img.save(fs::current_path() / (path.stem().string() + "-masked" + path.extension().string()));
}

void genGaussianRandomNoise(img::i32 w, img::i32 h, float mean = (255.f / 2), float dev = 255) {
    global_timer.restart();
    Image img = Image::gaussianRandomNoise(w, h, Image::PF_RGB8, mean, dev);
    global_timer.log_elapsed("gaussian_noise");

    img.save(fs::current_path() / "img-gaussian-noise.jpeg");
}

void add(fs::path path, fs::path other) {
    if (path.extension() != other.extension()) {
        return;
    }
    Image original_img{path};
    Image other_img{other};

    global_timer.restart();
    Image add_image = (original_img + other_img);
    global_timer.log_elapsed("add_image");

    add_image.save(fs::current_path()
                   / (path.stem().string() + "-" + other.stem().string() + "plus" + path.extension().string()));
}

void sub(fs::path path, fs::path other) {
    if (path.extension() != other.extension()) {
        return;
    }
    Image original_img{path};
    Image other_img{other};

    global_timer.restart();
    Image sub_image = (original_img - other_img);
    global_timer.log_elapsed("sub_image");

    sub_image.save(fs::current_path()
                   / (path.stem().string() + "-" + other.stem().string() + "-sub" + path.extension().string()));
}

void greyScaleLum(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.greyScaleLum();
    global_timer.log_elapsed("grey_scale_lum");

    img.save(fs::current_path() / (path.stem().string() + "-grey-lum" + path.extension().string()));
}

void greyScaleAvg(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.greyScaleAvg();
    global_timer.log_elapsed("grey_scale_avg");

    img.save(fs::current_path() / (path.stem().string() + "-gre-avg" + path.extension().string()));
}

void crop(fs::path path) {
    Image img{path};

    global_timer.restart();
    // img.crop(1, 1, img.width() / 4, img.height() / 4);
    Image cropped = img.crop(1, 1, (img.width() / 4) * 3, (img.height() / 4) * 3);
    // Image cropped = img.crop(img.width() / 4, img.height() / 4, (img.width() / 4) * 2, (img.height() / 4) * 2);
    global_timer.log_elapsed("cropping");

    img.save(fs::current_path() / (path.stem().string() + "-cropped" + path.extension().string()));
}

void flipX(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.flipX();
    global_timer.log_elapsed("flipping_X");

    img.save(fs::current_path() / (path.stem().string() + "-flipX" + path.extension().string()));
}

void flipY(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.flipY();
    global_timer.log_elapsed("flipping_Y");

    img.save(fs::current_path() / (path.stem().string() + "-flipY" + path.extension().string()));
}

void RotateRight(fs::path path) {
    Image img{path};

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

void RotateLeft(fs::path path) {
    Image img{path};

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

void neg(fs::path path) {
    Image img{path};

    global_timer.restart();
    ~img;
    global_timer.log_elapsed("neg");

    img.save(fs::current_path() / (path.stem().string() + "-neg" + path.extension().string()));
}

void padAll(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.pad(50, 70, 10, 50, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-1" + path.extension().string()));
}

void padAll1(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.padBorderEqual(100, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-2" + path.extension().string()));
}

void padTop(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.padTop(100, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-3" + path.extension().string()));
}

void padBottom(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.padBottom(100, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-4" + path.extension().string()));
}

void padRight(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.padRight(100, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-5" + path.extension().string()));
}

void padLeft(fs::path path) {
    Image img{path};

    global_timer.restart();
    img.padLeft(100, {255});
    global_timer.log_elapsed("pad_border");

    img.save(fs::current_path() / (path.stem().string() + "-pad-border-6" + path.extension().string()));
}

void do_all(fs::path path) {
    img_info(Image{path}, path);
    mask(path, .5f, 0, 0);

    float dev = 50.f;
    float m   = 0.f;
    add_noise_grey_gs(path, m, dev);
    add_noise_gs(path, m, dev);

    add_noise_grey_sap(path, .1f);

    add(path, (path.parent_path() / "img-3.jpeg"));
    sub(path, (path.parent_path() / "img-3.jpeg"));

    greyScaleAvg(path);
    greyScaleLum(path);
    crop(path);
    flipX(path);
    flipY(path);
    RotateRight(path);
    RotateLeft(path);
    neg(path);
    padAll(path);
    padAll1(path);
    padRight(path);
    padLeft(path);
    padTop(path);
    padBottom(path);
    std::cout << "------------------------------\n";
}

int main(int argc, char* argv[]) {
    fs::path img_path = fs::path(argv[0]).parent_path() / "test_images/";

    do_all(img_path / "img-1.jpeg");
    do_all(img_path / "img-2.png");
    do_all(img_path / "img-3.jpeg");
    do_all(img_path / "img-4.png");
    do_all(img_path / "img-5.jpeg");
    do_all(img_path / "img-6.jpeg");
    do_all(img_path / "img-7.png");

    // genGaussianRandomNoise(1270, 800);
    // genPoissonRandomNoise(1270, 800);

    return 0;
}
