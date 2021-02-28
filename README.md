# AwesomeSnake

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9aba85ea869247dc9539af22fa740b43)](https://app.codacy.com/gh/mysterious-games/AwesomeSnake?utm_source=github.com&utm_medium=referral&utm_content=mysterious-games/AwesomeSnake&utm_campaign=Badge_Grade)

My online snake game project

## Build & Run
### To build
Run ```build.sh``` script without arguments to compile **Release** build or with *Debug/debug/D/d* to compile **Debug** build
```bash
sh build.sh
sh build.sh Debug/debug/D/d
```

### To run
**You should firstly run server**

**For server**  
Command  
```./build/snakeserver <ip> <port>```

Example 
```bash
./build/snakeserver 127.0.0.1 36876
```
**For client**  
Command  
```./build/snakeclient <ip> <port>```

Example  
```bash
./build/snakeclient 127.0.0.1 36876
```
