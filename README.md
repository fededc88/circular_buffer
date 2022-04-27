# General Purpouse Circular Buffer

This C library is supposed to solve one of the most common problems when
developing. Implements a bunch of functions to deal with an user defined array
of any kind as a circular buffer.

Users should statically allocate on memory an array of any kind and provide to
the library the underlying array pointer, the array size, and the size of the
elements when initializing the circular buffer library and it will return the
pointer to a circular buffer handler to them.                                            
                                                                              
Systems which cannot use dynamic memory simply need to statically allocate
memory for a buffer struct instance and take a look to
circular_buffer_struct_init() function to initialize it.

## Documentation

You could find detailed Doxygen documentation clicking [here](https://fededc88.github.io/circular_buffer/).

## Folder structure

* **src**: Contains the circular buffer library source and header files.

* **test**: Contains the unit tests related to different algorithms of the
  project. 
  
* **docs**: Contains html Doxygen documentation source files. See [Documentation](#Documentation)

### Get the repo

In order to clone and get repo do:

```
git clone https://github.com/fededc88/circular_buffer.git 
```

## Testing

I am running some local test for specific algorithms using
[Ceedling](http://www.throwtheswitch.org/ceedling). Ceedling is an automated
testing framework for C applications. Unity is the unit test framework provided
with Ceedling that give us all of the test assertions. Unity is written in 100%
pure C code. It follows ANSI standards while supporting most embedded compiler
quirks. To run them, just install  ceedling and get a C/C++
compiller to have the framework running. 

Ceedling requires Ruby to run and uses GCC to build each test.

* [1] Install Ruby
  * apt (Debian or Ubuntu)

  ```
  sudo apt-get install ruby-full
  ```

* [2] Install Ceedling with the Ruby "gem" tool

  ```
  gem install ceedling
  ```

* [3] Install GCC 
  * apt (Debian or Ubuntu)

  ```
  sudo apt install build-essential
  ```

* And them, to compile source files and execute tests run:

  ```
  ceedling test
  ```

Ceedling will present you the results as console outputs. 

## Referencing

author: ***[Federico D. Ceccarelli](https://github.com/fededc88)***

Do you find this project interesting or useful for your work? Please let me know 
fededc88@gmail.com, [@fededc88](https://github.com/fededc88).

Any kind of submission are welcome!
