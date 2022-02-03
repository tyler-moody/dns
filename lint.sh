find . -iname "*.[c,h]" | xargs clang-format -i
find . -iname "*.cpp" | xargs clang-format -i
