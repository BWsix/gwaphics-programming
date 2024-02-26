# gwaphics-programming

This repository contains my weekly assignments and projects from an introductory
computer graphics course focusing on OpenGL.

### Quick start

working with command line

```bash
git clone https://github.com/BWsix/gwaphics-programming.git
cd gwaphics-programming
git submodule init
cmake -S . -B build
cmake --build build -j
./build/projects/PROJECT/EXECUTABLE
```

working with visual studio

1. Open Visual Studio
2. In `Get started`, select `Clone a repository`
3. In `Repository location`, enter `https://github.com/BWsix/gwaphics-programming.git`
4. Set root level CMakeLists.txt as Startup item
5. Run build

### Genereate a new project

```bash
./gen_project.sh PROJECT_NAME
```
