# Raytracing

Raytracing from scratch

### Compiling

To compile the project, open a terminal, go to the repository directory, and execute the following commands:

```bash
user@computer: ~/raytracing/ $ mkdir build && cd build
user@computer: ~/raytracing/build $ cmake ../
user@computer: ~/raytracing/build $ make
```

## Ray Tracing in One Weekend 

Implementations of codes described in the book [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by [Peter Shirley](https://github.com/petershirley).


### Running

The program will render a scene with randomly positioned spheres in a fairly high resolution. The image data is redirected to the stdout and can be re-directed to a [PPM](https://en.wikipedia.org/wiki/Netpbm) image file using the following command:

```bash
user@computer: ~/raytracing/build $ ./main > image.ppm
Scanlines reamaining:   0 of 800 -  100% - ETA 0s
Done Rendering!
```

![sample_random_scene](./imgs/sample_random_scene.png)

#### Other sample images

![grid](./imgs/grid_1.png)


## Ray Tracing: The Next Week 

Improvements of the previuos codes described in the book [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) by [Peter Shirley](https://github.com/petershirley).

## Rendering samples

![grid_2](./imgs/grid_2.png)

## Ray Tracing: The Rest of Your Life

Improvements of the previuos codes described in the book [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) by [Peter Shirley](https://github.com/petershirley).

![grid_3](./imgs/grid_3.png)