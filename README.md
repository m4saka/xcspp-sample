# xcspp-sample
This is a minimum working sample that uses the xcspp library (https://github.com/m4saka/xcspp).
As an example, this program solves 6-bit Multiplexer Problem, but you can replace it with your own class.

Compatible with: Linux/Windows/macOS

## Compilation
1. Clone this repository
```
$ git clone --recursive https://github.com/m4saka/xcspp-sample.git
$ cd xcspp-sample
```
Note: Please don't forget to use `--recursive` option of `git clone` so that you can clone the xcspp library as a submodule (= sub-repository of git).

2. Create `build` directory and run cmake
```
$ mkdir build
$ cd build
$ cmake ..
```

3. Build the project
```
$ cmake --build .
```

4. Run

Linux/macOS:
```
$ ./sample
```

Windows (with Visual Studio):
```
$ Debug/sample.exe
```
