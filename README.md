# Chronos Sync RPC
A simple way to synchronize the clock between two machines over a network.


## 1. Requirements
This project was created over Ubuntu 14.04.
You'll need to install some additional packages on your server and developer machines.
Just run the following command:
```
# sudo apt-get install -y rpcbind gcc build-essential
```


## 2. How to use
Run the server and open the client to get the server's clock.

### 2.1. Running the server
Run as root:
```
# sudo ./dist/server
```

### 2.2. Running the client (command line)
Run as root: "./dist/client <server_address> <method[0-2]>"
             @method: [0-Basic] [1-Christian] [2-NTP]
Example:
```
$ ./dist/client 192.168.0.105 0
```

### 2.3. Running the client from GUI
The client with GUI needs to be run from a super user.
```
# sudo ./chronos.sh
```


## 3. How to build
Run as a normal user, just run:
```
$ ./compile.sh
```
