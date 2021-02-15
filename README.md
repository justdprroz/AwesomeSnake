# AwesomeSnake
My online snake game project

# Requirements

### **You must have [SFML library](https://www.sfml-dev.org/) installed**

#### To install you can run one of this commands

On Arch Linux:  
```pacman -S sfml```

On Ubuntu Linux:  
```apt-get install libsfml-dev```

On Widnows:  
:exclamation::exclamation: **I am so sorry but Windows is unsuppoted** :exclamation::exclamation:

# Build & Run
## To build
Run ```build.sh``` script without arguments to compile **Release** build or with *Debug/debug/D/d* to compile **Debug** build
```
sh build.sh
sh build.sh Debug/debug/D/d
```

## To run
**You should firstly run server**
#### For server
```./build/snakeserver <ip> <port>```

Example 
```
./build/snakeserver 127.0.0.1 36876
```
#### For client
```./build/snakeclient <ip> <port>```

Example  
```
./build/snakeclient 127.0.0.1 36876
```
