# Avoids the "missing folder" error message
if [ -d "out" ]; then
    rm -Rf "out";
fi

# Making sure wrapper.sh is executable
chmod u+x wrapper.sh

# Run tests
python3 test.py tests/