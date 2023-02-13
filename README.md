# ARP-Assignment2 Submitted for the 14th February exam of ARP
This project is using shared memory, which is a type of inter-process communication (IPC) mechanism in operating systems, which allows multiple processes to access a common region of memory. This provides a way for processes to exchange data and synchronize their activities without requiring the overhead of system calls or network communication.

In shared memory, a portion of system memory is set aside as a shared region, which can be accessed by multiple processes. Each process can read and write to this region, and changes made by one process are immediately visible to all other processes accessing the same shared memory. The operating system is responsible for managing access to the shared memory and ensuring that data consistency is maintained, even in the presence of concurrent access.

Shared memory is a fast IPC mechanism because it avoids the overhead of system calls or network communication. However, it also requires coordination between processes to ensure that data consistency is maintained, which can be complex and error-prone. Additionally, shared memory can lead to race conditions and other synchronization problems if not used correctly.

Overall, shared memory is an important concept in operating systems and is widely used in a variety of applications, including database management systems, multi-threaded programs, and scientific simulations.

Base repository for the **second ARP assignment**.
The two processes involved in the simulation of the vision system, namely **processA** and **processB**, are implemented as simple *ncurses windows*. The development of the inter-process communication pipeline, that is the shared memory.

You also find a **master** process, responsible of spawning the entire simulation.

## *libbitmap* installation and usage
To work with the bitmap library, you need to follow these steps:
1. Download the source code from [this GitHub repo](https://github.com/draekko/libbitmap.git) in your file system.
2. Navigate to the root directory of the downloaded repo and run the configuration through command ```./configure```. Configuration might take a while.  While running, it prints some messages telling which features it is checking for.

if the *./configure* is giving you error you just need to check if you have extracted the downloaded files and also you should be in the respected folder and check if you can see the configure by the ```ls``` again if gives error use these commands to update ubuntu. 
```
sudo apt-get update 
sudo apt-get install build-essential
``` 
then you can use to find out build GNU type
```
./configure --build=$(dpkg-architecture -qDEB_BUILD_GNU_TYPE)
```

3. Type ```make``` to compile the package.
4. Run ```make install``` to install the programs and any data files and documentation.
5. Upon completing the installation, check that the files have been properly installed by navigating to ```/usr/local/lib```, where you should find the ```libbmp.so``` shared library ready for use.
6. In order to properly compile programs which use the *libbitmap* library, you first need to notify the **linker** about the location of the shared library. To do that, you can simply add the following line at the end of your ```.bashrc``` file:      
```export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"```

## Compiling and running **processA** and **processB**
The two processes are implemented as UIs through *ncurses* library, therefore you need to compile their source files by linking the shared library via ```-lncurses```. As for the first assignment, exploit the resize event of the windows to get out of situations in which the graphical elements do not properly spawn.

```
clear
gcc ./src/master.c -lm -lrt  -o ./bin/master
gcc ./src/processA.c -lbmp -lncurses -lm -lrt -o ./bin/processA
gcc ./src/processB.c -lbmp -lncurses -lm -lrt -o ./bin/processB
./bin/master
```


