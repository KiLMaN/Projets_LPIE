#ifndef GESTION_RESEAU_H
#define GESTION_RESEAU_H

#ifdef WIN32
    /* Do windows stuff */
    #include <windows.h>
    #include <winsock2.h>
    //#pragma comment(lib, "ws2_32.lib") // A faire dans le compilo sur Code :: Blocks
    typedef int MySocketLenght ;
    typedef SOCKET MySocket ;
#else
    /* Do linux stuff */
    #include <sys/socket.h>
    #include <netinet/in.h>
     typedef int MySocket ;
     typedef socklen_t MySocketLenght ;
#endif


#endif //GESTION_RESEAU_H
