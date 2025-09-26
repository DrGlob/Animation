                                            #include<math.h>
                                            #include<iostream>
                                            #include<thread>
                                            #include<chrono>
            void m(int x,int y)                                             {std::cout
                <<"\033["<<y                                           <<";"<<x<<"H";}
                    void d()                                         {std::cout<<"\n";
                        int s=3;                                   const char
                        *b[]=                                    {"  O  ",
                        " \\O  ",                             "  O/ ",
                            " \\O/ "}                              ;
                                 const char                   *a[]={
                                            " /|\\ ","  |\\ "
                                            ," /|  ","  |  "};
                                            const char*l[]={
                                            " / \\ "," /|  ",
                                            "  |\\ ","  |  "};
                                            m(1,s);std::cout<<
                                            b[0];m(1,s+1);
                                            std::cout<<a[0];
                                            m(1,s+2);std::cout<<l[0];
                                            std::cout<<std::flush;
                                            for(int i=0;i<30;i++)
                                            {std::this_thread::
                                                sleep_for(
                        std::chrono::milliseconds                (200));
                    m(1,s);std::cout                               <<"\033[K"<<b[i%4];
                    m(1,s+1);                                           std::cout<<
                "\033[K"<<a[i%4];                                        m(1,s+2);
            std::cout<<                                                   "\033[K"<<l[i%4];
        std::cout<<                                                         std::flush;}m(1,s+4);
        std::cout<<                                                           "\n"<<std::endl;}
       int main()                                                                {d();return 0;}




