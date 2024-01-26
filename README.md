# Tracer

A simple raytracer, based on [Ray Trancing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## How to Use

Either use the vscode tasks `cmake` and `build` or manually:
```sh
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

Run the script using:
```sh
$ ./build/src/tracer > image.ppm
$ mogrify -format jpg *.ppm # optional
```

## Updates

A single object in the scene.

![01_sphere](./images/01_sphere.jpg)
---

Coloring the sphere based on the direction of the normal vector (first image is a bug).

![02_chroma_bug](./images/02_chroma_BUG.jpg)
![03_normal](./images/03_normal.jpg)
---

Adding more objects to our world.

![04_world](./images/04_world.jpg)
---

Adding anti-aliasing with 5, 10, and 50 samples for pixel, respectively.

![05_samples](./images/05_aliasing_5samples.jpg)
![06_samples](./images/06_aliasing_10samples.jpg)
![07_samples](./images/07_aliasing_50samples.jpg)
---

Adding a mate texture to our spheres with 10 and 100 samples per pixel, respectively.

![08_diffusion](./images/08_diffusion_10samples.jpg)
![09_diffusion](./images/09_diffusion_100samples.jpg)
---

Removing shadow acne.

![10_shadow_acne](./images/10_shadow_acne.jpg)
---

Improving light reflection by using a lambertian distribution.

![11_lambertian_reflection](./images/11_lambertian_reflection.jpg)
---

Applying gamma correction.

![12_gamma_correction](./images/12_gamma_correction.jpg)
---

Adding new materials: lambertian and fuzzy metal.

![13_lambertian](./images/13_lambertian.jpg)
![14_metal](./images/14_metal.jpg)
![15_fuzzy_metal](./images/15_fuzzy_metal.jpg)

---

Adding new materials: dielectrics and hollow glass sphere.

![16_dielectrics](./images/16_dielectrics.jpg)
![17_hollow_glass](./images/17_hollow_glass.jpg)