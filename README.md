# xmp_tool
A command line tool for manipulating xmp data (tags) in image files.

## Features
-   Read xmp data from file
-   Get tags from files
-   Add tags to files
-   Remove tags from files
-   Create a database of tags and query files

# Build and Install

```
git clone --recursive git@github.com:ashwinmr/xmp_tool.git
cd xmp_tool
mkdir build
cd build
cmake ..
```

For mac/linux
```
make
```

For windows
```
cmake --build .
```

## Running

On mac/linux
```
cd build
./xmp_tool
```

On windows
```
cd Debug
./xmp_tool
```

# Usage

The xmp_tool allows subcommands for performing different functions

## Reading xmp data from files/s
```
xmp_tool read <path/to/file> [<path/to/file2>]
```

It can also accept piped lists of files
```
echo <path/to/file> | xmp_tool read
```

## Get tags from file/s
```
xmp_tool get <path/to/file> [<path/to/file2>]
```

It can also accept piped lists of files
```
echo <path/to/file> | xmp_tool get
```

## Add tags to file/s
```
xmp_tool add <path/to/file> [<path/to/file2>] -t <tag_1> <tag_2> ...  
```

It can also accept piped lists of files
```
echo <path/to/file> | xmp_tool add -t <tag_1> <tag_2> ...
```

## Remove tags from file/s
```
xmp_tool rem <path/to/file> [<path/to/file2>] -t <tag_1> <tag_2> ...
```

It can also accept piped lists of files
```
echo <path/to/file> | xmp_tool rem -t <tag_1> <tag_2> ...
```

You can also use the following flags

-   -a : remove all tags
-   -d : remove duplicate tags

# Running tests
```
cd build
ctest -V
```
