#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<unistd.h>


                const int width=80;const int
             height=30;const std::string dark_gray
         ="\033[90m";const std::string light_gray =
       "\033[37m";const std::string red="\033[31m";const
      std::string reset="\033[0m";void renderDeathStar()
    {std::vector<std::vector<char>>screen(height,std::vector<
   char>(width,        ' '));static double angle=0;angle+=0.05;
  for(int i=0;            i<height;i++){for(int j=0;j<width;j++)
 {double x=(j            -width/2.0)/(width/4.0);double y=(i-height
 /2.0)/(height           /3.0);double rx=x*cos(angle)-y*sin(angle);
 double ry=x*sin(angle)+y*cos(angle);double dist=rx*rx+ry*ry;if(dist<
 1.0){double crater_x=rx-0.4;double crater_y=ry-0.1;double cr_dist
 =crater_x*crater_x+crater_y*crater_y;if(cr_dist<0.1){if(cr_dist<
 0.02){screen[i][j]='O';}else{screen[i][j]='0';}}else{if(fmod(j+i*2+
 angle*50,6)<2){screen[i][j]='#';}else{screen[i][j]='.';}}}}}std::cout
  <<"\x1b[H";std::cout<<"ЗВЕЗДА СМЕРТИ Орбитальная боевая станция\n"
  ;for(int i=0;i<height;i++){for(int j=0;j<width;j++){char c=screen
    [i][j];if(c=='O'){std::cout<<red<<c;}else if(c=='0'){std::cout
    <<light_gray<<c;}else if(c=='#'){std::cout<<dark_gray<<c;}else
    if (c=='.'){std::cout<<light_gray<<c;}else{std::cout<<' ';}}
    std::cout<<reset<<'\n';}std::cout<<std::flush;}int main(){
     std::cout<<"\x1b[2J\x1b[?25l"<<std::flush;try{while(true
        ){renderDeathStar();usleep(100000);}}catch(...){std
            ::cout<<"\x1b[?25h\n";}return 0;return 0;}




            