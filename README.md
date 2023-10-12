# ExampleDistccMacos

This project presents a comprehensive analysis of utilizing the *distcc* tool for distributed compilation on macOS.

In summary, while distcc operates effectively with command-line build systems such as Makefiles and Ninja, it is not compatible with Xcode as of today. Further details on this limitation are provided later in the documentation.

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

As highlighted earlier, distcc and Xcode are currently incompatible. This issue has been formally reported on distcc's GitHub repository, under [Issue #251](https://github.com/distcc/distcc/issues/251). While a patch has been proposed as a potential solution, it has not yet been officially implemented by the distcc development team. For those interested, the proposed patch can be found [here](https://github.com/PSPDFKit-labs/distcc/commit/5e4350d7e4e8a7667ce88f2bfb68250b91c004e9).

We're keeping the Xcode project setup in this project on purpose. That way, if the distcc team ever fixes the compatibility issue with Xcode, we can quickly test it out and maybe even start using it.

To reproduce the error, run the `generateXcode.sh` script. This will generate an Xcode project in the `_build/Xcode` directory and automatically launch Xcode with the generated project.

```
./generateXcode.sh
```

When you build, Xcode will give the following error.

```
Command CompileC failed with a nonzero exit code
```

## References
- https://www.distcc.org/
- https://pspdfkit.com/blog/2017/crazy-fast-builds-using-distcc/
- https://github.com/distcc/distcc/issues/251
