#ifndef server_hpp
#define server_hpp

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <fcntl.h> // For fcntl
#include <netdb.h> // For addrinfo
#include <arpa/inet.h> // For inet_ntop
#include <map>

#define PORT "4242"

char* parse(char line[], const char symbol[])
{
    char *copy = strdup(line);
    
    char* message = NULL;
    char* token = strtok(copy, symbol);
    int current = 0;

    while( token != NULL ) {
      
      token = strtok(NULL, " ");
      if(current == 0){
          message = token;
          if(message == NULL){
              message = NULL;
          }
          return message;
      }
      current = current + 1;
   }
//    free(token);
//    free(copy);
   return message;
}

char* parse_method(char line[], const char symbol[])
{
    char *copy = strdup(line);
        
    char *message = NULL;
    char * token = strtok(copy, symbol);
    int current = 0;

    while( token != NULL ) {
      
      //token = strtok(NULL, " ");
      if(current == 0){
          message = token;
          if(message == NULL){
              message = NULL;
          }
          return message;
      }
      current = current + 1;
   }
   free(copy);
   free(token);
   return message;
}

// char* find_token(char line[], const char symbol[], const char match[])
// {
//     char *copy = (char *)malloc(strlen(line) + 1);
//     strcpy(copy, line);
        
//     char *message;
//     char * token = strtok(copy, symbol);

//     while( token != NULL ) {
      
//       //printf("--Token: %s \n", token);
      
//       if(strlen(match) <= strlen(token))
//       {
//           int match_char = 0;
//           for(int i = 0; i < strlen(match); i++)
//           {
//               if(token[i] == match[i])
//               {
//                   match_char++;
//               }
//           }
//           if(match_char == strlen(match)){
//             message = token;
//             return message;
//           }
//       }      
//       token = strtok(NULL, symbol);
//    }
//    free(copy);
//    free(token);
//    message = "";
//    return message;
// }

// int send_message(int fd, char image_path[], char head[]){
//     /*
//     char imageheader[] = 
//     "HTTP/1.1 200 Ok\r\n"
//     "Content-Type: image/jpeg\r\n\r\n";
//     */
//     struct stat stat_buf;  /* hold information about input file */

//     write(fd, head, strlen(head));

//     int fdimg = open(image_path, O_RDONLY);
    
//     if(fdimg < 0){
//         printf("Cannot Open file path : %s with error %d\n", image_path, fdimg); 
//     }
     
//     fstat(fdimg, &stat_buf);
//     int img_total_size = stat_buf.st_size;
//     int block_size = stat_buf.st_blksize;
//     //printf("image block size: %d\n", stat_buf.st_blksize);  
//     //printf("image total byte st_size: %d\n", stat_buf.st_size);
//     if(fdimg >= 0){
//         ssize_t sent_size;

//         while(img_total_size > 0){
//             //if(img_total_size < block_size){
//              //   sent_size = sendfile(fd, fdimg, NULL, img_total_size);
//             //}
//             //else{
//             //    sent_size = sendfile(fd, fdimg, NULL, block_size);
//             //}          
//             //img_total_size = img_total_size - sent_size;
        
//             //if(sent_size < 0){
//              //   printf("send file error --> file: %d, send size: %d , error: %s\n", fdimg, sent_size, strerror(errno));
//              //   img_total_size = -1;
//               int send_bytes = ((img_total_size < block_size) ? img_total_size : block_size);
//               int done_bytes = sendfile(fd, fdimg, NULL, block_size);
//               img_total_size = img_total_size - done_bytes;
//             //}
//         }
//         if(sent_size >= 0){
//             printf("send file: %s \n" , image_path);
//         }
//         close(fdimg);
//     }
// }


#endif
