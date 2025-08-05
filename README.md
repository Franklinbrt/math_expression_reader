# math_expression_reader

A C++ package for calculating saved math expressions in real time using the [exprtk](https://github.com/ArashPartow/exprtk) library

## 1. Building Package

Run the following commands in order to build the package

```bash
  cd build
  cmake ..
  cmake --build .
```

Run the following command line in order to calculate a value according to fixed parameters ./matrix_read

```bash
./matrix_read_1
```

Or, for random parameters, run the following one

```bash
./matrix_read_2
```
Once the matrix is saved on the .txt file as shown in the image below

<img width="671" height="190" alt="image" src="https://github.com/user-attachments/assets/307c2db4-1b43-4f75-a4bd-0090eabae759" />

It's possible to read it in real time and substitute the parameters values accordingly on the matrix.

<img width="563" height="475" alt="image" src="https://github.com/user-attachments/assets/27d6802e-53a7-4619-a7fc-71f2154b5d57" />
