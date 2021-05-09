#include <iostream>

int main(int argc, char* argv[]) {

    // Image
    const int im_width = 256, im_height = 256;

    // Render
    std::cout << "P3\n" << im_width << " " << im_height << "\n255\n";

    for (int j = im_height - 1; j >= 0; j--) {
        std::cerr << "\nScanlines reamaining: " << j + 1 << " " << std::flush;
        
        for (int i = 0; i < im_width; i++) {
            double r = double(i) / (im_width - 1);
            double g = double(j) / (im_height - 1);
            double b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    std::cerr << "\nDone Rendereing!\n";

    return 0;
}