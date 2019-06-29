# http-client-cpp

## Dependencies

<a href="https://github.com/curl/curl">curl</a>

## Install

sudo mkdir /usr/include/http-client/<br>
sudo cp include/\*.hpp /usr/include/http-client/<br>
g++ -shared -o build/libhttp_client.so -std=c++14 src/\*.cpp -fPIC -lcurl<br>
sudo cp build/libhttp_client.so /usr/lib/
