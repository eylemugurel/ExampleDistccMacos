# ExampleDistccMacos

This project presents a comprehensive analysis of utilizing the *distcc* tool for distributed compilation on macOS.

## Installation

A minimum of two Mac machines is required for this experiment. Ensure that the following tools are installed on each machine.

it's strongly recommended that all machines participating in distributed compilation have the same versions of the operating system, runtime libraries, and compilers.

### Homebrew

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

```
echo "export PATH=/opt/homebrew/bin:$PATH" >> ~/.zshrc
```

```
source ~/.zshrc
```

### Distcc

```
brew install distcc
```

### Xcode

Download Xcode from the App Store. The version used in this guide is 14.2 as of writing.

## Configuration

In real-world scenarios, distcc-enabled machines can function as both clients and servers. However, to simplify our experiment, we will configure one machine strictly as a client and another as a server.

In these experiments, the machines were set up on a local network and assigned IP addresses in the 192.168.x.x range (192.168.0.0/16).

### Client

To begin, you'll need to identify the local IP address of the server machine. Run the following command on the server machine:

```
ipconfig getifaddr en0
```

Once you have the server's IP address, add it to the `~/.distcc/hosts` file on the client machine. If you're working with multiple servers, you can add each IP address on a new line.

For example, if the server's IP address is `192.168.64.10`:

```
echo "192.168.64.10" >> ~/.distcc/hosts
```

To keep an eye on what's happening in real-time, launch distcc's monitoring tool on the client machine:

```
distccmon-text 1
```

### Server

Initiate *distccd*, which is the distcc server component, to allow it listening for incoming compilation requests from authorized clients.

```
distccd --allow 192.168.0.0/16 --daemon --no-detach --log-stderr --enable-tcp-insecure
```

Here's a breakdown of each option:

- `--allow 192.168.0.0/16`: Specifies the IP address range that is allowed to connect to the distcc server. In this case, any machine with an IP address in the 192.168.x.x range can connect.
- `--daemon`: This option allows distccd to run independently, listening for incoming requests without needing another service to manage it.
- `--no-detach`: Keeps the distcc daemon in the foreground, allowing it to be terminated with Ctrl+C, which is useful for debugging.
- `--log-stderr`: Send log messages to stderr, rather than to a file or syslog. This is mainly intended for use in debugging.
- `--enable-tcp-insecure`: This flag allows distcc to accept TCP connections without its default security restrictions. While this setting can be useful for testing, it's generally not recommended for use on public networks due to the increased security risks. It may be acceptable within a trusted local network environment.

## Usage

The provided `CMakeLists.txt` script checks whether distcc is installed and, if so, creates the compiler wrappers `distcc_cc.sh` and `distcc_cxx.sh` in the build directory. It then configures these scripts to be used for the compilation instead of the actual compilers.

The `DISTCC_FALLBACK=0` setting is included in wrapper bash scrips. This is specifically for testing purposes to ensure that all compilation tasks are strictly offloaded to remote machines. To include the local machine in the compilation process, remove this setting after the testing phase is complete.

### Make

Run the `generateMake.sh` script. This will generate a Makefiles project in the `_build/Make` directory. Then, run the `make` command in this directory.

```
./generateMake.sh
```

```
cd _build/Make
```

```
make
```

### Xcode

The compatibility issue between distcc and Xcode, discussed under [Issue #492](https://github.com/distcc/distcc/issues/492) in the distcc GitHub repository, has been resolved in this project.

To address the incompatibility, we have modified our Xcode project setup by disabling the **Enable Index-While-Building Functionality** feature, which was known to conflict with distcc's operations. The following setting in the project's CMake configuration prevents Xcode from utilizing the `-index-store-path` compiler flag:

```
set(CMAKE_XCODE_ATTRIBUTE_COMPILER_INDEX_STORE_ENABLE NO)
```

This flag was identified as the root cause of build failures when using distcc. With this adjustment, Xcode and distcc can now be used together seamlessly.

To set up and launch the Xcode project, run the `generateXcode.sh` script. This script generates an Xcode project in the `_build/Xcode` directory and automatically opens it in Xcode:

```
./generateXcode.sh
```

Once the Xcode project opens, proceed with building as you would typically. Press the **Run** button or use the `Cmd+B` shortcut to start the build process. distcc will now collaborate with Xcode to compile your project efficiently.

## References
- https://www.distcc.org/
