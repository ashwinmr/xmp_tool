# cpp_example
Example cpp program

# Build

```
git clone --recursive git@github.com:ashwinmr/cpp_example.git
cd cpp_example
mkdir build
cd build
cmake ..
```

## Mac/Linus
```
make
```

## Windows
```
cmake --build .
```

# Run

## Mac/Linux
```
cd build
./cpp_example
```

## Windows
```
cd Debug
./cpp_example.exe
```

# Install
```
cd build
sudo make install
```

# Running tests
```
cd build
ctest -V
```
