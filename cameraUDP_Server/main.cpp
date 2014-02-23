//
//  main.cpp
//  cameraUDP_Server
//
//  Created by Katsuya Fujii on 2/20/14.
//  Copyright (c) 2014 Katsuya Fujii. All rights reserved.
//

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main(int argc, const char * argv[])
{

    int sock;
    struct sockaddr_in addr;
  
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_len = sizeof(addr);
    
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
   

    char windowName[] = "camera";
    cvNamedWindow( windowName, CV_WINDOW_AUTOSIZE );
    cv::Mat image = cv::Mat(480,640,CV_8UC3);
   // IplImage *frame =  cvCreateImage(cvSize(320,240), IPL_DEPTH_8U,3);
    
    static const int receiveSize = 65000;    //１度に受信する画像のバッファサイズ        60KB(61440byte)
    static char buff[receiveSize];            //受信するバッファ(ヘッダ部 + 画像データ)
     
    int c;
    int re;
    vector<uchar> ibuff;
    memset(buff, 0, 5);
  
    
        /* パケット受信 */
    while(1){
       printf("Recieved\n");
      memset(buff, 0, receiveSize);
       re = recvfrom(sock, buff, receiveSize, 0, NULL,NULL);
       
        if(re!=-1){//エラーで無ければ表示
            for(int i=0; i<sizeof(buff) ; i++){
                ibuff.push_back((uchar)buff[i]);
            }
            image = imdecode( Mat(ibuff), CV_LOAD_IMAGE_COLOR);
            cv::imshow(windowName, image);
            ibuff.clear();
        }else{
            perror("sock");
        }
        
        c = cvWaitKey (1);
        if (c == '\x1b')
            break;
    }

    
    close(sock);
    
    return 0;
}

