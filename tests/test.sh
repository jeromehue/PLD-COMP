if [ -d "out" ]; then
    rm -Rf "out";
fi

chmod u+x wrapper.sh
python3 test.py tests/
