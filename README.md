# http-client-cpp

## Install

sudo mkdir http-client
sudo cp include/*.hpp /usr/include/http-client/
g++ -shared -o build/libhttp_client.so -std=c++14 src/*.cpp -fPIC -lcurl
sudo cp build/libhttp_client.so /usr/lib/
