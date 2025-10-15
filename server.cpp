#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

// IMPORTANT: Link with ws2_32.lib
// In your compiler: -lws2_32
// Or in Visual Studio: Project Properties -> Linker -> Input -> Additional Dependencies -> ws2_32.lib

int main() {
    // STEP 0: Initialize WinSock (WINDOWS ONLY!)
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }
    std::cout << "WinSock initialized!\n";
    
    // STEP 1: Create socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created!\n";
    
    // STEP 2: Setup address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // STEP 3: Bind
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Bound to port 8080\n";
    
    // STEP 4: Listen
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Listening for connections...\n";
    
    // STEP 5: Accept
    sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
    
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Client connected!\n";
    
    // STEP 6: Receive data
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (bytes_received > 0) {
        std::cout << "Received " << bytes_received << " bytes\n";
        buffer[bytes_received] = '\0';
        std::cout << "Data: " << buffer << "\n";
    }
    
    // STEP 7: Send response
    const char* response = "Hello from Windows server!";
    send(client_socket, response, strlen(response), 0);
    std::cout << "Response sent!\n";
    
    // STEP 8: Cleanup (IMPORTANT!)
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();  // Clean up WinSock
    
    std::cout << "Server closed\n";
    return 0;
}