#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

// Link with ws2_32.lib

int main() {
    // STEP 0: Initialize WinSock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }
    std::cout << "WinSock initialized!\n";
    
    // STEP 1: Create socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created!\n";
    
    // STEP 2: Setup server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    
    // Convert IP address
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    // STEP 3: Connect
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << "\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server!\n";
    
    // STEP 4: Send data
    const char* message = "Hello from Windows client!";
    send(client_socket, message, strlen(message), 0);
    std::cout << "Message sent!\n";
    
    // STEP 5: Receive response
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Server replied: " << buffer << "\n";
    }
    
    // STEP 6: Cleanup
    closesocket(client_socket);
    WSACleanup();
    
    std::cout << "Client closed\n";
    return 0;
}