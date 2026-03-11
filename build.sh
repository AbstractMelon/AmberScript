#!/bin/bash

# Build the compiler
echo "Building compiler..."
gcc asc.c -o asc || { echo "Failed to build compiler"; exit 1; }

# Compile the sample script
echo "Compiling sample.as..."
./asc sample.as || { echo "Failed to compile script"; exit 1; }

# Build the Linux executable
echo "Building Linux executable..."
gcc generated.c -o sample_linux || { echo "Failed to build Linux executable"; exit 1; }

# Build the Windows executable
echo "Building Windows executable..."
x86_64-w64-mingw32-gcc generated.c -o sample.exe || { echo "Failed to build Windows executable"; exit 1; }

# Create a wrapper script
echo "Creating wrapper script..."
echo '#!/bin/bash' > run.sh
echo 'OS="$(uname -s)"' >> run.sh
echo 'if [[ "$OS" == "Linux" ]]; then' >> run.sh
echo '    ./sample_linux' >> run.sh
echo 'elif [[ "$OS" == "MINGW"* || "$OS" == "MSYS"* || "$OS" == "CYGWIN"* ]]; then' >> run.sh
echo '    ./sample.exe' >> run.sh
echo 'else' >> run.sh
echo '    echo "Unsupported OS: $OS"' >> run.sh
echo '    exit 1' >> run.sh
echo 'fi' >> run.sh
chmod +x run.sh

echo "Build complete! Executables created: sample_linux, sample.exe, run.sh"