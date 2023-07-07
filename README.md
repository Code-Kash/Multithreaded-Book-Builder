# Multithreaded Order Book Builder

The Multithreaded Order Book Builder is a program designed to compile order data from multiple files into a single large file, ordered by time. It utilizes multithreaded synchronization techniques to optimize the process and improve performance.

## Features

- Processes 10,000 files, each corresponding to a separate stock ticker.
- Compiles all orders from the input files into a single large file.
- Orders in the output file are ordered by time.
- Utilizes multithreaded synchronization techniques for improved performance.

## Getting Started

### Prerequisites

- C++ compiler compatible with C++20 or higher.
- Standard Template Library (STL) support.
- CMake (optional, for building the project).

### Installation

1. Clone the repository or download the source code.

git clone <repository_url>

2. Build the project (if using CMake).

cd <project_directory>
mkdir build
cd build
cmake ..
cmake --build .

### Usage

1. Place the input files in the designated directory.
2. Run the compiled executable or the main program.
3. The program will process the input files, compile the order data, and generate the output file.
4. The output file will contain all orders compiled from the input files, ordered by time.

### Configuration

- The program allows configuration of the input directory path, output file path, and other settings.
- Modify the configuration parameters in the source code or provide them as command-line arguments.

## Performance

The Multithreaded Order Book Builder utilizes multithreaded synchronization techniques to optimize the compilation process and improve performance. By leveraging multiple threads, the program can process the input files concurrently, reducing the overall execution time.

The synchronization mechanisms, such as mutexes, condition variables, and lock guards, ensure proper coordination and synchronization between threads, preventing data races and maintaining the integrity of the compiled order data.

## Contributing

Contributions to the Multithreaded Order Book Builder are welcome. If you find any issues or have suggestions for improvements, please create an issue or submit a pull request.

## Acknowledgements

The Multithreaded Order Book Builder makes use of various libraries and technologies, including:

- C++ Standard Library
- Multithreading and synchronization techniques
- File I/O operations

## Contact

For any inquiries or further information, please contact [anupkash123@gmail.com].


