# Pipex

Pipex is a project designed to explore the inner workings of UNIX pipes by implementing them in C. This program replicates the following shell behavior:  

```sh
< file1 cmd1 | cmd2 > file2
```

## 🚀 Features  
- Execution of two chained commands through a pipe.  
- Proper error handling and memory allocation without leaks.  
- Compliance with UNIX conventions and `execve` compatibility.  

## 📌 Requirements  
- A UNIX-based system  
- A C compiler (`gcc` or `clang`)  
- `make` for compilation  

## 🔧 Installation & Compilation  
Clone the repository and compile the project:  
```sh
git clone https://github.com/your-username/pipex.git
cd pipex
make
```

## ▶️ Usage  
```sh
./pipex file1 "cmd1" "cmd2" file2
```
Example:  
```sh
./pipex infile "ls -l" "wc -l" outfile
```
is equivalent to:  
```sh
< infile ls -l | wc -l > outfile
```

## 🎯 Bonus Features  
- Handling multiple commands:  
  ```sh
  ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
  ```
  Equivalent to:  
  ```sh
  < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
  ```

- Support for **"here_doc"** mode:  
  ```sh
  ./pipex here_doc LIMITER cmd cmd1 file
  ```
  Equivalent to:  
  ```sh
  cmd << LIMITER | cmd1 >> file
  ```

---

