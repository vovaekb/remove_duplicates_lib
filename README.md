# remove_duplicates_lib

Simple shared library for remove duplicate image files from specific folder.

Stack:
- OpenCV
- Conan

Install:

```
conan install .
conan build .
```

## Run tests
Copy test data from folder data to bin
```
cp -r data/files bin
```

Move to folder bin and run tests:

```
cd bin && ./library_test
```
