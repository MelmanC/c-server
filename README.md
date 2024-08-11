## Install
To install the project, you need to run the following command in the root directory of the project
```bash
make
```

## Launch
To launch the project, you need to run the following command in the root directory of the project.  
You can specify the directory to serve as an argument (for POST and GET requests)
```bash
./server <OPTION : [--directory]> <directory>
```

## Usage
The server will listen on the port 8080.  
You can send GET and POST requests to the server.  
The server will respond with the content of the file if it exists.  
If the request is a POST request, the server will create a file with the content of the request in the specified directory.  
